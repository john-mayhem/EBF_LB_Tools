#include "ConfigManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <filesystem>  // Requires C++17

namespace fs = std::filesystem;


// Default configuration
const Configuration defaultConfig = {
    "   ", // authKey
    "https://epic-boss-fight.firebaseio.com/", // serverLocation
    "john-mayhem", // owner
    "ebf_lb", // repo
    "main", // branch
    "leaderboard", // folderPath
    "   " // token
};

void ensureConfigExists() {
    if (!fs::exists("config")) {
        fs::create_directory("config");
        std::cout << "> Configs not found. Creating.\n\n";
    }

    std::string configFilePath = "config/config.ini";
    if (!fs::exists(configFilePath)) {
        std::ofstream configFile(configFilePath);
        std::cout << "> Restoring and using config defaults.\n\n";
        if (configFile.is_open()) {
            // Write default configuration to file. Make sure each section is correctly formatted.
            configFile << "[Auth]\nKey=" << defaultConfig.authKey << "\n\n";
            configFile << "[Server]\nLocation=" << defaultConfig.serverLocation << "\n\n";

            // Consolidate the GitHub section into a single section
            configFile << "[GitHub]\n";  // Start the GitHub section
            configFile << "Owner=" << defaultConfig.owner << "\n";
            configFile << "Repo=" << defaultConfig.repo << "\n";
            configFile << "Branch=" << defaultConfig.branch << "\n";
            configFile << "Token=" << defaultConfig.token << "\n\n";  // Note: Fixed the mix-up here

            configFile << "[General]\nFolderPath=" << defaultConfig.folderPath << "\n";  // And here

            configFile.close();
        }
        else {
            std::cerr << "Unable to create config file.";
        }
    }
}


Configuration loadConfiguration(const std::string& filepath) {
    Configuration config;  // Configuration object to store the parsed data
    std::cout << "> Loading config data.\n\n";

    std::ifstream configFile(filepath);
    if (!configFile.is_open()) {
        std::cerr << "Failed to open config file, using default configuration." << std::endl;
        return defaultConfig;  // If we fail to open the file, return the default configuration
    }

    std::string line, currentSection;
    std::map<std::string, std::string> configEntries;  // To store key-value pairs from the file

    while (std::getline(configFile, line)) {
        // Remove whitespaces from the beginning of the line
        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](unsigned char ch) {
            return !std::isspace(ch);
            }));

        // Ignore comments and empty lines
        if (line.empty() || line[0] == '#' || line[0] == ';')
            continue;

        // Check if we are within a section
        if (line[0] == '[') {
            // We are entering a new section
            int endPos = line.find_first_of(']');
            if (endPos == std::string::npos) {
                // Syntax error (missing ']'), handle the error here, e.g., skip the line
                continue;
            }

            currentSection = line.substr(1, endPos - 1);
            continue;
        }

        // If we reach here, we are processing a key-value pair within a section
        int delimiterPos = line.find_first_of('=');
        if (delimiterPos != std::string::npos) {
            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);

            // Optional: Trim whitespaces from key and value
            key.erase(key.find_last_not_of(" \t\n\r\f\v") + 1);
            value.erase(0, value.find_first_not_of(" \t\n\r\f\v"));
            value.erase(value.find_last_not_of(" \t\n\r\f\v") + 1);

            // Insert the key-value pair into the map
            configEntries[currentSection + "." + key] = value;
        }
    }

    configFile.close();

    // Now, you can assign the values from the map to your Configuration structure.
    // Make sure to perform all necessary checks and error handling, in case some keys are missing.
    config.authKey = configEntries["Auth.Key"];
    config.serverLocation = configEntries["Server.Location"];
    config.owner = configEntries["GitHub.Owner"];
    config.repo = configEntries["GitHub.Repo"];
    config.branch = configEntries["GitHub.Branch"];
    config.token = configEntries["GitHub.Token"];
    config.folderPath = configEntries["General.FolderPath"];

    return config;
}