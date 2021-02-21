#pragma once
#include <string>
#include <vector>
#include "Player.h"
using namespace std;
class RankManager
{
private:
	string completelUserScoresFileName;
	bool isAuthenticated = false;
	vector<Player*> *playersInMem;
	//int userScore = 0;
	void loadPlayerInfoComplete();

public:
	const string fileDirectory = "Img\\";
	const string userScoreDirectory = "UserScores\\";
	const string fileExtention = ".txt";
	const string rankFileName = "Rankings";
	const string ranksFile = fileDirectory + rankFileName + fileExtention;
	const string partialUserScoresFileName = fileDirectory + userScoreDirectory;
	RankManager();
	
	
	Player * createNewPlayer(string, string, string, int score = 0);
	vector<Player*>* getAllPlayerSorted() {
		sortAndRankPlayers(playersInMem);
		return playersInMem;
	}
	void getRankingsFromFile(vector<Player*>*, string);
	void setRankingsToFile(vector<Player*>*, string);
	Player* userExist(vector<Player*>, string);
	Player * getOrUpdateScoreFromFile(Player *, std::string);
	void swap(Player *, Player *);
	void setOrUpdateScoreToFile(Player *, string);
	void sortAndRankPlayers(vector<Player*> *players);
	void createPlayerFromRankFile(string line, Player * player);
	void createPlayerFromScoreFile(string line, Player * p);
	~RankManager();
};                                                                                                                                                                                                                                                                                                                                                                                                                                    