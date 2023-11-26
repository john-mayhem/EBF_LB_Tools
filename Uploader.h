#ifndef UPLOADER_H
#define UPLOADER_H

#include <string>
#include "ConfigManager.h"  // Include this to use the Configuration struct

class Uploader {
public:
    Uploader(const std::string& configFilePath);
    void uploadFiles();

private:
    std::string readEncodedFile(const std::string& filePath);
    bool fileExists(const std::string& filePath);
    void uploadToGitHub(const std::string& fileContent, const std::string& targetFile);

    Configuration config;  // Store configuration here
};

#endif //UPLOADER_H
