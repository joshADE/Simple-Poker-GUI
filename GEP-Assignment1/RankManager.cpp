#include "RankManager.h"
#include <iostream>
#include <iomanip>
#include <fstream> 


RankManager::RankManager()
{
	this->playersInMem = new vector<Player*>();
	loadPlayerInfoComplete();
}

/* Loads the list of players that are maintained by the RankManager from the Rankings File.
	This is called immediately after the RankManager is created. The path to the Ranking File is specified by the ranksFile member. */
void RankManager::loadPlayerInfoComplete() {
	int i = 0;
	getRankingsFromFile(playersInMem, ranksFile);
	for (Player *p : *playersInMem) {
		completelUserScoresFileName = partialUserScoresFileName + p->getUserName() + fileExtention;
		//cout << endl << "Loaded: " << completelUserScoresFileName << "from storage" << endl;

		getOrUpdateScoreFromFile(p, completelUserScoresFileName);
		//cout << endl << p.toString() << endl;

	}
}

/* Creates a new player object, and adds the player to the list of players that are in the ranking. 
	Updated the player rankings file.
	Returns a reference to player. */
Player * RankManager::createNewPlayer(string name, string userName, string passWord, int score) {

	Player *player = new Player(userName, name, passWord, score);
	playersInMem->push_back(player); // add the player to the vector of players
	sortAndRankPlayers(playersInMem);
	setRankingsToFile(playersInMem, ranksFile); // update the rankings file, game will take care of updating the score file as needed
	return player;
};

/* Creates and adds players details from a rankings file (filename) into a vector of players. */
void RankManager::getRankingsFromFile(vector<Player*> *players, std::string filename) {
	ifstream stream(filename);

	std::string contents, line("\n");
	string name, userName;

	while (std::getline(stream, line))
	{
		if (line.size() > 0) {
			userName = line.substr(0, line.find(Player::RANK_FILE_DELIM));
			name = line.substr(line.find(Player::RANK_FILE_DELIM) + Player::RANK_FILE_DELIM.size(), line.size());
			players->push_back(new Player(userName, name, "****", 0));
		}
	}

	stream.close();
}

/* Given a vector of players, overrites the file specified by filename with player ranking details. */
void RankManager::setRankingsToFile(vector<Player*> *players, std::string filename) {
	ofstream stream;
	stream.open(filename);

	for (Player* p : *players)
	{
		stream << endl << p->getRankFileContent() << endl;
	}

	stream.close();
}

/* Checks if a player with a specific username exist within the vector players. It returns a reference to the user if they exist,
	otherwise it returns nullptr. */
Player* RankManager::userExist(vector<Player*> players, string username) {
	for (Player *p : players) {
		if (username.compare(p->getUserName()) == 0) {
			return p;
		}
	}
	return nullptr;
}

/* Swaps two players*/
void RankManager::swap(Player *p1, Player * p2) {
	Player temp;
	temp = *p1;
	*p1 = *p2;
	*p2 = temp;
}

/* Reads a player score details from a score file (filename) and updates the player score info into the player that is passed in. */
Player * RankManager::getOrUpdateScoreFromFile(Player *player, std::string filename) {

	ifstream stream(filename);

	std::string contents, line("\n");
	string name, password;
	//int score, firstDelim, secondDelim, temp;
	//Player * newPlayer = new Player;
	//newPlayer->setUserName(player->getUserName());
	while (std::getline(stream, line))
	{
		if (line.size() > 0) {
			//RankManager::createPlayerFromScoreFile(line, newPlayer);
			RankManager::createPlayerFromScoreFile(line, player);
		}
	}
	stream.close();
	//cout <<endl << newPlayer->toString()<< endl;
	//return newPlayer;
	return player;
}

/* Given a file(filename), overrites the content with details from the player object*/
void RankManager::setOrUpdateScoreToFile(Player *player, std::string filename) {
	ofstream stream;

	stream.open(filename);
	stream << endl << player->getScoreFileContent() << endl;

	stream.close();
}

/* Given a player that has a username/name, updates the player object with score info from a line that has been retreived from a player score file*/
void RankManager::createPlayerFromScoreFile(string line, Player * p) {
	int firstDelimLoc, secondDelimLoc;
	int size = Player::SCORE_FILE_DELIM.size();

	string delimiter = Player::SCORE_FILE_DELIM;

	string name, password, scores;
	int score = 0;
	size_t pos_start = 0, pos_end, delim_len = Player::SCORE_FILE_DELIM.size();

	if ((pos_end = line.find(delimiter, pos_start)) != string::npos) {
		name = line.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
	}

	if ((pos_end = line.find(delimiter, pos_start)) != string::npos) {
		password = line.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
	}


	//if ((pos_end = line.find(delimiter, pos_start)) != string::npos) {
	scores = line.substr(pos_start, line.size() - pos_start);
	//printf("\n%d\n", scores);
	score = stoi(scores);
	pos_start = pos_end + delim_len;
	//}


	p->setName(name);
	p->setPassWord(password);
	p->setScore(score);

}
/* Given a player with no details, , updates the player object with username/name info from a line that has been retreived from a players rank file*/
void RankManager::createPlayerFromRankFile(string line, Player * player) {
	//Not implemented
}
/* Sorts a vecor of players in descending order of player score. */
void RankManager::sortAndRankPlayers(vector<Player*> *players) {
	//Player * plys = players->data();
	const int SIZE = players->size();
	int maxIndex = 0;
	for (int i = 0; i < SIZE - 1; i++) {
		maxIndex = i;
		for (int j = i + 1; j < SIZE; j++) {
			if (players->at(i)->getScore() < players->at(j)->getScore()) {
				maxIndex = j;
			}
		}

		if (maxIndex != i) {

			iter_swap(players->begin() + i, players->begin() + maxIndex);
		}

	}

}


RankManager::~RankManager()
{
	for (int i = 0; i < (int)playersInMem->size(); i++)
	{
		delete playersInMem->at(i);
		playersInMem->at(i) = nullptr;
	}
	playersInMem->clear();
	playersInMem->shrink_to_fit();
}
