#pragma once
#include <string>

using namespace std;
class Card
{
public:
	static const int FACE_SIZE = 13;
	const static int SUIT_SIZE = 4;
	enum Suit {CLUBS, DIAMONDS, HEARTS, SPADES};
	enum Face {ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING };
	string face;
	string suit;
	const static string faces[FACE_SIZE];
	const static string suits[SUIT_SIZE];
	Face cardFace;
	Suit cardSuit;
	Card();
	Card(Face face, Suit suit);
	string toString();
	int compareFaces(const Card &c2);
	~Card();
};

