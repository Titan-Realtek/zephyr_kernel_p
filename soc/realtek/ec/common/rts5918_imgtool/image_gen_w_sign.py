# SPDX-License-Identifier: Apache-2.0
#
# Copyright (c) 2024 Realtek Semiconductor Corporation, SIBG-SD7
# Author: Dylan Hsieh <dylan.hsieh@realtek.com>

"""
The RTS5918 specific image header shows the bootROM how to
load the image from flash to internal SRAM, this script obtains
the header to original BIN and output a new BIN file.
"""

import argparse
import ctypes
import ecdsa
import hashlib
import os
import platform
import pyhsslms
import shutil
import struct
import sys

from ecdsa import SigningKey, NIST384p
from ecdsa.util import sigencode_string, sigencode_der

IMAGE_MAGIC = 0x524C544B  # ASCII 'RLTK'
IMAGE_HDR_SIZE = 32
FREQ_50M = 0
FREQ_25M = 1
FREQ_12P5M = 2
FREQ_6P25M = 3
NORMAL_read = "0x03"
DUAL_read = "0x3B"
QUAD_read = "0x6B"

TLV_MAGIC = 0x6907
TLV_IMAGE_HASH_SHA2_256 = 0x0010
TLV_IMAGE_HASH_SHA2_384 = 0x00A0
TLV_IMAGE_HASH_SHA3_384 = 0x00A1
TLV_IMAGE_HASH_SHA3_512 = 0x00A2
TLV_PUBLIC_KEY0_HASH_SHA2_384 = 0x1000
TLV_PUBLIC_KEY1_HASH_SHA2_384 = 0x1010
TLV_PUBLIC_KEY0_HASH_SHA3_384 = 0x1020
TLV_PUBLIC_KEY1_HASH_SHA3_384 = 0x1030
TLV_SIGNATURE_LMS = 0x00F0
TLV_SIGNATURE_ML_DSA_87 = 0x00F1
TLV_SIGNATURE_ECDSA = 0x0022

TLV_IMAGE_HASH_LEN = 0
PUBLIC_KEY_LEN = 0
SECRET_KEY_LEN = 0
SIGNATURE_KEY_LEN = 0

RED_TEXT_COLOR = "\033[31m"
RESET_TEXT_COLOR = "\033[0m"

def keyidx_range_check(value):
    """
    Check if the key inex is between 0 and 15
    """
    try:
        ivalue = int(value)
    except ValueError:
        error_message = (f"{RED_TEXT_COLOR}'{value}' is not an integer")
        error_message += (f"{RESET_TEXT_COLOR}")
        raise argparse.ArgumentTypeError(error_message)

    if not 0 <= ivalue <= 15:
        error_message = (f"{RED_TEXT_COLOR}Key index '{ivalue}' exceeds range [0-15]")
        error_message += (f"{RESET_TEXT_COLOR}")
        raise argparse.ArgumentTypeError(error_message)

    return ivalue

def revision_range_check(value):
    """
    Check if the image revision is between 0 and 31
    """
    try:
        ivalue = int(value)
    except ValueError:
        error_message = (f"{RED_TEXT_COLOR}'{value}' is not an integer")
        error_message += (f"{RESET_TEXT_COLOR}")
        raise argparse.ArgumentTypeError(error_message)

    if not 0 <= ivalue <= 31:
        error_message = (f"{RED_TEXT_COLOR}Key index '{ivalue}' exceeds range [0-31]")
        error_message += (f"{RESET_TEXT_COLOR}")
        raise argparse.ArgumentTypeError(error_message)

    return ivalue

