// SPDX-License-Identifier: MIT

#ifndef OQS_KEM_HQC_H
#define OQS_KEM_HQC_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_hqc_128
#define OQS_KEM_hqc_128_length_public_key 2249
#define OQS_KEM_hqc_128_length_secret_key 2289
#define OQS_KEM_hqc_128_length_ciphertext 4481
#define OQS_KEM_hqc_128_length_shared_secret 64
#define OQS_KEM_hqc_128_length_plaintext 16
OQS_KEM *OQS_KEM_hqc_128_new(void);
OQS_API OQS_STATUS OQS_KEM_hqc_128_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_hqc_128_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_hqc_128_encaps_with_m(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key, const uint8_t *m);
OQS_API OQS_STATUS OQS_KEM_hqc_128_numrejections(uint8_t *plaintext, size_t *seed_expanded_bytes);
OQS_API OQS_STATUS OQS_KEM_hqc_128_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_hqc_192
#define OQS_KEM_hqc_192_length_public_key 4522
#define OQS_KEM_hqc_192_length_secret_key 4562
#define OQS_KEM_hqc_192_length_ciphertext 9026
#define OQS_KEM_hqc_192_length_shared_secret 64
#define OQS_KEM_hqc_192_length_plaintext 24
OQS_KEM *OQS_KEM_hqc_192_new(void);
OQS_API OQS_STATUS OQS_KEM_hqc_192_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_hqc_192_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_hqc_192_encaps_with_m(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key, const uint8_t *m);
OQS_API OQS_STATUS OQS_KEM_hqc_192_numrejections(uint8_t *plaintext, size_t *seed_expanded_bytes);
OQS_API OQS_STATUS OQS_KEM_hqc_192_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_hqc_256
#define OQS_KEM_hqc_256_length_public_key 7245
#define OQS_KEM_hqc_256_length_secret_key 7285
#define OQS_KEM_hqc_256_length_ciphertext 14469
#define OQS_KEM_hqc_256_length_shared_secret 64
#define OQS_KEM_hqc_256_length_plaintext 32
OQS_KEM *OQS_KEM_hqc_256_new(void);
OQS_API OQS_STATUS OQS_KEM_hqc_256_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_hqc_256_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_hqc_256_encaps_with_m(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key, const uint8_t *m);
OQS_API OQS_STATUS OQS_KEM_hqc_256_numrejections(uint8_t *plaintext, size_t *seed_expanded_bytes);
OQS_API OQS_STATUS OQS_KEM_hqc_256_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#endif

