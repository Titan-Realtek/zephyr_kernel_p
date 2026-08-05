import sys
import hexdump
from enum import IntEnum

class SpiFreq(IntEnum):
    FREQ_50M = 0
    FREQ_25M = 1
    FREQ_12P5M = 2
    FREQ_6P25M = 3

class SpiRdCmd(IntEnum):
    NORMAL_READ = 0x03
    DUAL_READ = 0x3B
    QUAD_READ = 0x6B

class TlvTag(IntEnum):
    TLV_KEYHASH = 0x01  
    TLV_PUBKEY = 0x02  
    TLV_SHA256 = 0x10  
    TLV_SHA384 = 0x11  
    TLV_RSA2048_PSS = 0x20  
    TLV_ECDSA224 = 0x21  
    TLV_ECDSA_SIG = 0x22  
    TLV_RSA3072_PSS = 0x23  
    TLV_ED25519 = 0x24  
    TLV_ENC_RSA2048 = 0x30  
    TLV_ENC_KW = 0x31  
    TLV_ENC_EC256 = 0x32  
    TLV_ENC_X25519 = 0x33  
    TLV_DEPENDENCY = 0x40  
    TLV_SEC_CNT = 0x50  
    TLV_BOOT_RECORD = 0x60  
    TLV_SHA2_384 = 0xA0  
    TLV_SHA3_384 = 0xA1  
    TLV_SHA3_512 = 0xA2  
    TLV_KEYSLOT0_SHA2_384 = 0x1000
    TLV_KEYSLOT1_SHA2_384 = 0x1010
    TLV_KEYSLOT0_SHA3_384 = 0x1020
    TLV_KEYSLOT1_SHA3_384 = 0x1030
    TLV_SIG_LMS = 0xF0  
    TLV_SIG_ML_DSA_87 = 0xF1  

def get_spifmt_string(selector):
    try:
        return SpiFreq(selector).name
    except ValueError:
        return "UNKNOWN_FREQ"

def get_spirdcmd_string(selector):
    try:
        return SpiRdCmd(selector).name
    except ValueError:
        return "UNKNOWN_RDCMD"

def get_tlvtag_string(selector):
    try:
        return TlvTag(selector).name
    except ValueError:
        return "UNKNOWN_TLV_TAG"

# Get input stream from standard input
# input_stream = sys.stdin.buffer.read()
# input_buffer = bytearray(input_stream)

# Get input stream from argv
with open(sys.argv[1], 'rb') as f:
    input_buffer = bytearray(f.read())

# Parser image header
image_hdr = input_buffer[0:32]
image_hdr_magic = image_hdr[0:4].decode('ascii')
image_hdr_load_addr = int.from_bytes(image_hdr[4:8], byteorder='little')
image_hdr_header_size = int.from_bytes(image_hdr[8:12], byteorder='little')
image_hdr_image_size = int.from_bytes(image_hdr[12:16], byteorder='little')
image_hdr_jump_addr = int.from_bytes(image_hdr[16:20], byteorder='little')
image_hdr_flags = int.from_bytes(image_hdr[20:24], byteorder='little')
image_hdr_spifmt = int(image_hdr[28]) >> 2
image_hdr_spirdcmd = int(image_hdr[29])
image_hdr_keyidx = int(image_hdr[30])
image_hdr_revision = int(image_hdr[31])

# Print header content
print(r'[Image Header]')
hexdump.hexdump(image_hdr)
print(f'* Magic Word   : {image_hdr_magic}')
print(f'* Load Address : 0x{image_hdr_load_addr:08X}')
print(f'* Header Size  : {image_hdr_header_size:d}')
print(f'* Image Size   : {image_hdr_image_size:d} (0x{image_hdr_image_size:X})')
print(f'* Jump Address : 0x{image_hdr_jump_addr:x}')
print(f'* Flags        : 0x{image_hdr_flags:x}')
print(f'* SPI Format   : {get_spifmt_string(image_hdr_spifmt)}')
print(f'* SPI RdCmd    : {get_spirdcmd_string(image_hdr_spirdcmd)}')
print(f'* Key Index    : {image_hdr_keyidx:d}')
print(f'* Revision     : {image_hdr_revision:d}')
print()

# Parser TLV header
tlv_offset = image_hdr_header_size + image_hdr_image_size
tlv_hdr = input_buffer[tlv_offset:tlv_offset+4]
tlv_hdr_magic = tlv_hdr[0:2]
tlv_hdr_length = int.from_bytes(tlv_hdr[2:4], byteorder='little')

# Print TLV header
print(r'[TLV Header]')
hexdump.hexdump(tlv_hdr)
print(f'* Magic Word   : {tlv_hdr_magic[0]:02X} {tlv_hdr_magic[1]:02X}')
print(f'* TLV Length   : {tlv_hdr_length} (0x{tlv_hdr_length:X})')

# Print TLV recursive
tlv_idx = 0
tlv_content_offset = tlv_offset + 4
accumul_read = 0
while accumul_read < tlv_hdr_length:
    offset = tlv_content_offset+accumul_read
    tlv_tag = input_buffer[offset:offset+2]
    tlv_len = input_buffer[offset+2:offset+4]
    tlv_tag = int.from_bytes(tlv_tag, byteorder='little')
    tlv_len = int.from_bytes(tlv_len, byteorder='little')
    tlv_value = input_buffer[offset+4:offset+4+tlv_len]
    print(f'* [TLV Content {tlv_idx}]')
    print(f'    * TLV Tag : {tlv_tag:04X} ({get_tlvtag_string(tlv_tag)})')
    print(f'    * TLV Len : {tlv_len:d} (0x{tlv_len:04X})')
    hexdump.hexdump(tlv_value)

    # Update
    tlv_idx += 1
    accumul_read += 4 + tlv_len