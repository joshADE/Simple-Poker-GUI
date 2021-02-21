#pragma once
#include <string>
using namespace std;
class Player
{
private:
	string name, userName, password;
	int score;
public:
	static string RANK_FILE_FORMAT;
	static string RANK_FILE_DELIM;
	static string SCORE_FILE_FORMAT;
	static string SCORE_FILE_DELIM;
	Player();
	bool setScore(int Score);
	Player(string, string, string, int);
	~Player();
	
	string getName();
	string getUserName();
	int getScore();
	string getPassWord();
	string getRankDetails();

	static int compare(const void * p1, const void * p2);

	string getRankFileContent();
	string getScoreFileContent();

	void setPassWord(string);
	void setUserName(string);
	void setName(string);
	
	bool authenticate(string password);

	string toString();

};