def parse_args():
    """
    Parsing the arguments
    """
    parser = argparse.ArgumentParser(allow_abbrev=False)

    parser.add_argument(
        "-I",
        "--input",
        type=str,
        dest="original_bin",
        required=True,
        help="Input bin file path",
    )

    parser.add_argument(
        "-O",
        "--output",
        type=str,
        dest="signed_bin",
        required=True,
        help="Output bin file path",
    )

    parser.add_argument(
        "-F",
        "--spi-freq",
        type=int,
        dest="spi_freq",
        choices=[FREQ_50M, FREQ_25M, FREQ_12P5M, FREQ_6P25M],
        default=FREQ_6P25M,
        help="Specify the frequency of SPI I/F.",
    )

    parser.add_argument(
        "-R",
        "--spi-rdcmd",
        type=str,
        dest="spi_rdcmd",
        choices=[NORMAL_read, DUAL_read, QUAD_read],
        default=NORMAL_read,
        help="Specify the command for flash read.",
    )

    parser.add_argument(
        "-i",
        "--key-index",
        type=keyidx_range_check,
        dest="key_index",
        default="0",
        help="Set the revision of image.",
    )

    parser.add_argument(
        "-r",
        "--revision",
        type=revision_range_check,
        dest="revision",
        default="0",
        help="Set the revision of image.",
    )

    parser.add_argument(
        "-L",
        "--load-addr",
        type=lambda x: int(x, 0),
        dest="load_addr",
        required=True,
        help="Load address for image when it should run from RAM.",
    )

    parser.add_argument(
        "-H",
        "--image-hash-alg",
        type=str,
        dest="image_hash_alg",
        default="SHA2_384",
        choices=["SHA2_256", "SHA2_384", "SHA3_384", "SHA3_512"],
        help="Select algorithm for image hash.",
    )

    parser.add_argument(
        "-p",
        "--public-key",
        type=str,
        dest="public_key_bin",
        nargs='+',
        default=None,
        help="Public Key",
    )

    parser.add_argument(
        "-s",
        "--secret-key",
        type=str,
        dest="secret_key_bin",
        default=None,
        help="Secret Key",
    )

    parser.add_argument(
        "-k",
        "--key-hash-alg",
        type=str,
        dest="key_hash_alg",
        default=None,
        choices=[None, "SHA2_384", "SHA3_384"],
        help="Select algorithm for public key hash.",
    )

    parser.add_argument(
        "-S",
        "--signature-algorithm",
        type=str,
        dest="signature_algorithm",
        default=None,
        choices=[None, "LMS", "MLDSA", "ECDSA"],
        help="Select signature encryption algorithm.",
    )

    parser.add_argument(
        "-D",
        "--dual-sign",
        action="store_true",
        dest="dual_sign",
        help="Dual Signature Enable",
    )

    parser.add_argument(
        "-DH",
        "--dual-image-hash-alg",
        type=str,
        dest="dual_image_hash_alg",
        default="SHA2_384",
        choices=["SHA2_384", "SHA3_384"],
        help="Select algorithm for image hash.",
    )

    parser.add_argument(
        "-Dp",
        "--dual-public-key",
        type=str,
        dest="dual_public_key_bin",
        nargs='+',
        default=None,
        help="Dual Public Key",
    )

    parser.add_argument(
        "-Ds",
        "--dual-secret-key",
        type=str,
        dest="dual_secret_key_bin",
        default=None,
        help="Dual Secret Key",
    )

    parser.add_argument(
        "-Dk",
        "--dual_key-hash-alg",
        type=str,
        dest="dual_key_hash_alg",
        default="SHA2_384",
        choices=["SHA2_384", "SHA3_384"],
        help="Select algorithm for public key hash.",
    )

    parser.add_argument("-V", "--verbose", action="count", default=0, help="Verbose Output")

    ret_args = parser.parse_args()
    return ret_args

def check_valid_argument(
    load_addr, spi_freq, spi_rdcmd, key_index, revision, original_bin, signed_bin, image_hash_alg,
    public_key_bin, secret_key_bin, key_hash_alg, signature_algorithm, dual_sign,
    dual_image_hash_alg, dual_public_key_bin, dual_secret_key_bin, dual_key_hash_alg
):

    if((key_hash_alg is None) and (signature_algorithm is not None)):
        error_message = (f"{RED_TEXT_COLOR}Error: signature_algorithm is '"'None'"', key_hash_alg must be one of '"'SHA2-384'"', '"'SHA3-384'"'")
        error_message += (f"{RESET_TEXT_COLOR}")
        sys.exit(error_message)

    if((key_hash_alg is not None) and (signature_algorithm is None)):
        error_message = (f"{RED_TEXT_COLOR}Error: key_hash_alg is '"'None'"', signature_algorithm must be one of '"'LMS'"', '"'MLDSA'"', '"'ECDSA'"'")
        error_message += (f"{RESET_TEXT_COLOR}")
        sys.exit(error_message)


    if(signature_algorithm == "LMS"):
        if(image_hash_alg != "SHA2_384" and image_hash_alg != "SHA3_384"):
            error_message = (f"{RED_TEXT_COLOR}Error: signature_algorithm is '"'LMS'"', image_hash_alg should be '"'SHA2_384 or SHA3_384'"'")
            error_message += (f"{RESET_TEXT_COLOR}")
            sys.exit(error_message)

    if(signature_algorithm == "MLDSA"):
        if(image_hash_alg != "SHA3_512"):
            error_message = (f"{RED_TEXT_COLOR}Error: signature_algorithm is '"'MLDSA'"', image_hash_alg should be '"'SHA3_512'"'")
            error_message += (f"{RESET_TEXT_COLOR}")
            sys.exit(error_message)

    if(signature_algorithm == "ECDSA"):
        if(image_hash_alg != "SHA2_384" and image_hash_alg != "SHA3_384"):
            error_message = (f"{RED_TEXT_COLOR}Error: signature_algorithm is '"'ECDSA'"', image_hash_alg should be '"'SHA2_384 or SHA3_384'"'")
            error_message += (f"{RESET_TEXT_COLOR}")
            sys.exit(error_message)

    if(dual_sign is True):
        if((public_key_bin or secret_key_bin or signature_algorithm) is None):
            error_message = (f"{RED_TEXT_COLOR}Error: dual sign is enabled, public_key & secret_key & signature_alrorithm cannot be '"'None'"'")
            error_message += (f"{RESET_TEXT_COLOR}")
            sys.exit(error_message)
        if((dual_public_key_bin or dual_secret_key_bin) is None):
            error_message = (f"{RED_TEXT_COLOR}Error: dual sign is enabled, dual_public_key & dual_secret_key cannot be '"'None'"'")
            error_message += (f"{RESET_TEXT_COLOR}")
            sys.exit(error_message)

