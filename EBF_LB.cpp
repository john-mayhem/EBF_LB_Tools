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

#include <iostream>
#include "ConfigManager.h"
#include "Downloader.h"
#include "Parser.h"
#include "LeaderboardManager.h"
#include <string>
#include "FileEncoder.h"
#include "Uploader.h"


int main()
{
    std::cout << "#########################################################################\n\n";
    std::cout << "        Epic Boss Fight leaderboards parsing script\n\n";
    std::cout << "        Author: John Mayhem\n\n";
    std::cout << "#########################################################################\n\n\n\n";


    const std::string inputPath = "./data/output.json";
    const std::string outputPath = "./data/formatted_output.txt";
    std::string outputleaderboardmmrPath = "./data/leaderboard_mmr.txt";
    std::string outputWinRateLeaderboardPath = "./data/leaderboard_wr.txt";
    std::string leaderboardMmrPath = "./data/leaderboard_mmr.txt";
    std::string leaderboardWrPath = "./data/leaderboard_wr.txt";
    std::string encodedLeaderboardMmrPath = "./data/lb_1.txt";
    std::string encodedLeaderboardWrPath = "./data/lb_2.txt";


    // Ensure config file exists, create with default if not
    std::cout << "> Checking configs\n\n";
    ensureConfigExists();

    // Load the configuration
    Configuration config = loadConfiguration("config/config.ini");

    // Use the actual values from your configuration here
    RetrieveAndSavePlayerData(config.serverLocation, config.authKey);

    FormatAndSavePlayerData(inputPath, outputPath);

    LeaderboardManager lbManager(outputPath);

    if (lbManager.loadDataFromFile()) {
        lbManager.createMMRLeaderboard(outputleaderboardmmrPath);
        lbManager.createWinRateLeaderboard(outputWinRateLeaderboardPath);
    }
    else {
        std::cerr << "An error occurred. Exiting program." << std::endl;
        return 1; // Exit with an error code
    }

    // Encode the MMR leaderboard file
    FileEncoder::encodeFileToBase64(leaderboardMmrPath, encodedLeaderboardMmrPath);

    // Encode the WinRate leaderboard file
    FileEncoder::encodeFileToBase64(leaderboardWrPath, encodedLeaderboardWrPath);


    try {
        // Assuming your ConfigManager is set up correctly and the config file path is known
        std::string configFilePath = "./config/config.ini";  // Adjust the path as necessary

        // Create the Uploader, which internally loads its configuration
        Uploader uploader(configFilePath);

        // Start the file upload process
        uploader.uploadFiles();

    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;  // Return an error code
    }


    std::cout << "Press Enter to continue...";
    std::cin.get();  // Wait for 'Enter' key press
    return 0;
}
