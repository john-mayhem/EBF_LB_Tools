#ifndef FILE_ENCODER_H
#define FILE_ENCODER_H

#include <string>

// Function declaration for global use (not part of the FileEncoder class)
std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);

class FileEncoder {
public:
    // Method to encode a file's content and save it to a new file
    static void encodeFileToBase64(const std::string& sourceFilePath, const std::string& destinationFilePath);
};

#endif //FILE_ENCODER_H
