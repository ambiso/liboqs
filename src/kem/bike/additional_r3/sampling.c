/* Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0"
 *
 * Written by Nir Drucker, Shay Gueron and Dusan Kostic,
 * AWS Cryptographic Algorithms Group.
 */

#include <assert.h>

#include "sampling.h"
#include "sampling_internal.h"
#include <oqs/rand.h>

// SIMD implementation of is_new function requires the size of wlist
// to be a multiple of the number of DWORDS in a SIMD register (REG_DWORDS).
// The function is used both for generating D and T random numbers so we define
// two separate macros.
#define AVX512_REG_DWORDS (16)
#define WLIST_SIZE_ADJUSTED_D \
  (AVX512_REG_DWORDS * DIVIDE_AND_CEIL(D, AVX512_REG_DWORDS))
#define WLIST_SIZE_ADJUSTED_T \
  (AVX512_REG_DWORDS * DIVIDE_AND_CEIL(T, AVX512_REG_DWORDS))

void get_seeds(OUT seeds_t *seeds)
{
  OQS_randombytes((uint8_t *)seeds, NUM_OF_SEEDS * sizeof(seed_t));

  for(uint32_t i = 0; i < NUM_OF_SEEDS; ++i) {
    print("s: ", (uint64_t *)&seeds->seed[i], SIZEOF_BITS(seed_t));
  }
}

// BSR returns ceil(log2(val))
_INLINE_ uint8_t bit_scan_reverse_vartime(IN uint64_t val)
{
  // index is always smaller than 64
  uint8_t index = 0;

  while(val != 0) {
    val >>= 1;
    index++;
  }

  return index;
}

_INLINE_ ret_t get_rand_mod_len(OUT int * trace, OUT uint32_t *    rand_pos,
                                IN const uint32_t len,
                                IN OUT aes_ctr_prf_state_t *prf_state)
{
  const uint64_t mask = MASK(bit_scan_reverse_vartime(len));

  do {
    if (trace != NULL) *trace += 1;
    // Generate a 32 bits (pseudo) random value.
    // This can be optimized to take only 16 bits.
    GUARD(aes_ctr_prf((uint8_t *)rand_pos, prf_state, sizeof(*rand_pos)));

    // Mask relevant bits only
    (*rand_pos) &= mask;

    // Break if a number that is smaller than len is found
    if((*rand_pos) < len) {
      break;
    }

  } while(1 == 1);

  return SUCCESS;
}

_INLINE_ void make_odd_weight(IN OUT r_t *r)
{
  if(((r_bits_vector_weight(r) % 2) == 1)) {
    // Already odd
    return;
  }

  r->raw[0] ^= 1;
}

// Returns an array of r pseudorandom bits.
// No restrictions exist for the top or bottom bits.
// If the generation requires an odd number, then set must_be_odd=1.
// The function uses the provided prf context.
ret_t sample_uniform_r_bits_with_fixed_prf_context(
  OUT r_t *r,
  IN OUT aes_ctr_prf_state_t *prf_state,
  IN const must_be_odd_t      must_be_odd)
{
  // Generate random data
  GUARD(aes_ctr_prf(r->raw, prf_state, R_BYTES));

  // Mask upper bits of the MSByte
  r->raw[R_BYTES - 1] &= MASK(R_BITS + 8 - (R_BYTES * 8));

  if(must_be_odd == MUST_BE_ODD) {
    make_odd_weight(r);
  }

  return SUCCESS;
}

_INLINE_ ret_t generate_indices_mod_z(OUT int * trace, OUT idx_t *     out,
                             IN const size_t num_indices,
                             IN const size_t z,
                             IN OUT aes_ctr_prf_state_t *prf_state,
                             IN const sampling_ctx *ctx)
{
  size_t ctr = 0;
  

  // Generate num_indices unique (pseudo) random numbers modulo z
  do {
    if (trace != NULL) *trace += 1;
    GUARD(get_rand_mod_len(trace, &out[ctr], z, prf_state));
    ctr += ctx->is_new(out, ctr);
  } while(ctr < num_indices);

  return SUCCESS;
}

