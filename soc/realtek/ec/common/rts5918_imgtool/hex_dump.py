import argparse

def convert_image(input_bin_file, output_file):
    try:
        # Step 1: Read the binary file
        with open(input_bin_file, 'rb') as bin_file:
            binary_data = bin_file.read()

        image = "const uint8_t image[] = {"

        # Iterate over each byte, format as hex, and append to the output string
        for i, byte in enumerate(binary_data):
            # Print each byte in hex format with two digits, prefixed by 0x
            if i % 16 == 0:
                image += "\n    "
            image += f"0x{byte:02x}, "

        # Remove the trailing comma and space, and close the brace
        image += "\n};"

        with open(output_file, 'w') as file:
            file.write("#include <stdint.h>")
            file.write("\n")
            file.write("\n")

        image_len = "const uint32_t image_len = "
        image_len += str(hex(len(binary_data)))
        image_len += ";\n"

        with open(output_file, 'a') as file:
            file.write(image_len)
            file.write("\n")

        with open(output_file, 'a') as file:
            file.write(image)
            file.write("\n")

    except FileNotFoundError:
        print(f"Error: The file {input_bin_file} was not found.")
    except Exception as e:
        print(f"An error occurred: {e}")

def main():
    # Set up argument parsing
    parser = argparse.ArgumentParser(description='Convert a binary file to a hex byte array and save as a text file.')
    parser.add_argument('input_bin_file', type=str, help='Path to the input binary file.')
    parser.add_argument('-o', '--output', type=str, default='image.h', help='Path to the output text file (optional).')

    # Parse the arguments
    args = parser.parse_args()

    # Call the conversion function
    convert_image(args.input_bin_file, args.output)

if __name__ == '__main__':
    main()