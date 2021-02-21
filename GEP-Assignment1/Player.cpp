#include "Player.h"
string Player::RANK_FILE_DELIM = " | ";
string Player::SCORE_FILE_DELIM = "|";
string Player::RANK_FILE_FORMAT = "%s" + Player::RANK_FILE_DELIM + "%s"; // userName (DELIMINATOR) name - (on a single) line in ranking files
string Player::SCORE_FILE_FORMAT = "%s" + Player::SCORE_FILE_DELIM +"%s" + Player::SCORE_FILE_DELIM + "%d"; // userName (DELIMINATOR) password (DELIMINATOR) score  - (on a single) line in userName.txt files


Player::Player()
{
	name = "NA";
	userName = "NA";
	password = "NA";
	score = 0;
}

Player::Player(string userName, string name , string passWord, int score) {
	this->name = name;
	this->userName = userName;
	this->password = passWord;
	this->score = score;
}


Player::~Player()
{
}

string Player::getName() { return name; }
string Player::getUserName() { return userName; }
int Player::getScore() { return score; }
string Player::getPassWord() { return password; };


int Player::compare(const void * p1, const void * p2) {
	int p1Score = ((Player *)p1)->getScore(), p2Score = ((Player *)p2)->getScore();
	if (p1Score < p2Score)
		return -1;
	if (p1Score == p2Score)
		return 0;
	else
		return 1;
}

string Player::getRankFileContent() {
	string content = this->getUserName();
	content += Player::RANK_FILE_DELIM;
	content += this->getName();
	return content;
}

string Player::getScoreFileContent() {
	/*char *content{};
	sprintf_s(content, 100, Player::SCORE_FILE_FORMAT.data(), userName, password, score);
	*/
	string content = this->getName();
	content += Player::SCORE_FILE_DELIM;
	content += this->getPassWord();
	content += Player::SCORE_FILE_DELIM;
	content += to_string(this->getScore());
	return content;
}
string Player::getRankDetails() {
	string details = "Name: " + this->name;
	details += " Score: " + to_string(this->score);

	return details;
}

void Player::setPassWord(string password) {
	this->password = password;
}
bool Player::setScore(int score) {
	if (score > 0) {
		this->score = score;
		return true;
	}
	return false;
}

bool Player::authenticate(string password) {
	return !(password.compare(this->password));
}

string Player::toString() {
	string data = "Name: " + this->name;
	data += ", Username: " + this->userName;
	data += ", Password: " + this->password;
	data += ", Score: " + to_string(this->score);

	return data;
}

void Player::setName(string newName) {
	this->name = newName;
}

void Player::setUserName(string username) {
	this->userName = username;
}
