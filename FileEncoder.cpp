#include "FileEncoder.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <iterator>

#include <string>

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while ((i++ < 3))
            ret += '=';
    }

    return ret;
}


void FileEncoder::encodeFileToBase64(const std::string& sourceFilePath, const std::string& destinationFilePath) {
    // Open the source file
    std::ifstream inputFile(sourceFilePath, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open source file: " << sourceFilePath << std::endl;
        return;
    }

    // Read the file into a string
    std::string fileContents((std::istreambuf_iterator<char>(inputFile)),
        std::istreambuf_iterator<char>());

    // Close the input file
    inputFile.close();

    // Encode the string using Base64
    std::string encodedData = base64_encode(reinterpret_cast<const unsigned char*>(fileContents.c_str()), fileContents.size());

    // Open the destination file
    std::ofstream outputFile(destinationFilePath);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to create destination file: " << destinationFilePath << std::endl;
        return;
    }

    // Write the encoded data to the file
    outputFile << encodedData;

    // Close the output file
    outputFile.close();

    std::cout << "File encoded successfully and saved to " << destinationFilePath << std::endl;
}