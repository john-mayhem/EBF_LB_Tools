#pragma once
#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>

struct Configuration {
    std::string authKey;
    std::string serverLocation;
    std::string owner;
    std::string repo;
    std::string branch;
    std::string folderPath;
    std::string token;
};

// Function declarations
void ensureConfigExists();
Configuration loadConfiguration(const std::string& filepath);

// Default configuration values
extern const Configuration defaultConfig;

#endif  // CONFIG_MANAGER_H
