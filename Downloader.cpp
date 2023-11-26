#include <iostream>
#include <fstream>
#include "Downloader.h"
#include "ConfigManager.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <thread>
#include <atomic>
#include <chrono>
#include <filesystem> 


// This function will run in a separate thread and show a simple spinning progress bar
void showProgressBar(std::atomic<bool>& done) {
    char progressIndicators[] = { '/', '-', '\\', '|' };
    int progressIndex = 0;
    while (!done) {
        std::cout << "\r" << progressIndicators[progressIndex] << std::flush;
        progressIndex = (progressIndex + 1) % 4;
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // update speed of the spinner
    }
    std::cout << "\r \r" << std::flush; // clear the progress bar
}

void RetrieveAndSavePlayerData(const std::string& serverLocation, const std::string& authKey) {
    std::atomic<bool> downloadDone(false);

    std::thread progressBarThread(showProgressBar, std::ref(downloadDone));

    std::string requestUrl = serverLocation + authKey + "/players.json";

    // std::cout << requestUrl << std::endl;

    // Perform the GET request using the CPR library
    cpr::Response r = cpr::Get(cpr::Url{ requestUrl });
    std::cout << "> Retrieving data\n\n";

    downloadDone = true;
    if (progressBarThread.joinable()) {
        progressBarThread.join(); // ensure the progress bar thread finishes cleanly
    }


    // Check if the request was successful
    if (r.status_code == 200) {

        std::cout << "> Data acquisition success\n\n";

        // Parse the response text (JSON) with nlohmann's json library
        nlohmann::json jsonObject = nlohmann::json::parse(r.text);

        // Serialize the JSON to a string with pretty formatting
        std::string formattedJson = jsonObject.dump(4); // '4' here indicates an indent of 4 spaces for pretty formatting

        namespace fs = std::filesystem;  // use a namespace alias for convenience

        std::string dataDirectory = "./data";  // relative path to the data directory from the executable
        std::string outputFilePath = dataDirectory + "/output.json";

        try {
            // Check if 'data' directory exists, create it if not
            if (!fs::exists(dataDirectory)) {
                fs::create_directories(dataDirectory);  // can create directories recursively if needed
            }
            else {
                // If the directory exists, we clear any files within it
                for (const auto& file : fs::directory_iterator(dataDirectory)) {
                    fs::remove(file);  // remove the file. Be cautious: this will delete the files!
                }
            }

            // Save the formatted JSON to a file in the 'data' directory
            std::ofstream outputFile(outputFilePath);
            if (outputFile.is_open()) {
                outputFile << formattedJson;
                outputFile.close();
                std::cout << "> Data saved to " << outputFilePath << std::endl;
                std::cout << "\n";
            }
            else {
                std::cerr << "! Unable to open file for writing: " << outputFilePath << std::endl;
            }
        }
        catch (const fs::filesystem_error& e) {
            std::cerr << "!Filesystem error: " << e.what() << std::endl;
            // handle the error, e.g., by letting the function return an error code
        }
        catch (const std::exception& e) {
            std::cerr << "!General error: " << e.what() << std::endl;
            // handle the error
        }
    }
    else {
        std::cerr << "!Failed to make request. Status code: " << r.status_code << std::endl;
    }
}