// Returns an array of r pseudorandom bits.
// No restrictions exist for the top or bottom bits.
// If the generation requires an odd number, then set must_be_odd = MUST_BE_ODD
ret_t sample_uniform_r_bits(OUT r_t *r,
                            IN const seed_t *      seed,
                            IN const must_be_odd_t must_be_odd)
{
  // For the seedexpander
  DEFER_CLEANUP(aes_ctr_prf_state_t prf_state = {0}, aes_ctr_prf_state_cleanup);

  GUARD(init_aes_ctr_prf_state(&prf_state, MAX_AES_INVOKATION, seed));

  GUARD(sample_uniform_r_bits_with_fixed_prf_context(r, &prf_state, must_be_odd));

  return SUCCESS;
}

ret_t generate_sparse_rep(OUT pad_r_t *r,
                          OUT idx_t *wlist,
                          IN OUT aes_ctr_prf_state_t *prf_state)
{

  // Initialize the sampling context
  sampling_ctx ctx;
  sampling_ctx_init(&ctx);

  idx_t wlist_temp[WLIST_SIZE_ADJUSTED_D] = {0};

  GUARD(generate_indices_mod_z(NULL, wlist_temp, D, R_BITS, prf_state, &ctx));

  bike_memcpy(wlist, wlist_temp, D * sizeof(idx_t));
  ctx.secure_set_bits(r, 0, wlist, D);

  return SUCCESS;
}

ret_t generate_error_vector(OUT int * trace, OUT pad_e_t *e, IN const seed_t *seed)
{
  DEFER_CLEANUP(aes_ctr_prf_state_t prf_state = {0}, aes_ctr_prf_state_cleanup);

  GUARD(init_aes_ctr_prf_state(&prf_state, MAX_AES_INVOKATION, seed));

  // Initialize the sampling context
  sampling_ctx ctx;
  sampling_ctx_init(&ctx);

  if (trace != NULL) *trace += 1;

  idx_t wlist[WLIST_SIZE_ADJUSTED_T] = {0};
  GUARD(generate_indices_mod_z(trace, wlist, T, N_BITS, &prf_state, &ctx));

  // (e0, e1) hold bits 0..R_BITS-1 and R_BITS..2*R_BITS-1 of the error, resp.
  ctx.secure_set_bits(&e->val[0], 0, wlist, T);
  ctx.secure_set_bits(&e->val[1], R_BITS, wlist, T);

  // Clean the padding of the elements
  PE0_RAW(e)[R_BYTES - 1] &= LAST_R_BYTE_MASK;
  PE1_RAW(e)[R_BYTES - 1] &= LAST_R_BYTE_MASK;
  bike_memset(&PE0_RAW(e)[R_BYTES], 0, R_PADDED_BYTES - R_BYTES);
  bike_memset(&PE1_RAW(e)[R_BYTES], 0, R_PADDED_BYTES - R_BYTES);

  return SUCCESS;
}

// New function by Alexander
ret_t make_error_vector(OUT pad_e_t *e, IN const idx_t *wlist, IN const int t)
{
  // (e0, e1) hold bits 0..R_BITS-1 and R_BITS..2*R_BITS-1 of the error, resp.
  secure_set_bits_port(&e->val[0], 0, wlist, t);
  secure_set_bits_port(&e->val[1], R_BITS, wlist, t);

  // Clean the padding of the elements
  PE0_RAW(e)[R_BYTES - 1] &= LAST_R_BYTE_MASK;
  PE1_RAW(e)[R_BYTES - 1] &= LAST_R_BYTE_MASK;
  bike_memset(&PE0_RAW(e)[R_BYTES], 0, R_PADDED_BYTES - R_BYTES);
  bike_memset(&PE1_RAW(e)[R_BYTES], 0, R_PADDED_BYTES - R_BYTES);

  return SUCCESS;
}
