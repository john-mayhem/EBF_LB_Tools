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

#include "LeaderboardManager.h"
#include <fstream>
#include <sstream>
#include <algorithm> // for std::sort
#include <iostream> // for standard I/O operations

// Constructor implementation
LeaderboardManager::LeaderboardManager(const std::string& inputFilePath) : inputFilePath(inputFilePath) {}

bool LeaderboardManager::loadDataFromFile() {
    std::ifstream inputFile(inputFilePath);

    if (!inputFile.is_open()) {
        std::cerr << "!Error: Unable to open file: " << inputFilePath << std::endl;
        return false; // File opening failed
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        PlayerData player;

        // Assuming the structure is: steamID,mmr,plays,wins
        std::getline(iss, player.steamID, ',');
        iss >> player.mmr;
        iss.ignore(); // skip the comma
        iss >> player.plays;
        iss.ignore(); // skip the comma
        iss >> player.wins;

        players.push_back(player);
    }

    inputFile.close();
    return true; // Data loaded successfully
}

void LeaderboardManager::createMMRLeaderboard(const std::string& outputleaderboardmmrPath) {
    // Check if data is loaded
    if (players.empty()) {
        std::cerr << "!Error: No player data loaded. Cannot create leaderboard." << std::endl;
        return;
    }

    // Sort players by MMR in descending order
    std::sort(players.begin(), players.end(), [](const PlayerData& a, const PlayerData& b) -> bool {
        return a.mmr > b.mmr; // Sort in descending order by MMR
        });

    std::ofstream outputFile(outputleaderboardmmrPath);
    if (!outputFile.is_open()) {
        std::cerr << "!Error: Unable to open file for writing: " << outputleaderboardmmrPath << std::endl;
        return; // File opening for writing failed
    }

    // Counter for recording only the top 100 or less
    int counter = 0;

    // Iterate through the sorted list, filter as necessary, and write the top players to the file
    for (const auto& player : players) {
        // Filter out players with specific conditions
        if (player.steamID != "0" && player.mmr >= 5240) {
            outputFile << player.steamID << ", " << player.mmr << ", " << player.plays << ", " << player.wins << "\n";

            // Increment the counter
            counter++;

            // If we've written 100 players, break out of the loop
            if (counter >= 100) {
                break;
            }
        }
    }

    // Close the file
    outputFile.close();

    // Log success message
    std::cout << "> Leaderboard created successfully with " << counter << " players." << std::endl;
    std::cout << "\n";
}

void LeaderboardManager::createWinRateLeaderboard(const std::string& outputPath) {
    // Check if data is loaded
    if (players.empty()) {
        std::cerr << "!Error: No player data loaded. Cannot create leaderboard." << std::endl;
        return;
    }

    // Sort players by win rate in descending order. Win rate is calculated as wins/plays.
    std::sort(players.begin(), players.end(), [](const PlayerData& a, const PlayerData& b) -> bool {
        double winRateA = (a.plays == 0) ? 0 : static_cast<double>(a.wins) / a.plays;
        double winRateB = (b.plays == 0) ? 0 : static_cast<double>(b.wins) / b.plays;
        return winRateA > winRateB; // Sort in descending order by win rate
        });

    std::ofstream outputFile(outputPath);
    if (!outputFile.is_open()) {
        std::cerr << "!Error: Unable to open file for writing: " << outputPath << std::endl;
        return; // File opening for writing failed
    }

    // Counter for recording only the top 100 or less
    int counter = 0;

    // Iterate through the sorted list, filter as necessary, and write the top players to the file
    for (const auto& player : players) {
        // Filter out players with specific conditions
        if (player.steamID != "0" && player.wins >= 50) { // Changed condition for win rate leaderboard
            outputFile << player.steamID << ", " << player.mmr << ", " << player.plays << ", " << player.wins << "\n";

            // Increment the counter
            counter++;

            // If we've written 100 players, break out of the loop
            if (counter >= 100) {
                break;
            }
        }
    }

    // Close the file
    outputFile.close();

    // Log success message
    std::cout << "> Win rate leaderboard created successfully with " << counter << " players." << std::endl;
    std::cout << "\n";
}