def generate_image_header(original_bin, load_addr, spi_freq, spi_rdcmd, key_index, revision, image_hash_alg, signature_algorithm):

    hdr_payload = bytearray(0)
    img_size = os.path.getsize(original_bin)
    hdr_fmt = (
        "<"
        +
        # type ImageHdr struct {
        "I"  # Magic    uint32
        + "I"  # LoadAddr uint32
        + "I"  # HdrSz    uint32
        + "I"  # ImgSz    uint32
        + "I"  # JumpAddr uint32
        + "I"  # Flags    uint32
        + "I"  # reserved uint32
        + "B"  # SpiFmt   uint8
        + "B"  # SpiRdCmd uint8
        + "B"  # KeyIdx   uint8
        + "B"  # Revision uint8
    )  # }

    hdr = struct.pack(
        hdr_fmt,
        IMAGE_MAGIC,
        load_addr,
        IMAGE_HDR_SIZE,
        img_size,
        load_addr + IMAGE_HDR_SIZE,
        0,
        0,
        0 + (int(spi_freq) << 2),
        int(spi_rdcmd, 0),
        key_index,
        revision,
    )

    hdr_payload[: len(hdr)] = hdr

    with open("hdr.bin", "wb") as outfile:
        outfile.write(hdr_payload)
        outfile.flush()
        outfile.close()

def calculate_image_hash(original_bin, image_hash_alg):

    if(image_hash_alg == "SHA2_384"):
        sha = hashlib.sha384()
    elif(image_hash_alg == "SHA3_384"):
        sha = hashlib.sha3_384()
    elif(image_hash_alg == "SHA3_512"):
        sha = hashlib.sha3_512()
    elif(image_hash_alg == "SHA2_256"):
        sha = hashlib.sha256()
    else:
        error_message = (f"{RED_TEXT_COLOR}Error image hash algorithm")
        error_message += (f"{RESET_TEXT_COLOR}")
        sys.exit(error_message)

    # Compute Image Hash
    with open("hdr.bin", "rb") as hdr, open(original_bin, "rb") as original, open("hash.bin", "wb") as digest:
        sha.update(hdr.read())
        sha.update(original.read())
        image_hash = sha.digest()
        digest.write(image_hash)
        digest.flush()
        hdr.close()
        original.close()
        digest.close()

def calculate_dual_image_hash(original_bin, dual_image_hash_alg, dual_sign):

    if(dual_sign is False):
        return

    if(dual_image_hash_alg == "SHA2_384"):
        sha = hashlib.sha384()
    elif(dual_image_hash_alg == "SHA3_384"):
        sha = hashlib.sha3_384()
    elif(dual_image_hash_alg == "SHA3_512"):
        sha = hashlib.sha3_512()
    elif(dual_image_hash_alg == "SHA2_256"):
        sha = hashlib.sha256()
    else:
        error_message = (f"{RED_TEXT_COLOR}Error image hash algorithm")
        error_message += (f"{RESET_TEXT_COLOR}")
        sys.exit(error_message)

    # Compute Image Hash
    with open("hdr.bin", "rb") as hdr, open(original_bin, "rb") as original, open("hash2.bin", "wb") as digest:
        sha.update(hdr.read())
        sha.update(original.read())
        image_hash = sha.digest()
        digest.write(image_hash)
        digest.flush()
        hdr.close()
        original.close()
        digest.close()

