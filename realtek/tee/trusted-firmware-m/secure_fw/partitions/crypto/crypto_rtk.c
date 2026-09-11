/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stddef.h>
#include <stdint.h>

#include "config_tfm.h"
#include "tfm_mbedcrypto_include.h"

#include "tfm_crypto_api.h"
#include "tfm_crypto_defs.h"

#include <psa/rtk_psa_crypto.h>

/*!
 * \addtogroup tfm_crypto_api_shim_layer
 *
 */
psa_status_t tfm_crypto_rtk_interface(psa_invec in_vec[], psa_outvec out_vec[])
{
	const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;
	psa_status_t status = PSA_ERROR_NOT_SUPPORTED;
	enum tfm_crypto_func_sid_t sid = iov->function_id;

	switch (sid) {
	case TFM_CRYPTO_RTK_CRYPTO_INIT_SID: {
		status = rtk_psa_crypto_init();

		return status;
	}
	case TFM_CRYPTO_RTK_HASH_ABORT_SID: {
		const struct rtk_hash_compute_params *params = (const struct rtk_hash_compute_params *)in_vec[1].base;
		uint8_t *sha_ctx = params->sha_ctx;

		status = rtk_psa_hash_abort(iov->alg, sha_ctx);
		break;
	}
	case TFM_CRYPTO_RTK_HASH_SETUP_SID: {
		const struct rtk_hash_compute_params *params = (const struct rtk_hash_compute_params *)in_vec[1].base;
		uint8_t *sha_ctx = params->sha_ctx;

		status = rtk_psa_hash_setup(iov->alg, sha_ctx);
		break;
	}
	case TFM_CRYPTO_RTK_HASH_UPDATE_SID: {
		const struct rtk_hash_compute_params *params = (const struct rtk_hash_compute_params *)in_vec[1].base;
		const uint8_t *input = params->input;
		size_t input_len = params->input_len;
		uint8_t *sha_ctx = params->sha_ctx;

		status = rtk_psa_hash_update(iov->alg, input, input_len, sha_ctx);
		break;
	}
	case TFM_CRYPTO_RTK_HASH_FINISH_SID: {
		const struct rtk_hash_compute_params *params = (const struct rtk_hash_compute_params *)in_vec[1].base;
		uint8_t *sha_ctx = params->sha_ctx;
		uint8_t *hash = params->hash;

		status = rtk_psa_hash_finish(iov->alg, hash, sha_ctx);
		break;
	}
	case TFM_CRYPTO_RTK_HASH_COMPUTE_SID: {
		const struct rtk_hash_compute_params *params = (const struct rtk_hash_compute_params *)in_vec[1].base;
		const uint8_t *input = params->input;
		size_t input_len = params->input_len;
		uint8_t *hash = params->hash;

		status = rtk_psa_hash_compute(iov->alg, input, input_len, hash);
		break;
	}
	case TFM_CRYPTO_RTK_MAC_COMPUTE_SID: {
		const struct rtk_mac_compute_params *params = (const struct rtk_mac_compute_params *)in_vec[1].base;
		const uint8_t *key = params->key;
        size_t key_len = params->key_len;
		const uint8_t *input = params->input;
        size_t input_len = params->input_len;
        uint8_t *hmac_hash = params->hmac_hash;

		status = rtk_psa_mac_compute(iov->alg, key, key_len, input, input_len, hmac_hash);
        break;
	}
	case TFM_CRYPTO_RTK_ECDSA_GENKEY_SID: {
		const struct rtk_ecdsa_genkey_params *params = (const struct rtk_ecdsa_genkey_params *)in_vec[1].base;
		uint8_t *pk = params->pk;
		uint8_t *sk = params->sk;
		uint8_t *seed = params->seed;
		size_t seed_len = params->seed_len;

		status = rtk_psa_ecdsa_genkey(iov->alg, pk, sk, seed, seed_len);
		break;
	}
	case TFM_CRYPTO_RTK_ECDSA_SIGN_SID: {
		const struct rtk_ecdsa_sign_params *params = (const struct rtk_ecdsa_sign_params *)in_vec[1].base;
		const uint8_t *sk = params->sk;
		const uint8_t *hash = params->hash;
		size_t hash_len = params->hash_len;
		uint8_t *sig = out_vec[0].base;
		size_t sig_size = out_vec[0].len;

		status = rtk_psa_ecdsa_sign(iov->alg, sk, hash, hash_len, sig, &out_vec[0].len, sig_size);
		if (status != PSA_SUCCESS) {
			out_vec[0].len = 0;
		}
	
		break;
	}
	case TFM_CRYPTO_RTK_ECDSA_VERIFY_SID: {
		const struct rtk_ecdsa_verify_params *params = (const struct rtk_ecdsa_verify_params *)in_vec[1].base;
		const uint8_t *pk = params->pk;
		const uint8_t *hash = params->hash;
		size_t hash_len = params->hash_len;
		const uint8_t *sig = params->sig;
		size_t sig_len = params->sig_len;

		status = rtk_psa_ecdsa_verify(iov->alg, pk, hash, hash_len, sig, sig_len);
		break;
	}
	case TFM_CRYPTO_RTK_ECDSA_SIGN_RAW_SID: {
		const struct rtk_ecdsa_sign_params *params = (const struct rtk_ecdsa_sign_params *)in_vec[1].base;
		const uint8_t *sk = params->sk;
		const uint8_t *hash = params->hash;
		size_t hash_len = params->hash_len;
		uint8_t *r_buf = params->r_buf;
		uint8_t *s_buf = params->s_buf;

		status = rtk_psa_ecdsa_sign_raw(iov->alg, sk, hash, hash_len, r_buf, s_buf);
		break;
	}
	case TFM_CRYPTO_RTK_ECDSA_VERIFY_RAW_SID: {
		const struct rtk_ecdsa_verify_params *params = (const struct rtk_ecdsa_verify_params *)in_vec[1].base;
		const uint8_t *pk = params->pk;
		const uint8_t *hash = params->hash;
		size_t hash_len = params->hash_len;
		const uint8_t *sig = params->sig;

		status = rtk_psa_ecdsa_verify_raw(iov->alg, pk, hash, hash_len, sig);
		break;
	}
	case TFM_CRYPTO_RTK_ECDH_GEN_SHARED_SECRET_SID: {
		const struct rtk_ecdh_gen_shared_secret_params *params = (const struct rtk_ecdh_gen_shared_secret_params *)in_vec[1].base;
		const uint8_t *pk = params->pk;
		const uint8_t *sk = params->sk;
		uint8_t *ss = params->ss;

		status = rtk_psa_ecdh_gen_shared_secret(iov->alg, pk, sk, ss);
		break;
	}
	case TFM_CRYPTO_RTK_AES_GCM_ENCRYPT_SID: {
		const struct rtk_aes_gcm_enc_params *params = (const struct rtk_aes_gcm_enc_params *)in_vec[1].base;
		uint8_t *key = params->key;
		size_t key_len = params->key_len;
		uint8_t *iv = params->iv;
		size_t iv_len = params->iv_len;
		uint8_t *add = params->add;
		size_t add_len = params->add_len;
		uint8_t *input = params->input;
		size_t input_len = params->input_len;
		uint8_t *output = params->output;
		uint8_t *tag = params->tag;
		size_t tag_len = params->tag_len;

		status = rtk_psa_aes_gcm_encrypt(key, key_len, iv, iv_len, add, add_len, input, input_len, output, tag, tag_len);
		break;
	}
	case TFM_CRYPTO_RTK_AES_GCM_DECRYPT_SID: {
		const struct rtk_aes_gcm_dec_params *params = (const struct rtk_aes_gcm_dec_params *)in_vec[1].base;
		uint8_t *key = params->key;
		size_t key_len = params->key_len;
		uint8_t *iv = params->iv;
		size_t iv_len = params->iv_len;
		uint8_t *add = params->add;
		size_t add_len = params->add_len;
		uint8_t *input = params->input;
		size_t input_len = params->input_len;
		uint8_t *tag = params->tag;
		size_t tag_len = params->tag_len;
		uint8_t *output = params->output;

		status = rtk_psa_aes_gcm_decrypt(key, key_len, iv, iv_len, add, add_len, input, input_len, output, tag, tag_len);
		break;
	}
	case TFM_CRYPTO_RTK_MLDSA_GENKEY_SID: {
		const struct rtk_mldsa_genkey_params *params = (const struct rtk_mldsa_genkey_params *)in_vec[1].base;
		uint8_t *pk = params->pk;
		uint8_t *sk = params->sk;
		uint8_t *seed = params->seed;

		status = rtk_psa_mldsa_genkey(iov->alg, pk, sk, seed);

		break;
	}
	case TFM_CRYPTO_RTK_MLDSA_SIGN_SID: {
		const struct rtk_mldsa_sign_params *params = (const struct rtk_mldsa_sign_params *)in_vec[1].base;
		const uint8_t *msg = params->msg;
		size_t msg_len = params->msg_len;
		const uint8_t *ctx = params->ctx;
		size_t ctx_len = params->ctx_len;
		const uint8_t *sk = params->sk;
		const uint8_t *rnd = params->rnd;
		uint8_t *sig = params->sig;

		status = rtk_psa_mldsa_sign(iov->alg, sig, msg, msg_len, ctx, ctx_len, sk, rnd);

		break;
	}
	case TFM_CRYPTO_RTK_MLDSA_VERIFY_SID: {
		const struct rtk_mldsa_verify_params *params = (const struct rtk_mldsa_verify_params *)in_vec[1].base;
		uint8_t *sig = params->sig;
		const uint8_t *msg = params->msg;
		size_t msg_len = params->msg_len;
		const uint8_t *ctx = params->ctx;
		size_t ctx_len = params->ctx_len;
		const uint8_t *pk = params->pk;

		status = rtk_psa_mldsa_verify(iov->alg, sig, msg, msg_len, ctx, ctx_len, pk);
		break;
	}
	case TFM_CRYPTO_RTK_KYBER_GENKEY_SID: {
		const struct rtk_kyber_genkey_params *params = (const struct rtk_kyber_genkey_params *)in_vec[1].base;
		uint8_t *pk = params->pk;
		uint8_t *sk = params->sk;
		const uint8_t *coins = params->coins;

		status = rtk_psa_kyber_genkey(iov->alg, pk, sk, coins);
		break;
	}
	case TFM_CRYPTO_RTK_KYBER_ENCRYPT_SID: {
		const struct rtk_kyber_kem_enc_params *params = (const struct rtk_kyber_kem_enc_params *)in_vec[1].base;
		const uint8_t *pk = params->pk;
		const uint8_t *coins = params->coins;
		uint8_t *ct = params->ct;
		uint8_t *ss = params->ss;

		status = rtk_psa_kyber_kem_enc(iov->alg, ct, ss, pk, coins);
		break;
	}
	case TFM_CRYPTO_RTK_KYBER_DECRYPT_SID: {
		const struct rtk_kyber_kem_dec_params *params = (const struct rtk_kyber_kem_dec_params *)in_vec[1].base;
		const uint8_t *ct = params->ct;
		const uint8_t *sk = params->sk;
		uint8_t *ss = params->ss;

		status = rtk_psa_kyber_kem_dec(iov->alg, ss, ct, sk);
		break;
	}
	default:
		return PSA_ERROR_NOT_SUPPORTED;
	}

	return status;
}
