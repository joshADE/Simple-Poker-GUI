#include "DeckOfCards.h"
#include <time.h>
#include <stdlib.h>
const static int DEFUALT_SIZE = Card::FACE_SIZE * Card::SUIT_SIZE;


DeckOfCards::DeckOfCards()
{
	int facesize = Card::FACE_SIZE;
	currentCard = 0;
	for (int s = 0; s < Card::SUIT_SIZE; s++) {
		for (int f = 0; f < facesize; f++) {
			deck[(s * facesize) + f] = Card(static_cast<Card::Face>(f), static_cast<Card::Suit>(s));
		}
	}
	//shuffle();
}


DeckOfCards::~DeckOfCards()
{
}

void DeckOfCards::shuffle() {
	srand((int)time(NULL));
	int randIndex = 0;
	for (int i = DeckOfCards::DEFUALT_SIZE - 1; i >= 0; i--) {
		randIndex = rand() % DeckOfCards::DEFUALT_SIZE;
		swapCards(&deck[i], &deck[randIndex]);
	}

	/*
	int facesize = Card::FACE_SIZE;
	for (int s = 0; s < Card::SUIT_SIZE; s++) {
		for (int f = 0; f < facesize; f++) {
			swapCards(&deck[(s * facesize) + f], &deck[(DeckOfCards::DEFUALT_SIZE - 1) - ((s * facesize) + f)]);
		}
	}
	*/
	//currentCard = 0;
}


void DeckOfCards::swapCards(Card *card1, Card *card2) {
	Card temp = *card1;
	*card1 = *card2;
	*card2 = temp;
}

Card *DeckOfCards::dealCard() {
	if (!moreCards()) {
		return  nullptr;
	}
	Card *c = new Card(deck[currentCard]);
	currentCard++;
	return c;
}

bool DeckOfCards::moreCards() {
	return (currentCard < DeckOfCards::DEFUALT_SIZE);
}

string DeckOfCards::toString() {
	int facesize = Card::FACE_SIZE;
	string data = "Deck: \n";
	for (int s = 0; s < Card::SUIT_SIZE; s++) {
		for (int f = 0; f < facesize; f++) {
			data += "Card " + to_string((s * facesize) + f + 1) + ": "  + deck[(s * facesize) + f].toString() + "\n";
		}
	}

	return data;

}