def calculate_signature(key_index, public_key_bin, secret_key_bin, signature_algorithm):
    if(public_key_bin or secret_key_bin or signature_algorithm) is None:
        return

    if(signature_algorithm == "LMS"):
        PUBLIC_KEY_LEN = 60
        SECRET_KEY_LEN = 72
        with open("hash.bin", "rb") as digest:
            image_hash = digest.read()
            digest.close()

        ## Read Public Key
        with open((public_key_bin[key_index]), "rb") as public_key_file:
            public_key = public_key_file.read()
            public_key_file.close()

        if(len(public_key) != PUBLIC_KEY_LEN):
            error_message = (f"{RED_TEXT_COLOR}Error: public_key length is not {PUBLIC_KEY_LEN}: {len(public_key)}")
            error_message += (f"{RESET_TEXT_COLOR}")
            sys.exit(error_message)

        ## Read Secret Key
        with open((secret_key_bin), "rb") as secret_key_file:
            secret_key = secret_key_file.read()
            secret_key_file.close()

        if(len(secret_key) != SECRET_KEY_LEN):
            error_message = (f"{RED_TEXT_COLOR}Error: secret_key length is not {SECRET_KEY_LEN}: {len(secret_key)}")
            error_message += (f"{RESET_TEXT_COLOR}")
            sys.exit(error_message)

        shutil.copyfile(public_key_bin[key_index], "LMS.pub")
        shutil.copyfile(secret_key_bin, "LMS.prv")
        private_key = pyhsslms.HssLmsPrivateKey("LMS")
        signature = private_key.sign(image_hash)
        with open("signature.bin", "wb") as signature_file:
            signature_file.write(signature)
            signature_file.flush()
            signature_file.close()
        os.remove("LMS.pub")
        os.remove("LMS.prv")

    elif(signature_algorithm == "MLDSA"):
        PUBLIC_KEY_LEN = 2592
        SECRET_KEY_LEN = 4896
        with open("hash.bin", "rb") as digest:
            image_hash = digest.read()
            digest.close()

        if(len(image_hash) != 0x40):
            error_message = (f"{RED_TEXT_COLOR}Error: image_hash length is not {0x40}: {len(image_hash)}")
            error_message += (f"{RESET_TEXT_COLOR}")
            sys.exit(error_message)

        ## Read Public Key
        with open((public_key_bin[key_index]), "rb") as public_key_file:
            public_key = public_key_file.read()
            public_key_file.close()

        if(len(public_key) != PUBLIC_KEY_LEN):
            error_message = (f"{RED_TEXT_COLOR}Error: public_key length is not {PUBLIC_KEY_LEN}: {len(public_key)}")
            error_message += (f"{RESET_TEXT_COLOR}")
            sys.exit(error_message)

        ## Read Secret Key
        with open(secret_key_bin, "rb") as secret_key_file:
            secret_key = secret_key_file.read()
            secret_key_file.close()

        if(len(secret_key) != SECRET_KEY_LEN):
            error_message = (f"{RED_TEXT_COLOR}Error: secret_key length is not {SECRET_KEY_LEN}: {len(secret_key)}")
            error_message += (f"{RESET_TEXT_COLOR}")
            sys.exit(error_message)

        current_os = platform.system()

        if(current_os == "Linux"):
            ## Compute Signature
            # Load Dynamically Linked Shared Object Library
            lib_path = os.path.join(os.path.dirname(__file__), 'libpqcrystals_dilithium5_sign.so')

            # Use CDLL to Load Library
            try:
                libpqcrystals_dilithium5_sign = ctypes.CDLL(lib_path)
                print("Python: Successfully loaded libpqcrystals_dilithium5_sign.so")
            except OSError as e:
                error_message = (f"{RED_TEXT_COLOR}Python: Error loading .so file: {e}")
                error_message += (f"{RESET_TEXT_COLOR}")
                sys.exit(error_message)

            ## Set Function Prototype:
            ## int sign_signature(char* signature_file_name, const uint8_t *hash, size_t hash_len, const uint8_t *sk)
            # Set Arguments
            libpqcrystals_dilithium5_sign.sign_signature.argtypes = [
                ctypes.c_char_p,
                ctypes.POINTER(ctypes.c_uint8),
                ctypes.c_size_t
            ]
            # Set Return Type
            libpqcrystals_dilithium5_sign.sign_signature.restype = ctypes.c_int
            result = libpqcrystals_dilithium5_sign.sign_signature(
                "signature.bin".encode('utf-8'),
                (ctypes.c_uint8 * len(image_hash))(*image_hash),
                len(image_hash),
                (ctypes.c_uint8 * len(secret_key))(*secret_key),
            )
            if(result != 0):
                exit()
        elif(current_os == "Windows"):
            ## Compute Signature
            # Load Dynamic-Link Library
            dll_path = os.path.abspath("libpqcrystals_dilithium5_sign.dll")

            # Use CDLL to Load Library
            try:
                libpqcrystals_dilithium5_sign = ctypes.CDLL(dll_path)
                print("Python: Successfully loaded libpqcrystals_dilithium5_sign.dll")
            except OSError as e:
                error_message = (f"{RED_TEXT_COLOR}Python: Error loading .dll file: {e}")
                error_message += (f"{RESET_TEXT_COLOR}")
                sys.exit(error_message)

            ## Set Function Prototype:
            ## int sign_signature(char* signature_file_name, const uint8_t *hash, size_t hash_len, const uint8_t *sk)
            # Set Arguments
            libpqcrystals_dilithium5_sign.sign_signature.argtypes = [
                ctypes.c_char_p,
                ctypes.POINTER(ctypes.c_uint8),
                ctypes.c_size_t
            ]
            # Set Return Type
            libpqcrystals_dilithium5_sign.sign_signature.restype = ctypes.c_int
            result = libpqcrystals_dilithium5_sign.sign_signature(
                "signature.bin".encode('utf-8'),
                (ctypes.c_uint8 * len(image_hash))(*image_hash),
                len(image_hash),
                (ctypes.c_uint8 * len(secret_key))(*secret_key),
            )
            if(result != 0):
                exit()
        else:
            error_message = (f"{RED_TEXT_COLOR}Error: MLDSA is not support on {current_os}")
            error_message += (f"{RESET_TEXT_COLOR}")
            sys.exit(error_message)

    elif(signature_algorithm == "ECDSA"):
        PUBLIC_KEY_LEN = 97
        SECRET_KEY_LEN = 48
        with open("hash.bin", "rb") as digest:
            image_hash = digest.read()
            digest.close()

        ## Read Public Key
        with open((public_key_bin[key_index]), "rb") as public_key_file:
            public_key = public_key_file.read()
            public_key_file.close()

        if(len(public_key) != PUBLIC_KEY_LEN):
            error_message = (f"{RED_TEXT_COLOR}Error: public_key(uncompressed format) length is not {PUBLIC_KEY_LEN}: {len(public_key)}")
            error_message += (f"{RESET_TEXT_COLOR}")
            sys.exit(error_message)

        ## Read Secret Key
        with open((secret_key_bin), "rb") as secret_key_file:
            secret_key = secret_key_file.read()
            ecdsa_sk = SigningKey.from_string(secret_key, curve=NIST384p)
            secret_key_file.close()

        if(len(secret_key) != SECRET_KEY_LEN):
            error_message = (f"{RED_TEXT_COLOR}Error: secret_key length is not {SECRET_KEY_LEN}: {len(secret_key)}")
            error_message += (f"{RESET_TEXT_COLOR}")
            sys.exit(error_message)

        signature = ecdsa_sk.sign_digest_deterministic(image_hash, hashfunc=hashlib.sha384, sigencode=sigencode_string)
        with open("signature.bin", "wb") as signature_file:
            signature_file.write(signature)
            signature_file.flush()
            signature_file.close()

    elif(signature_algorithm is None):
        pass
    else:
        error_message = (f"{RED_TEXT_COLOR}Error: signature_algorithm type is unknown: {signature_algorithm}")
        error_message += (f"{RESET_TEXT_COLOR}")
        sys.exit(error_message)

