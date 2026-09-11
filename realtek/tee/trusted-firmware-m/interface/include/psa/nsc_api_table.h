#ifndef __NSC_API_TABLE_H__
#define __NSC_API_TABLE_H__

#include <stddef.h>
#include <stdint.h>

#define NSC_API_TABLE_ADDR ((uintptr_t)0x0001F000UL)
#define NSC_API_TABLE_MAGIC 0x4E534341 /* "NSCA" */
#define NSC_API_TABLE_VERSION 0x1

typedef enum {
	SHA_224 = 0,
	SHA_256,
	SHA_384,
	SHA_512,
	SHA1,
	SHA3_224,
	SHA3_256,
	SHA3_384,
	SHA3_512,
	SHA_MAX_NUM,
} SHA_TYPE_t;

typedef enum {
	DP_NONE = 0,
	SECP192R1,
	SECP224R1,
	SECP256R1,
	SECP384R1,
	SECP521R1,
	BP256R1,
	BP384R1,
	BP512R1,
	CURVE25519,
	SECP192K1,
	SECP224K1,
	SECP256K1,
	CURVE448,
	ECDSA_CURVE_MAX_NUM,
} ECDSA_CURVE_TYPE_t;

typedef enum {
	MLDSA44 = 2,
	MLDSA65 = 3,
	MLDSA87 = 5,
} MLDSA_MODE_TYPE_t;

typedef enum {
	KYBER512 = 2,
	KYBER768 = 3,
	KYBER1024 = 4,
} KYBER_MODE_TYPE_t;

typedef int (*nsc_crypto_init_fn_t)(uint32_t heap_ptr, uint8_t *mbedtls_mem_buf,
				    size_t mbedtls_mem_buf_len);

typedef void (*nsc_crypto_deinit_fn_t)(void);

typedef int32_t (*nsc_crypto_rng_fn_t)(void *p_rng, unsigned char *output, size_t output_len);

typedef int32_t (*nsc_sha_init_fn_t)(SHA_TYPE_t type, void *sha_ctx);

typedef void (*nsc_sha_free_fn_t)(SHA_TYPE_t type, void *sha_ctx);

typedef void (*nsc_sha_clone_fn_t)(SHA_TYPE_t type, void *dst, const void *src);

typedef int32_t (*nsc_sha_starts_fn_t)(SHA_TYPE_t type, void *sha_ctx);

typedef int32_t (*nsc_sha_update_fn_t)(SHA_TYPE_t type, const uint8_t *data, size_t len,
				       void *sha_ctx);

typedef int32_t (*nsc_sha_finish_fn_t)(SHA_TYPE_t type, uint8_t *hash, void *sha_ctx);

typedef int32_t (*nsc_sha_fn_t)(SHA_TYPE_t type, const uint8_t *data, size_t len, uint8_t *hash);

typedef int32_t (*nsc_hmac_fn_t)(SHA_TYPE_t type, const uint8_t *key, size_t key_len,
				 const uint8_t *data, size_t len, uint8_t *hmac_hash);

typedef int32_t (*nsc_ecdsa_genkey_fn_t)(ECDSA_CURVE_TYPE_t curve, uint8_t *pk, uint8_t *sk,
					 uint8_t *seed, size_t seed_len);

typedef int32_t (*nsc_ecdsa_sign_fn_t)(ECDSA_CURVE_TYPE_t curve, const uint8_t *sk,
				       const uint8_t *hash, size_t hash_len, uint8_t *sig,
				       size_t *sig_len, size_t sig_size);

typedef int32_t (*nsc_ecdsa_sign_raw_fn_t)(ECDSA_CURVE_TYPE_t curve, const uint8_t *sk,
					   const uint8_t *hash, size_t hash_len, uint8_t *r_buf,
					   uint8_t *s_buf);

typedef int32_t (*nsc_ecdsa_verify_fn_t)(ECDSA_CURVE_TYPE_t curve, const uint8_t *pk,
					 const uint8_t *hash, size_t hash_len, const uint8_t *sig,
					 size_t sig_len);

typedef int32_t (*nsc_ecdsa_verify_raw_fn_t)(ECDSA_CURVE_TYPE_t curve, const uint8_t *pk,
					     const uint8_t *hash, size_t hash_len,
					     const uint8_t *sig);

typedef int32_t (*nsc_ecdh_gen_shared_secret_fn_t)(ECDSA_CURVE_TYPE_t curve, const uint8_t *pk,
						   const uint8_t *sk, uint8_t *ss);

