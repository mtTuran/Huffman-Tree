# Huffman Compression Project

## Overview

This project implements a Canonical Huffman coding algorithm for file compression and decompression.  
The program reads input files, compresses them using Huffman encoding, and reconstructs the original file by decoding the compressed data.

The goal is to provide a functional example of Huffman encoding/decoding in C, including building the Huffman tree, encoding the data, and decoding it back to the original form.

The real goal is for me to learn data structures better while also practicing pointers.

---

## Project Structure

huffman-compression-project/
├── src/ # Source code files
│ ├── compression.c # Compression algorithm implementation
│ ├── decompression.c # Decompression algorithm implementation
│ ├── huffman_tree.c # Huffman tree creation and management
│ ├── hashing.c # Hashing utilities
│ ├── main_compress.c # Main function for compression
│ └── main_decompress.c # Main function for decompression
├── include/ # Header files
│ ├── huffman_tree.h
│ └── hashing.h
├── data/ # Example input/output files
│ ├── input.txt
│ ├── compressed.bin
│ └── decompressed.txt

---

## How to Compile

Since this project does not include a build system like Makefile or setup scripts, you can compile the executables manually using `gcc` from the terminal.

### Compile Compression Executable
```powershell
cd huffman-compression-project                                                  # enter the project folder
gcc src/main_compress.c src/huffman.c src/hashtable.c -o build/compress         # compile the compressor and put it into build folder
gcc src/main_decompress.c src/huffman.c src/hashtable.c -o build/decompress     # compile the decompressor and put it into build folder
```
## How to Run

### Compress a file
```powershell
cd build            # enter the executable folder
./compress.exe      # run the compressor
./decompress.exe    # run the decompressor
```

## Notes

Make sure your terminal is in the project root directory when running these commands.

You may need to adjust file paths based on your environment. Especially since the txt file that is to be compressed end decompressed is hardcoded into code, you need to be careful about filenames.

The project currently supports basic text file compression; only the .txt files are tested.