def calculate_dual_signature(dual_sign, key_index, dual_public_key_bin, dual_secret_key_bin):
    if(dual_sign is False):
        return

    PUBLIC_KEY_LEN = 97
    SECRET_KEY_LEN = 48
    with open("hash2.bin", "rb") as digest:
        image_hash = digest.read()
        digest.close()

    ## Read Public Key
    with open((dual_public_key_bin[key_index]), "rb") as public_key_file:
        public_key = public_key_file.read()
        public_key_file.close()

    if(len(public_key) != PUBLIC_KEY_LEN):
        error_message = (f"{RED_TEXT_COLOR}Error: public_key(uncompressed format) length is not {PUBLIC_KEY_LEN}: {len(public_key)}")
        error_message += (f"{RESET_TEXT_COLOR}")
        sys.exit(error_message)

    ## Read Secret Key
    with open((dual_secret_key_bin), "rb") as secret_key_file:
        secret_key = secret_key_file.read()
        ecdsa_sk = SigningKey.from_string(secret_key, curve=NIST384p)
        secret_key_file.close()

    if(len(secret_key) != SECRET_KEY_LEN):
        error_message = (f"{RED_TEXT_COLOR}Error: secret_key length is not {SECRET_KEY_LEN}: {len(secret_key)}")
        error_message += (f"{RESET_TEXT_COLOR}")
        sys.exit(error_message)

    signature = ecdsa_sk.sign_digest_deterministic(image_hash, hashfunc=hashlib.sha384, sigencode=sigencode_der)
    with open("signature2.bin", "wb") as signature_file:
        signature_file.write(signature)
        signature_file.flush()
        signature_file.close()

def calculate_image_hash_length():
    with open("hash.bin", "rb") as digest:
        image_hash = digest.read()
        length = (
            0x2     # Image Hash TLV Type Len
            + 0x2   # Length of Image Hash TLV Length
            + len(image_hash)
        )
        digest.close()
    return length

def calculate_public_key_length(key_hash_alg, public_key_bin):
    length = 0
    if(key_hash_alg is None):
        return length
    for i, key_path in enumerate(public_key_bin):
        with open((key_path), "rb") as public_key_file:
            public_key = public_key_file.read()
            length += (
                0x2     # Public Key TLV Type Length
                + 0x2   # Length of Public Key TLV Length
                + len(public_key)
            )
            public_key_file.close()
    return length

