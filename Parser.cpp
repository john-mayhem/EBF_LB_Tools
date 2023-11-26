/***************************************************************************
 * #########################################################################
 *#  ______       _        ____                  ______ _       _     _    #
 *# |  ____|     (_)      |  _ \                |  ____(_)     | |   | |   #
 *# | |__   _ __  _  ___  | |_) | ___  ___ ___  | |__   _  __ _| |__ | |_  #
 *# |  __| | '_ \| |/ __| |  _ < / _ \/ __/ __| |  __| | |/ _` | '_ \| __| #
 * #| |____| |_) | | (__  | |_) | (_) \__ \__ \ | |    | | (_| | | | | |_  #
 * #|______| .__/|_|\___| |____/ \___/|___/___/ |_|    |_|\__, |_| |_|\__| #
 * #      | |                                             __/ |            #
 * #      |_|                                            |___/             #
 * # _                    _           _                         _          #
 * #| |                  | |         | |                       | |         #
 * #| |     ___  __ _  __| | ___ _ __| |__   ___   __ _ _ __ __| |         #
 * #| |    / _ \/ _` |/ _` |/ _ \ '__| '_ \ / _ \ / _` | '__/ _` |         #
 * #| |___|  __/ (_| | (_| |  __/ |  | |_) | (_) | (_| | | | (_| |         #
 * #|______\___|\__,_|\__,_|\___|_|  |_.__/ \___/ \__,_|_|  \__,_|         #
 * #                                                                       #
 * #                                                                       #
 * #  _____           _                                                    #
 * # / ____|         | |                                                   #
 * #| (___  _   _ ___| |_ ___ _ __ ___                                     #
 * # \___ \| | | / __| __/ _ \ '_ ` _ \                                    #
 * # ____) | |_| \__ \ ||  __/ | | | | |                                   #
 * #|_____/ \__, |___/\__\___|_| |_| |_|                                   #
 * #         __/ |                                                         #
 * #        |___/                                                          #
 * #                                                                       #
 * #     Author: John Mayhem                                               #
 * #     Copyright: (c) 2023                                               #
 * #     Description: This crap makes a Leaderboard for Dota2's EBF        #
 * #                                                                       #
 * #########################################################################
 ***************************************************************************/

#include "Parser.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

// Function to format and save player data
void FormatAndSavePlayerData(const std::string& inputJsonPath, const std::string& formattedOutputPath) {
    // Open and parse the JSON file
    std::ifstream inputFile(inputJsonPath);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open input JSON file: " << inputJsonPath << std::endl;
        return;
    }

    nlohmann::json jsonData;
    inputFile >> jsonData;
    inputFile.close();  // Close the input file after reading

    // Open the output file
    std::ofstream outputFile(formattedOutputPath);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to create output file: " << formattedOutputPath << std::endl;
        return;
    }

    // Iterate through each player's data and write to the output file
    for (auto& [key, value] : jsonData.items()) {
        // Make sure the JSON structure is as expected and the types are correct
        auto& playerData = value;
        int mmr = playerData["mmr"].get<int>();  // Ensure the data type matches
        int plays = playerData["plays"].get<int>();  // your actual data
        int wins = playerData["wins"].get<int>();

        outputFile << key << ", " << mmr << ", " << plays << ", " << wins << std::endl;
    }

    outputFile.close();  // Close the output file
    std::cout << "> Data formatted and saved to " << formattedOutputPath << std::endl;
    std::cout << "\n";
}
