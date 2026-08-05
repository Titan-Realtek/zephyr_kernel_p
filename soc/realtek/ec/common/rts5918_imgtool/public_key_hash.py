import argparse
import hashlib
import os
import platform
import struct


def parse_args():
    """
    Parsing the arguments
    """
    parser = argparse.ArgumentParser(allow_abbrev=False)

    parser.add_argument(
        "-I",
        "--input",
        type=str,
        dest="public_key",
        required=True,
        help="Input bin file path",
    )

    ret_args = parser.parse_args()
    return ret_args

def main():

    args = parse_args()

    public_key_sha2_bin = f"{args.public_key.rsplit('.', 1)[0]}_sha2_384.txt"
    with open(public_key_sha2_bin, 'w', encoding='utf-8') as outfile, open(args.public_key, "rb") as infile:
        sha = hashlib.sha384()
        sha.update(infile.read())
        public_key_hash = sha.digest()

        target_endianness = '<' # '<' for Little-Endian
        uint32_array = struct.unpack(target_endianness + '12I', public_key_hash)
        outfile.write("{\n")
        for i, num in enumerate(uint32_array):
            log_entry = f"  0x{num:08X}, \n"
            outfile.write(log_entry)
        outfile.write("},\n")
        outfile.flush()
        outfile.close()
        infile.close()

    public_key_sha3_bin = f"{args.public_key.rsplit('.', 1)[0]}_sha3_384.txt"
    with open(public_key_sha3_bin, 'w', encoding='utf-8') as outfile, open(args.public_key, "rb") as infile:
        sha = hashlib.sha3_384()
        sha.update(infile.read())
        public_key_hash = sha.digest()

        target_endianness = '<' # or '<' for Little-Endian
        uint32_array = struct.unpack(target_endianness + '12I', public_key_hash)
        outfile.write("{\n")
        for i, num in enumerate(uint32_array):
            log_entry = f"  0x{num:08X}, \n"
            outfile.write(log_entry)
        outfile.write("},\n")
        outfile.flush()
        outfile.close()
        infile.close()

    print(f"write public key sha2 hash to {public_key_sha2_bin}")
    print(f"write public key sha3 hash to {public_key_sha3_bin}")

if __name__ == "__main__":

    main()