def calculate_signature_length(signature_algorithm):
    length = 0
    if(signature_algorithm is None):
        return length
    with open("signature.bin", "rb") as signature_file:
        signature = signature_file.read()
        length += (
            0x2     # Signature TLV Type Length
            + 0x2   # Length of Signature TLV Length
            + len(signature)
        )
        signature_file.close()
    return length

def calculate_dual_image_hash_length(dual_sign):
    if(dual_sign is False):
        return 0
    with open("hash2.bin", "rb") as digest:
        image_hash = digest.read()
        length = (
            0x2     # Image Hash TLV Type Len
            + 0x2   # Length of Image Hash TLV Length
            + len(image_hash)
        )
        digest.close()
    return length

def calculate_dual_public_key_length(dual_sign, dual_public_key_bin):
    length = 0
    if(dual_sign is False):
        return length
    for i, key_path in enumerate(dual_public_key_bin):
        with open((key_path), "rb") as public_key_file:
            public_key = public_key_file.read()
            length += (
                0x2     # Public Key TLV Type Length
                + 0x2   # Length of Public Key TLV Length
                + len(public_key)
            )
            public_key_file.close()
    return length

def calculate_dual_signature_length(dual_sign):
    length = 0
    if(dual_sign is False):
        return length
    with open("signature2.bin", "rb") as signature_file:
        signature = signature_file.read()
        length += (
            0x2     # Signature TLV Type Length
            + 0x2   # Length of Signature TLV Length
            + len(signature)
        )
        signature_file.close()
    return length

def calculate_tlv_length(key_hash_alg, public_key_bin, signature_algorithm, dual_sign, dual_public_key_bin):
    tlv_length = (
        0x2     # TLV Magic Length
        + 0x2   # Length of TLV Len
    )
    tlv_length += calculate_image_hash_length()
    tlv_length += calculate_public_key_length(key_hash_alg, public_key_bin)
    tlv_length += calculate_signature_length(signature_algorithm)
    tlv_length += calculate_dual_image_hash_length(dual_sign)
    tlv_length += calculate_dual_public_key_length(dual_sign, dual_public_key_bin)
    tlv_length += calculate_dual_signature_length(dual_sign)
    return tlv_length

def tlv_init(tlv_length):
    tag_payload = bytearray(0)
    tag_fmt = (
        "<"
        # type ImageTag struct {
        + "H"  # TLV Magic uint16
        + "H"  # TLV Length uint16
    )  # }
    tag = struct.pack(
        tag_fmt,
        TLV_MAGIC,
        tlv_length,
    )
    tag_payload[: len(tag)] = tag
    with open("tlv.bin", "wb") as tlv:
        tlv.write(tag_payload)
        tlv.flush()
        tlv.close()

def add_image_hash2tlv(image_hash_alg):
    if(image_hash_alg == "SHA2_384"):
        tlv_image_hash = TLV_IMAGE_HASH_SHA2_384
    elif(image_hash_alg == "SHA3_384"):
        tlv_image_hash = TLV_IMAGE_HASH_SHA3_384
    elif(image_hash_alg == "SHA3_512"):
        tlv_image_hash = TLV_IMAGE_HASH_SHA3_512
    elif(image_hash_alg == "SHA2_256"):
        tlv_image_hash = TLV_IMAGE_HASH_SHA2_256
    else:
        error_message = (f"{RED_TEXT_COLOR}Error: argument -H/--img-hash")
        error_message += (f"{RESET_TEXT_COLOR}")
        sys.exit(error_message)

    image_hash_payload = bytearray(0)

    image_hash_fmt = (
        "<"
        # type ImageTag struct {
        + "H"  # TLV Type for Image Hash uint16
        + "H"  # Image Hash Length uint16
    )  # }

    with open("hash.bin", "rb") as digest:
        image_hash = digest.read()
        digest.close()

    image_hash_tlv = struct.pack(
        image_hash_fmt,
        tlv_image_hash,
        len(image_hash)
    )

    image_hash_payload[: len(image_hash_tlv)] = image_hash_tlv

    with open("tlv.bin", "ab") as tlv:
        tlv.write(image_hash_payload)
        tlv.write(image_hash)
        tlv.flush()
        tlv.close()

