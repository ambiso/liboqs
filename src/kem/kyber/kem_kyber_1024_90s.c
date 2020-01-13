// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <oqs/kem_kyber.h>

#if defined(OQS_ENABLE_KEM_kyber_1024_90s)

OQS_KEM *OQS_KEM_kyber_1024_90s_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_kyber_1024_90s;
	kem->alg_version = "https://github.com/pq-crystals/kyber/commit/28413dfbf523fdde181246451c2bd77199c0f7ff";

	kem->claimed_nist_level = 5;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_kyber_1024_90s_length_public_key;
	kem->length_secret_key = OQS_KEM_kyber_1024_90s_length_secret_key;
	kem->length_ciphertext = OQS_KEM_kyber_1024_90s_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_kyber_1024_90s_length_shared_secret;

	kem->keypair = OQS_KEM_kyber_1024_90s_keypair;
	kem->encaps = OQS_KEM_kyber_1024_90s_encaps;
	kem->decaps = OQS_KEM_kyber_1024_90s_decaps;

	return kem;
}

extern int pqcrystals_kyber1024_90s_ref_keypair(uint8_t *pk, uint8_t *sk);
extern int pqcrystals_kyber1024_90s_ref_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
extern int pqcrystals_kyber1024_90s_ref_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
extern int pqcrystals_kyber1024_90s_ref_dec_measure(uint8_t *ss, const uint8_t *ct, const uint8_t *sk, uint64_t *rdtsc_buffer, const unsigned char **rdtsc_labels, int *fail);

#if defined(OQS_ENABLE_KEM_kyber_1024_90s_avx2)
extern int pqcrystals_kyber1024_90s_avx2_keypair(uint8_t *pk, uint8_t *sk);
extern int pqcrystals_kyber1024_90s_avx2_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
extern int pqcrystals_kyber1024_90s_avx2_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
extern int pqcrystals_kyber1024_90s_avx2_dec_measure(uint8_t *ss, const uint8_t *ct, const uint8_t *sk, uint64_t *rdtsc_buffer, const unsigned char **rdtsc_labels, int *fail);
#endif

OQS_API OQS_STATUS OQS_KEM_kyber_1024_90s_keypair(uint8_t *public_key, uint8_t *secret_key) {
#if defined(OQS_ENABLE_KEM_kyber_1024_90s_avx2)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AES) && OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI2) && OQS_CPU_has_extension(OQS_CPU_EXT_POPCNT)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) pqcrystals_kyber1024_90s_avx2_keypair(public_key, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) pqcrystals_kyber1024_90s_ref_keypair(public_key, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) pqcrystals_kyber1024_90s_ref_keypair(public_key, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_KEM_kyber_1024_90s_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
#if defined(OQS_ENABLE_KEM_kyber_1024_90s_avx2)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AES) && OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI2) && OQS_CPU_has_extension(OQS_CPU_EXT_POPCNT)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) pqcrystals_kyber1024_90s_avx2_enc(ciphertext, shared_secret, public_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) pqcrystals_kyber1024_90s_ref_enc(ciphertext, shared_secret, public_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) pqcrystals_kyber1024_90s_ref_enc(ciphertext, shared_secret, public_key);
#endif
}

OQS_API OQS_STATUS OQS_KEM_kyber_1024_90s_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key) {
#if defined(OQS_ENABLE_KEM_kyber_1024_90s_avx2)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AES) && OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI2) && OQS_CPU_has_extension(OQS_CPU_EXT_POPCNT)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) pqcrystals_kyber1024_90s_avx2_dec(shared_secret, ciphertext, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) pqcrystals_kyber1024_90s_ref_dec(shared_secret, ciphertext, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) pqcrystals_kyber1024_90s_ref_dec(shared_secret, ciphertext, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_KEM_kyber_1024_90s_decaps_measure(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key, uint64_t *rdtsc_buffer, const unsigned char **rdtsc_labels, int *fail) {
#if defined(OQS_ENABLE_KEM_kyber_1024_90s_avx2)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AES) && OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI2) && OQS_CPU_has_extension(OQS_CPU_EXT_POPCNT)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) pqcrystals_kyber1024_90s_avx2_dec_measure(shared_secret, ciphertext, secret_key, rdtsc_buffer, rdtsc_labels, fail);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) pqcrystals_kyber1024_90s_ref_dec_measure(shared_secret, ciphertext, secret_key, rdtsc_buffer, rdtsc_labels, fail);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) pqcrystals_kyber1024_90s_ref_dec_measure(shared_secret, ciphertext, secret_key, rdtsc_buffer, rdtsc_labels, fail);
#endif
}

#endif