typedef int32_t (*nsc_aes_gcm_encrypt_fn_t)(uint8_t *key, size_t key_len, uint8_t *iv,
					    size_t iv_len, uint8_t *add, size_t add_len,
					    uint8_t *input, size_t ilen, uint8_t *output,
					    uint8_t *tag, size_t tag_len);

typedef int32_t (*nsc_aes_gcm_decrypt_fn_t)(uint8_t *key, size_t key_len, uint8_t *iv,
					    size_t iv_len, uint8_t *add, size_t add_len,
					    uint8_t *input, size_t ilen, uint8_t *output,
					    uint8_t *tag, size_t tag_len);

typedef int (*nsc_mldsa_genkey_fn_t)(MLDSA_MODE_TYPE_t mode, uint8_t *pk, uint8_t *sk,
				     uint8_t *seed);

typedef int (*nsc_mldsa_sign_fn_t)(MLDSA_MODE_TYPE_t mode, uint8_t *sig, size_t *siglen,
				   const uint8_t *msg, size_t msg_len, const uint8_t *ctx,
				   uint8_t ctx_len, const uint8_t *sk, const uint8_t *rnd);

typedef int (*nsc_mldsa_verify_fn_t)(MLDSA_MODE_TYPE_t mode, uint8_t *sig, size_t siglen,
				     const uint8_t *msg, size_t msg_len, const uint8_t *ctx,
				     uint8_t ctx_len, const uint8_t *pk);

typedef int (*nsc_kyber_genkey_fn_t)(KYBER_MODE_TYPE_t mode, uint8_t *pk, uint8_t *sk,
				     const uint8_t *coins);

typedef int (*nsc_kyber_kem_enc_fn_t)(KYBER_MODE_TYPE_t mode, uint8_t *ct, uint8_t *ss,
				      const uint8_t *pk, const uint8_t *coins);

typedef int (*nsc_kyber_kem_dec_fn_t)(KYBER_MODE_TYPE_t mode, uint8_t *ss, const uint8_t *ct,
				      const uint8_t *sk);

typedef uint32_t (*otp_read_idx_fn_t)(uint32_t idx);

typedef uint32_t (*otp_write_idx_fn_t)(uint32_t idx, uint32_t value);

typedef void (*enable_secure_boot_fn_t)(void);

typedef int8_t (*check_secure_boot_status_fn_t)(void);

typedef uint32_t (*puf_read_UID_fn_t)(uint32_t idx);

typedef struct {
	uint32_t magic;
	uint16_t version;
	uint16_t table_size;

	/* Crypto APIs */
	nsc_crypto_init_fn_t crypto_init;
	nsc_crypto_deinit_fn_t crypto_deinit;
	nsc_crypto_rng_fn_t crypto_rng;

	/* SHA */
	nsc_sha_init_fn_t sha_init;
	nsc_sha_free_fn_t sha_free;
	nsc_sha_clone_fn_t sha_clone;
	nsc_sha_starts_fn_t sha_starts;
	nsc_sha_update_fn_t sha_update;
	nsc_sha_finish_fn_t sha_finish;
	nsc_sha_fn_t sha;

	/* HMAC */
	nsc_hmac_fn_t hmac;

	/* ECDSA */
	nsc_ecdsa_genkey_fn_t ecdsa_genkey;
	nsc_ecdsa_sign_fn_t ecdsa_sign;
	nsc_ecdsa_sign_raw_fn_t ecdsa_sign_raw;
	nsc_ecdsa_verify_fn_t ecdsa_verify;
	nsc_ecdsa_verify_raw_fn_t ecdsa_verify_raw;

	/* ECDH */
	nsc_ecdh_gen_shared_secret_fn_t ecdh_gen_shared_secret;

	/* AES-GCM */
	nsc_aes_gcm_encrypt_fn_t aes_gcm_encrypt;
	nsc_aes_gcm_decrypt_fn_t aes_gcm_decrypt;

	/* ML-DSA */
	nsc_mldsa_genkey_fn_t mldsa_genkey;
	nsc_mldsa_sign_fn_t mldsa_sign;
	nsc_mldsa_verify_fn_t mldsa_verify;

	/* ML-KEM / Kyber */
	nsc_kyber_genkey_fn_t kyber_genkey;
	nsc_kyber_kem_enc_fn_t kyber_kem_enc;
	nsc_kyber_kem_dec_fn_t kyber_kem_dec;

	/* OTP */
	otp_read_idx_fn_t otp_read_idx;
	otp_write_idx_fn_t otp_write_idx;
	enable_secure_boot_fn_t enable_secure_boot;
	check_secure_boot_status_fn_t check_secure_boot_status;
	puf_read_UID_fn_t puf_read_UID;

} NSC_API_FnTbl_t;

#endif /* __NSC_API_TABLE_H__ */