def add_public_key2tlv(key_hash_alg, public_key_bin, signature_algorithm):
    if(key_hash_alg is None):
        return

    public_key_payload = bytearray(0)

    public_key_fmt = (
        "<"
        # type Public Key struct {
        + "H"  # TLV Type for Public Key uint16
        + "H"  # Public Key Length uint16
    )  # }

    for i, key_path in enumerate(public_key_bin):
        if(key_hash_alg == "SHA2_384"):
            if(signature_algorithm == "ECDSA"):
                tlv_key_hash = (TLV_PUBLIC_KEY1_HASH_SHA2_384 | i)
            else: 
                tlv_key_hash = (TLV_PUBLIC_KEY0_HASH_SHA2_384 | i)

        elif(key_hash_alg == "SHA3_384"):
            if(signature_algorithm == "ECDSA"):
                tlv_key_hash = (TLV_PUBLIC_KEY1_HASH_SHA3_384 | i)
            else: 
                tlv_key_hash = (TLV_PUBLIC_KEY0_HASH_SHA3_384 | i)
        else:
            error_message = (f"{RED_TEXT_COLOR}Error: argument -k/--key-hash-alg")
            error_message += (f"{RESET_TEXT_COLOR}")
            sys.exit(error_message)

        with open((key_path), "rb") as public_key_file:
            public_key = public_key_file.read()
            public_key_file.close()

        public_key_tlv = struct.pack(
            public_key_fmt,
            tlv_key_hash,
            len(public_key),
        )

        public_key_payload[: len(public_key_tlv)] = public_key_tlv
        
        with open("tlv.bin", "ab") as tlv:
            tlv.write(public_key_payload)
            tlv.write(public_key)
            tlv.flush()
            tlv.close()

def add_signature2tlv(signature_algorithm):
    if(signature_algorithm is None):
        return

    if(signature_algorithm == "LMS"):
        tlv_signature_algorithm = TLV_SIGNATURE_LMS
    elif(signature_algorithm == "MLDSA"):
        tlv_signature_algorithm = TLV_SIGNATURE_ML_DSA_87
    elif(signature_algorithm == "ECDSA"):
        tlv_signature_algorithm = TLV_SIGNATURE_ECDSA
    elif(signature_algorithm is None):
        pass
    else:
        error_message = (f"{RED_TEXT_COLOR}Error: argument -S/--signature-algorithm")
        error_message += (f"{RESET_TEXT_COLOR}")
        sys.exit(error_message)

    signature_payload = bytearray(0)

    signature_fmt = (
        "<"
        # type Signature struct {
        + "H"  # TLV Type for Signature uint16
        + "H"  # Signature Length uint16
    )  # }

    with open("signature.bin", "rb") as signature_file:
        signature = signature_file.read()
        signature_file.close()

    signature_tlv = struct.pack(
        signature_fmt,
        tlv_signature_algorithm,
        len(signature),
    )

    signature_payload[: len(signature_tlv)] = signature_tlv

    with open("tlv.bin", "ab") as tlv:
        tlv.write(signature_payload)
        tlv.write(signature)
        tlv.flush()
        tlv.close()

def add_dual_image_hash2tlv(dual_sign, dual_image_hash_alg):
    if(dual_sign is False):
        return

    if(dual_image_hash_alg == "SHA2_384"):
        tlv_image_hash = TLV_IMAGE_HASH_SHA2_384
    elif(dual_image_hash_alg == "SHA3_384"):
        tlv_image_hash = TLV_IMAGE_HASH_SHA3_384
    else:
        error_message = (f"{RED_TEXT_COLOR}Error: argument -dH/--dual-img-hash")
        error_message += (f"{RESET_TEXT_COLOR}")
        sys.exit(error_message)

    image_hash_payload = bytearray(0)

    image_hash_fmt = (
        "<"
        # type ImageTag struct {
        + "H"  # TLV Type for Image Hash uint16
        + "H"  # Image Hash Length uint16
    )  # }

    with open("hash2.bin", "rb") as digest:
        image_hash = digest.read()
        digest.close()

    image_hash_tlv = struct.pack(
        image_hash_fmt,
        tlv_image_hash,
        len(image_hash)
    )

    image_hash_payload[: len(image_hash_tlv)] = image_hash_tlv

    with open("tlv.bin", "ab") as tlv:
        tlv.write(image_hash_payload)
        tlv.write(image_hash)
        tlv.flush()
        tlv.close()

def add_dual_public_key2tlv(dual_sign, dual_key_hash_alg, dual_public_key_bin):
    if(dual_sign is False):
        return

    public_key_payload = bytearray(0)

    public_key_fmt = (
        "<"
        # type Public Key struct {
        + "H"  # TLV Type for Public Key uint16
        + "H"  # Public Key Length uint16
    )  # }

    for i, key_path in enumerate(dual_public_key_bin):
        if(dual_key_hash_alg == "SHA2_384"):
            tlv_key_hash = (TLV_PUBLIC_KEY1_HASH_SHA2_384 | i)

        elif(dual_key_hash_alg == "SHA3_384"):
            tlv_key_hash = (TLV_PUBLIC_KEY1_HASH_SHA3_384 | i)
        else:
            error_message = (f"{RED_TEXT_COLOR}Error: argument -dk/--dual-key-hash-alg")
            error_message += (f"{RESET_TEXT_COLOR}")
            sys.exit(error_message)

        with open((key_path), "rb") as public_key_file:
            public_key = public_key_file.read()
            public_key_file.close()

        public_key_tlv = struct.pack(
            public_key_fmt,
            tlv_key_hash,
            len(public_key),
        )

        public_key_payload[: len(public_key_tlv)] = public_key_tlv
        
        with open("tlv.bin", "ab") as tlv:
            tlv.write(public_key_payload)
            tlv.write(public_key)
            tlv.flush()
            tlv.close()

