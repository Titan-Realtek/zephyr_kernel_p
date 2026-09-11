#ifndef __RTK_CRYPTO_H__
#define __RTK_CRYPTO_H__

#include <psa/crypto.h>

psa_status_t rtk_psa_crypto_init(void);
psa_status_t rtk_psa_hash_abort(psa_algorithm_t alg, uint8_t *sha_ctx);
psa_status_t rtk_psa_hash_setup(psa_algorithm_t alg, uint8_t *sha_ctx);
psa_status_t rtk_psa_hash_update(psa_algorithm_t alg, const uint8_t *input, size_t input_len,
				 uint8_t *sha_ctx);
psa_status_t rtk_psa_hash_finish(psa_algorithm_t alg, uint8_t *hash, uint8_t *sha_ctx);
psa_status_t rtk_psa_hash_compute(psa_algorithm_t alg, const uint8_t *input, size_t input_len,
				  uint8_t *hash);
psa_status_t rtk_psa_mac_compute(psa_algorithm_t alg, const uint8_t *key, size_t key_len,
				 const uint8_t *input, size_t input_len, uint8_t *hmac_hash);
psa_status_t rtk_psa_ecdsa_genkey(psa_algorithm_t alg, uint8_t *pk, uint8_t *sk, uint8_t *seed,
				  size_t seed_len);
psa_status_t rtk_psa_ecdsa_sign(psa_algorithm_t alg, const uint8_t *sk, const uint8_t *hash,
				size_t hash_len, uint8_t *sig, size_t *sig_len, size_t sig_size);
psa_status_t rtk_psa_ecdsa_verify(psa_algorithm_t alg, const uint8_t *pk, const uint8_t *hash,
				  size_t hash_len, const uint8_t *sig, size_t sig_len);
psa_status_t rtk_psa_ecdsa_sign_raw(psa_algorithm_t alg, const uint8_t *sk, const uint8_t *hash,
				    size_t hash_len, uint8_t *r_buf, uint8_t *s_buf);
psa_status_t rtk_psa_ecdsa_verify_raw(psa_algorithm_t alg, const uint8_t *pk, const uint8_t *hash,
				      size_t hash_len, const uint8_t *sig);
psa_status_t rtk_psa_ecdh_gen_shared_secret(psa_algorithm_t alg, const uint8_t *pk,
					    const uint8_t *sk, uint8_t *ss);
psa_status_t rtk_psa_aes_gcm_encrypt(uint8_t *key, size_t key_len, uint8_t *iv, size_t iv_len,
				     uint8_t *add, size_t add_len, uint8_t *input, size_t ilen,
				     uint8_t *output, uint8_t *tag, size_t tag_len);
psa_status_t rtk_psa_aes_gcm_decrypt(uint8_t *key, size_t key_len, uint8_t *iv, size_t iv_len,
				     uint8_t *add, size_t add_len, uint8_t *input, size_t ilen,
				     uint8_t *output, uint8_t *tag, size_t tag_len);
psa_status_t rtk_psa_mldsa_genkey(psa_algorithm_t alg, uint8_t *pk, uint8_t *sk, uint8_t *seed);
psa_status_t rtk_psa_mldsa_sign(psa_algorithm_t alg, uint8_t *sig, const uint8_t *msg,
				size_t msg_len, const uint8_t *ctx, uint8_t ctx_len,
				const uint8_t *sk, const uint8_t *rnd);
psa_status_t rtk_psa_mldsa_verify(psa_algorithm_t alg, uint8_t *sig, const uint8_t *msg,
				  size_t msg_len, const uint8_t *ctx, uint8_t ctx_len,
				  const uint8_t *pk);
psa_status_t rtk_psa_kyber_genkey(psa_algorithm_t alg, uint8_t *pk, uint8_t *sk,
				  const uint8_t *coins);
psa_status_t rtk_psa_kyber_kem_enc(psa_algorithm_t alg, uint8_t *ct, uint8_t *ss, const uint8_t *pk,
				   const uint8_t *coins);
psa_status_t rtk_psa_kyber_kem_dec(psa_algorithm_t alg, uint8_t *ss, const uint8_t *ct,
				   const uint8_t *sk);
#endif /* __RTK_CRYPTO_H__ */