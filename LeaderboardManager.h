#ifndef LEADERBOARD_MANAGER_H
#define LEADERBOARD_MANAGER_H

#include <string>
#include <vector>

// Define a structure for Player data
struct PlayerData {
    std::string steamID;
    int mmr = 0;    // initialized to 0
    int plays = 0;  // initialized to 0
    int wins = 0;   // initialized to 0
};

class LeaderboardManager {
public:
    LeaderboardManager(const std::string& inputFilePath); // Constructor
    void createMMRLeaderboard(const std::string& outputPath);
    void createWinRateLeaderboard(const std::string& outputPath);
    bool loadDataFromFile();

private:
    std::vector<PlayerData> players;
    std::string inputFilePath;
};

#endif //LEADERBOARD_MANAGER_H