def add_dual_signature2tlv(dual_sign):
    if(dual_sign is False):
        return

    tlv_signature_algorithm = TLV_SIGNATURE_ECDSA

    signature_payload = bytearray(0)

    signature_fmt = (
        "<"
        # type Signature struct {
        + "H"  # TLV Type for Signature uint16
        + "H"  # Signature Length uint16
    )  # }

    with open("signature2.bin", "rb") as signature_file:
        signature = signature_file.read()
        signature_file.close()

    signature_tlv = struct.pack(
        signature_fmt,
        tlv_signature_algorithm,
        len(signature),
    )

    signature_payload[: len(signature_tlv)] = signature_tlv

    with open("tlv.bin", "ab") as tlv:
        tlv.write(signature_payload)
        tlv.write(signature)
        tlv.flush()
        tlv.close()

def construct_tlv(image_hash_alg, key_hash_alg, public_key_bin, signature_algorithm, dual_sign, dual_public_key_bin, dual_image_hash_alg, dual_key_hash_alg):

    tlv_length = calculate_tlv_length(key_hash_alg, public_key_bin, signature_algorithm, dual_sign, dual_public_key_bin)
    tlv_init(tlv_length)
    add_image_hash2tlv(image_hash_alg)
    add_public_key2tlv(key_hash_alg, public_key_bin, signature_algorithm)
    add_signature2tlv(signature_algorithm)
    add_dual_image_hash2tlv(dual_sign, dual_image_hash_alg)
    add_dual_public_key2tlv(dual_sign, dual_key_hash_alg, dual_public_key_bin)
    add_dual_signature2tlv(dual_sign)

def generate_signed_image(original_bin, signed_bin):
    with open(signed_bin, "wb") as signed, open("hdr.bin", "rb") as hdr, open(original_bin, "rb") as original, open("tlv.bin", "rb") as tlv:
        signed.write(hdr.read())
        signed.write(original.read())
        signed.write(tlv.read())
        signed.flush()
        signed.close()
        hdr.close()
        original.close()
        tlv.close()
    print(f"Sign {original_bin} to {signed_bin} successfully")

def main():
    """
    Image generateor tool entry point
    """
    args = parse_args()
    if args.verbose:
        print(f"  Input = {args.original_bin}")
        print(f"  Output = {args.signed_bin}")
        print(f"  Load Address = {hex(args.load_addr)}")
        print(f"  SPI Frequency = {args.spi_freq}")
        print(f"  SPI Read Command = {args.spi_rdcmd}")
        print(f"  Image Revision = {args.revision}")
        print(f"  Key Index = {args.key_index}")
        print(f"  Image Hash Algorithm = {args.image_hash_alg}")
        print(f"  Public Key File = {args.public_key_bin}")
        print(f"  Secret Key File = {args.secret_key_bin}")
        print(f"  Public Key Hash Algirithm = {args.key_hash_alg}")
        print(f"  Signature Algorithm = {args.signature_algorithm}")

    check_valid_argument(
        args.load_addr,
        args.spi_freq,
        args.spi_rdcmd,
        args.key_index,
        args.revision,
        args.original_bin,
        args.signed_bin,
        args.image_hash_alg,
        args.public_key_bin,
        args.secret_key_bin,
        args.key_hash_alg,
        args.signature_algorithm,
        args.dual_sign,
        args.dual_image_hash_alg,
        args.dual_public_key_bin,
        args.dual_secret_key_bin,
        args.dual_key_hash_alg,
    )

    generate_image_header(
        args.original_bin,
        args.load_addr,
        args.spi_freq,
        args.spi_rdcmd,
        args.key_index,
        args.revision,
        args.image_hash_alg,
        args.signature_algorithm,
    )

    calculate_image_hash(args.original_bin, args.image_hash_alg)

    calculate_dual_image_hash(args.original_bin, args.dual_image_hash_alg, args.dual_sign)

    calculate_signature(args.key_index, args.public_key_bin, args.secret_key_bin, args.signature_algorithm)
    # Dual signagure only supoorts ECDSA
    calculate_dual_signature(args.dual_sign, args.key_index, args.dual_public_key_bin, args.dual_secret_key_bin)

    construct_tlv(args.image_hash_alg, args.key_hash_alg, args.public_key_bin, args.signature_algorithm, args.dual_sign, args.dual_public_key_bin, args.dual_image_hash_alg, args.dual_key_hash_alg)

    generate_signed_image(args.original_bin, args.signed_bin)

if __name__ == "__main__":

    main()