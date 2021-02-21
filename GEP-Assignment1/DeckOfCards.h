#pragma once
#include "Card.h"
using namespace std;
class DeckOfCards
{
public:
	const static int DEFUALT_SIZE = Card::FACE_SIZE * Card::SUIT_SIZE;
	Card deck[DEFUALT_SIZE];
	int currentCard;
	DeckOfCards();
	~DeckOfCards();
	void shuffle();
	void swapCards(Card *card1, Card *card2);
	Card *dealCard();
	bool moreCards();
	string toString();
	/* Sets the currentcard back to zero*/
	void ResetDeck() { currentCard = 0; }
};

