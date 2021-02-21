#include "Card.h"
//const int Card::FACE_SIZE = 4;
//const int Card::SUIT_SIZE = 13;
const string Card::suits[Card::SUIT_SIZE] = { "CLUBS", "DIAMONDS", "HEARTS", "SPADES"};
const string Card::faces[Card::FACE_SIZE] = {"ACE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING" }; 
using namespace std;
Card::Card()
{
	Card(Face::ACE, Suit::CLUBS);
}
Card::Card(Face f, Suit s) {
	face = Card::faces[static_cast<int>(f)];
	suit = Card::suits[static_cast<int>(s)];
	cardFace = f;
	cardSuit = s;
}

Card::~Card()
{
	face = "\0";
	suit = "\0";

}

int findIndex(string face) {
	int size = Card::FACE_SIZE;
	for (int i = 0; i < size; i++) {
		if (face.compare(Card::faces[i]) == 0) {
			return i;
		}
	}
	return -1;

}

int Card::compareFaces(const Card &c2) {

	int c1index = findIndex(this->face);
	
	int c2index = findIndex(c2.face);
	 //printf("%d:%d\n",c1index,c2index);
	if (c1index == c2index) {
		return 0;
	}
	else if (c1index > c2index) {
		return 1;
	}
	else {
		return -1;
	}

}

string Card::toString() {
	return face + " of " + suit;
}
