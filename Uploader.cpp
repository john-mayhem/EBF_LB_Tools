#include "Uploader.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>  // For checking if file exists
#include <cpr/cpr.h>  // Include the cpr library for HTTP requests
#include <nlohmann/json.hpp>
#include <sstream>  // Necessary for std::stringstream
#include "FileEncoder.h"

Uploader::Uploader(const std::string& configFilePath) {
    // Load the configuration at the creation of the Uploader object
    this->config = loadConfiguration(configFilePath);
}

void Uploader::uploadFiles() {
    std::string mmrFilePath = "./data/lb_1.txt";
    std::string wrFilePath = "./data/lb_2.txt";

    if (fileExists(mmrFilePath) && fileExists(wrFilePath)) {
        std::string encodedMmrContent = readEncodedFile(mmrFilePath);
        std::string encodedWrContent = readEncodedFile(wrFilePath);

        // Upload the content. You should handle the actual HTTP requests here.
        uploadToGitHub(encodedMmrContent, "lb_1.txt");
        uploadToGitHub(encodedWrContent, "lb_2.txt");
    }
    else {
        std::cerr << "One or both files do not exist or are empty." << std::endl;
    }
}

std::string Uploader::readEncodedFile(const std::string& filePath) {
    // Open the file and read it into a buffer
    std::ifstream file(filePath, std::ios::binary);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    // Get the string content
    std::string content = buffer.str();

    // Encode the content in base64
    // This uses the base64 function from earlier in the process.
    // Adjust according to how you have base64 encoding set up in your project.
    std::string encodedContent = base64_encode(reinterpret_cast<const unsigned char*>(content.c_str()), content.length());

    return encodedContent;
}

bool Uploader::fileExists(const std::string& filePath) {
    struct stat buffer;
    return (stat(filePath.c_str(), &buffer) == 0);
}

void Uploader::uploadToGitHub(const std::string& fileContent, const std::string& targetFile) {
    // Construct the URL for the GitHub API
    std::string url = "https://api.github.com/repos/" + this->config.owner + "/" + this->config.repo + "/contents/" + this->config.folderPath + "/" + targetFile;

    // Headers for the requests
    cpr::Header headers = {
        {"Authorization", "token " + this->config.token},
        {"User-Agent", "cpr-example"}  // GitHub requires a user-agent header
    };

    // First, make a GET request to retrieve the file's current SHA
    auto getResponse = cpr::Get(cpr::Url{ url }, headers);

    // Check if the GET request was successful
    if (getResponse.status_code != 200) {
        std::cerr << "Failed to retrieve the SHA for '" << targetFile << "'. Status code: " << getResponse.status_code << " Error: " << getResponse.text << std::endl;
        return;
    }

    // Parse the JSON response to extract the SHA
    nlohmann::json jsonResponse = nlohmann::json::parse(getResponse.text);
    std::string sha;
    if (jsonResponse.contains("sha")) {
        sha = jsonResponse["sha"];
    }
    else {
        std::cerr << "SHA not found in the response for '" << targetFile << "'." << std::endl;
        return;
    }

    // Create the payload for the update request, including the SHA
    cpr::Body payload{ R"({"message": "Leaderboard data update", "content": ")" + fileContent + R"(", "sha": ")" + sha + R"("})" };

    // Make the PUT request to update the file on GitHub
    auto putResponse = cpr::Put(cpr::Url{ url }, headers, payload);

    // Check the status of the PUT request
    if (putResponse.status_code == 200 || putResponse.status_code == 201) {
        std::cout << "File '" << targetFile << "' uploaded successfully. Status code: " << putResponse.status_code << std::endl;
    }
    else {
        std::cerr << "Failed to upload '" << targetFile << "'. Status code: " << putResponse.status_code << " Error: " << putResponse.text << std::endl;
    }
}
