#pragma once
#include "DeckOfCards.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "Card.h"

using namespace std;

int containsPair(Card[], int);
int containsTwoPair(Card[], int);

enum Ranking { ROYALFLUSH = 1, STRAIGHTFLUSH, FOUROFAKIND, FULLHOUSE, FLUSH, STRAIGHT, THREEOFAKIND, TWOPAIR, PAIR, HIGHCARD, NORANK };

void printHand(Card hand[], int size) {
	for (int j = 0; j < size; j++) {

		cout << hand[j].toString() << endl;

	}
}

int findIndex(string s, const string stringArray[], int size) {
	for (int i = 0; i < size; i++) {
		if (s.compare(stringArray[i]) == 0) {
			return i;
		}
	}
	return -1;

}

int testConditionFaceIncreasing(Card *card1, Card * card2) {
	int card1Index = findIndex(card1->face, Card::faces, Card::FACE_SIZE);
	int card2Index = findIndex(card2->face, Card::faces, Card::FACE_SIZE);

	//cout << endl << "C1Index: " << card1Index << ", C2Index: " << card2Index << endl;

	if (card1Index == -1 || card2Index == -1) {
		return -1;
	}
	else if (card1Index > card2Index) {
		return 1;
	}
	else {
		return 0;

	}

}

int testConditionFaceDecreasing(Card *card1, Card * card2) {
	return testConditionFaceIncreasing(card2, card1);
}

int testConditionSuitIncreasing(Card *card1, Card * card2) {
	int card1Index = findIndex(card1->suit, Card::suits, Card::SUIT_SIZE);
	int card2Index = findIndex(card2->suit, Card::suits, Card::SUIT_SIZE);

	if (card1Index == -1 || card2Index == -1) {
		return -1;
	}
	else if (card1Index > card2Index) {
		return 1;
	}
	else {
		return 0;
	}

}

int testConditionSuitDecreasing(Card *card1, Card * card2) {
	return testConditionSuitIncreasing(card2, card1);
}
void swapCards(Card * card1, Card * card2) {
	Card temp = *card1;
	*card1 = *card2;
	*card2 = temp;
}

void sort(Card * hand, int size, int(*fun_condition)(Card*, Card*)) {
	int compindex, res;


	for (int i = 0; i < size - 1; i++) {
		compindex = i;
		for (int j = i + 1; j < size; j++) {
			// perform selection sort comparison
			if (res = ((*fun_condition)(&hand[compindex], &hand[j])) == 1) {
				compindex = j;
			}
			//cout << to_string(res) << endl;

		}

		// swap values
		if (compindex != i) {
			Card temp = hand[i];
			hand[i] = hand[compindex];
			hand[compindex] = temp;
		}

	}

}

/*Sorts a hand by faces in increasing order. The order is determined by the order in the faces string array the Card class*/
void sortFaces(Card hand[], int size) {
	sort(hand, size, (&testConditionFaceIncreasing));
}

/*Sorts a hand by suits in increasing order. The order is determined by the order in the suits string array in the Card class*/
void sortSuits(Card hand[], int size) {
	sort(hand, size, (&testConditionSuitIncreasing));
}


/* This function returns a positive number representing the index of the last face if the
	hand has n (numberOfSameFaces) number of cards with the same faces, otherwise it returns 0. It will also sort the hand based on the increasing face*/
int consecutiveFace(Card hand[], int size, int numberOfSameFaces) {
	int c = 1;
	int p = 1;

	sortFaces(hand, size); // sort faces sorts the hands faces by ascending order
	vector<int> consecFaces;
	consecFaces.push_back(1);
	Card current = hand[0];
	for (int i = 1; i < size; i++) {

		//cout << endl << to_string(p) << "Current Face: " << current.face << ", hand[i].Face: " << hand[i].face << endl;
		if ((p = current.face.compare(hand[i].face)) == 0) {
			c = ++consecFaces.at(consecFaces.size() - 1);


			if (c >= numberOfSameFaces)
				return i;
		}
		else {

			consecFaces.push_back(1);
			current = hand[i];
		}
	}

	return 0;

}

/* This function returns a positive number representing the index of the last suit if the
	hand has n number of card with the same suits, otherwise it returns 0. It also sorts the hand by suit*/
int consecutiveSuits(Card hand[], int size, int numberOfSameSuits) {
	int c = 1;
	//Card *hand2 = 
	int p = 1;
	sortSuits(hand, size);
	vector<int> consecSuits;
	consecSuits.push_back(1);
	Card current = hand[0];
	for (int i = 1; i < size; i++) {

		//cout << endl << to_string(p) << "Current Suit: " << current.suit << ", hand[i].Suit: " << hand[i].suit << endl;
		if ((p = current.suit.compare(hand[i].suit)) == 0) {
			c = ++consecSuits.at(consecSuits.size() - 1);


			if (c >= numberOfSameSuits)
				return i;
		}
		else {

			consecSuits.push_back(1);
			current = hand[i];
		}
	}
	return 0;

}

/* Checks if a hand has 3 cards with the same face and sorts the hand in ascending order.
 If the hand has a three of a kind, it return the index of the last card in the three of a kind
 otherwise it returns 0 */
int threeOfAKind(Card hand[], int size) {
	return consecutiveFace(hand, size, 3);
}
/* Checks if a hand has 4 cards with the same face and sorts the hand in ascending order.
 If the hand has a four of a kind, it return the index of the last card in the four of a kind
 otherwise it returns 0 */
int fourOfAKind(Card *hand, int size) {
	return consecutiveFace(hand, size, 4);
}

/* Determins if a hand contains a straight. If it does, it returns the index of the last card in the straight,
otherwise it returns 0.*/
int straight(Card hand[], int size) {

	int c = 0;
	int i;
	//sortFaces(hand, size); // sortFaces sorts the hand by increasing order of faces by defualt
	sort(hand, size, (&testConditionFaceIncreasing));
	Card current = hand[0];

	int position = findIndex(current.face, Card::faces, Card::FACE_SIZE), next = 0;
	//cout << endl << "Position: " << to_string(position) << ", Next: " << to_string(next) << endl;

	for (i = 1; i < size; i++) {

		//cout << endl << hand[i].toString() << endl;

		if ((position - (next = findIndex(hand[i].face, Card::faces, Card::FACE_SIZE))) != -1) {
			return 0;

		}
		current = hand[i];
		//cout << endl << "Position: " << to_string(position) << ", Next: " << to_string(next) << endl;
		position = next;
	}

	return i;

}

/* Determins if a hand contains a flush. If it does, it returns the index of the last card in the flush,
otherwise it returns 0.*/
int flush(Card hand[], int size) {
	return consecutiveSuits(hand, size, 5);
}

/* Determins if a hand contains a straight flush. If it does, it returns the index of the last card in the straight flush,
otherwise it returns 0.*/
int straightflush(Card hand[], int size) {
	int val = std::_Min_value(straight(hand, size), flush(hand, size));
	return val;
}
/* Determins if a hand contains a royal flush. If it does it returns the size - 1,
otherwise it returns 0.*/
int royalFlush(Card hand[], int size) {

	int isFlush = flush(hand, size);

	if (isFlush == 0) {
		return isFlush;
	}


	sortFaces(hand, size);
	if (
		hand[0].face.compare(Card::faces[Card::Face::ACE]) == 0 &&
		hand[1].face.compare(Card::faces[Card::Face::TEN]) == 0 &&
		hand[2].face.compare(Card::faces[Card::Face::JACK]) == 0 &&
		hand[3].face.compare(Card::faces[Card::Face::QUEEN]) == 0 &&
		hand[4].face.compare(Card::faces[Card::Face::KING]) == 0
		) {
		return size - 1;

	}
	return 0;


}


/* The function returns a number(val) between a max and a min inclusively, and wraps the value between the bounds
so if it is less than the min, it counts down from the max, and if it is greater than max, it counts up from the min*/
int clampWrapInclusive(int min, int max, int val) {
	int N = abs(max - min), newval = 0;
	if (val < min)
		newval = (max + 1) - ((abs(min - val) % (N + 1) != 0) ? abs(min - val) % (N + 1) : (N + 1));
	else if (val > max)
		newval = (min - 1) + ((abs(max - val) % (N + 1) != 0) ? abs(max - val) % (N + 1) : (N + 1));
	else
		newval = val;
	//cout << to_string(newval) << endl;
	return newval;
}

/* Copies the contents of originalHand to newHand starting from and index in the originalHand.
It starts adding cards from the beginning of the originalHand once the index reaches end of the original hand*/
void copyHand(Card originalHand[], int originalSize, Card newHand[], int newSize, int index)
{
	if (originalSize <= 0 || originalSize <= newSize)
		return;
	int lastindex = newSize + index;
	for (int i = index; i < lastindex; i++) {
		// the index that the last of the threeofakind could be anywhere in the sorted deck
		// once it has been found, create a newhand of the 2reamaining cards in the hand
		//cout << endl << to_string(i) << endl;
		newHand[i - index] = originalHand[clampWrapInclusive(0, originalSize - 1, i)];
	}

}
/* Check if a hand contains a fullhouse. It returns the index of the last card
in the three of a king while also sortig the hand in ascending order of face. Otherwise it returns 0 if the hand
doesn't contain a fullhouse */
int fullHouse(Card hand[], int size) {

	if (!fourOfAKind(hand, size)) {
		int ind = 0;

		if (ind = threeOfAKind(hand, size)) {
			const int newsize = size - 3;
			if (newsize <= 1) return 0;
			Card *newhand = new Card[newsize];
			int handindex;
			ind; // threeOfAKind returns the index of the last card + 1 of the sorted hand 
					//   that is part of the three cards su must substract one

			for (int i = newsize - 1; i >= 0; i--) {
				// the index that the last of the threeofakind could be anywhere in the sorted deck
				// once it has been found, create a newhand of the 2reamaining cards in the hand
				handindex = (ind + 1) - newsize - 1 - (newsize - i);
				//cout << endl << to_string(handindex) << endl;
				newhand[i] = hand[clampWrapInclusive(0, size - 1, handindex)];
			}

			int resultOfContainsPair = containsPair(newhand, newsize); // cointains pair return a positive number if it contains a pair
			delete[] newhand;
			if (resultOfContainsPair) { // any non-zero number evaluates to true
				return ind; // return the index of the last card that was found in the three of  a kind
			}
		}
	}
	return 0;
}
/* Check if a hand contains a pair. It returns the index of the last card
in the pair while also sortig the hand in ascending order of face. Otherwise it returns 0 if the hand
doesn't contain a pair */
int containsPair(Card hand[], int size) {
	return consecutiveFace(hand, size, 2);
}
/*
Checks if a hand containts two pairs while also sorting the hand in ascending order of face.
It returns the index of the second card in the lower value pair. Otherwise it will return 0
*/
int containsTwoPair(Card hand[], int size) {

	int ind = 0;
	if (ind = containsPair(hand, size)) {
		int newsize = size - 2;
		if (newsize <= 1) return 0;
		Card * newHand = new Card[newsize];

		copyHand(hand, size, newHand, newsize, ind);
		vector<string> facesV;

		for (int i = 0; i < newsize; i++) {
			if (facesV.end() != find(facesV.begin(), facesV.end(), newHand[i].face)) {
				delete[] newHand;
				// rather than returning the index of the pair in the new hand, return index of second
				// card in the first pair that was found in the original sorted hand 
				// (lower value pair) 
				return ind;
			}


			facesV.push_back(newHand[i].face);
		}
		delete[] newHand;
	}

	return 0;

}

/* Not used */
int compareHands(Card  hand1[], Card hand2[], int sizes, int startIndex, int(*fun_condition)(Card*, Card*), int *indexWhereMisMatch) {
	indexWhereMisMatch = 0;

	sort(hand1, sizes, fun_condition);
	sort(hand2, sizes, fun_condition);
	int hand1Face, hand2Face;
	for (int i = startIndex; i < sizes; i++) {
		hand1Face = findIndex(hand1[i].face, Card::faces, Card::FACE_SIZE);
		hand2Face = findIndex(hand2[i].face, Card::faces, Card::FACE_SIZE);
		if (hand1Face > hand2Face) {
			return 1;
		}
		if (hand2Face > hand1Face) {
			return -1;
		}
	}
	return 0;
}

/* Text description of all the ranking/strengths */
string readStrengthDescription(int value) {
	string result;
	switch (value) {
	case ROYALFLUSH:
		result = "royal flush";
		break;
	case STRAIGHTFLUSH:
		result = "straight flush";
		break;
	case FOUROFAKIND:
		result = "four of a kind";
		break;
	case FULLHOUSE:
		result = "full house";
		break;
	case FLUSH:
		result = "flush";
		break;
	case STRAIGHT:
		result = "straight";
		break;
	case THREEOFAKIND:
		result = "three of a kind";
		break;
	case TWOPAIR:
		result = "two pairs";
		break;
	case PAIR:
		result = "pair";
		break;
	default:
		result = "nothing";
		break;
	}
	return result;
}

/* Evaluates the strength of the hand*/
int describeHand(Card hand[], int size) {

	if (royalFlush(hand, size)) { return ROYALFLUSH; }
	else if (straightflush(hand, size)) { return STRAIGHTFLUSH; }
	else if (fourOfAKind(hand, size)) { return FOUROFAKIND; }
	else if (fullHouse(hand, size)) { return FULLHOUSE; }
	else if (flush(hand, size)) { return FLUSH; }
	else if (straight(hand, size)) { return STRAIGHT; }
	else if (threeOfAKind(hand, size)) { return THREEOFAKIND; }
	else if (containsTwoPair(hand, size)) { return TWOPAIR; }
	else if (containsPair(hand, size)) { return PAIR; }
	return NORANK;
}

/* Test the hand againts all the strentghs/ranking and prints out whether it meets the conditions*/
void checkHand(Card hand[], int size) {
	cout << "Does it have a pair:" << endl;
	cout << ((containsPair(hand, size)) ? "true" : "false") << endl << endl;

	cout << "Does it have two pairs:" << endl;
	cout << ((containsTwoPair(hand, size)) ? "true" : "false") << endl << endl;

	cout << "Does it have a three of a kind:" << endl;
	cout << ((threeOfAKind(hand, size)) ? "true" : "false") << endl << endl;

	cout << "Does it have a straight:" << endl;
	cout << ((straight(hand, size)) ? "true" : "false") << endl << endl;

	cout << "Does it have a flush:" << endl;
	cout << ((flush(hand, size)) ? "true" : "false") << endl << endl;

	cout << "Does it have a full house:" << endl;
	cout << ((fullHouse(hand, size)) ? "true" : "false") << endl << endl;

	cout << "Does it have a four Of A Kind:" << endl;
	cout << ((fourOfAKind(hand, size)) ? "true" : "false") << endl << endl;

	cout << "Does it have a straight flush:" << endl;
	cout << ((straightflush(hand, size)) ? "true" : "false") << endl << endl;

	cout << "Does it have a royal Flush:" << endl;
	cout << ((royalFlush(hand, size)) ? "true" : "false") << endl << endl;


}



/* Compares the hand of p1 and p2. If p1 has a stronger hand than p2, p1 wins and the function returns 1.
If p2 hand is stronger -1 is returned. If the have the same strength, the faces of the cards are checked to detetermine the winner
p1 and p2 are passed into the vector hands with p1 at index 0 and p2 at index 1. Their strengths/ranking are in the handStr vector*/
int compareHand(vector<int> *handStr, vector<Card*> *hands, int handsize) {

	int strength1 = handStr->at(0), strenght2 = handStr->at(1);
	if (strength1 != strenght2) {

		if (strength1 < strenght2) // lower strentgh value corresponts to better hand in the enum Ranking 
			return 1;
		else
			return -1;
	}

	Card *hand1 = hands->at(0);
	Card *hand2 = hands->at(1);
	Card *subHand1, *subHand2;
	int locationOfLastCard1;
	int locationOfLastCard2;
	int i, cmp, fifthcard1, fifthcard2, restOfHandCompare,
		locationOfFisrtCard1,
		locationOfFisrtCard2,
		twoofKindCompare,
		threeofKindCompare, newhandsize;
	// if both player get the same strength of hands, check individual cards 
	// to determine winner
	switch (strength1) {
	case ROYALFLUSH:// Royal Flust - tie for both players
		return 0;
		break;
	case STRAIGHTFLUSH:// Straight Flush - sort by face in descending order and compare highest card
		sort(hand1, handsize, &testConditionFaceDecreasing);
		sort(hand2, handsize, &testConditionFaceDecreasing);
		cmp = hand1[0].compareFaces(hand2[0]);
		return cmp;
		break;
	case FOUROFAKIND: // four of a kind - compare the faces in the four of a kind

		locationOfLastCard1 = fourOfAKind(hand1, handsize); // will eiter be position 3 or position 4 in a 5 card hand
		locationOfLastCard2 = fourOfAKind(hand2, handsize);
		cmp = hand1[locationOfLastCard1].compareFaces(hand2[locationOfLastCard2]);


		if (cmp != 0) { // return the greater of the two cards if they are not the same
			return cmp;
		}

		fifthcard1 = clampWrapInclusive(0, handsize - 1, locationOfLastCard1 + 1); // will return the index of the fifth card which is either
																				  // 4(last index) or 0(first index) in 5 cad sorted deck
		fifthcard2 = clampWrapInclusive(0, handsize - 1, locationOfLastCard2 + 1);
		return hand1[fifthcard1].compareFaces(hand2[fifthcard2]);

		break;
	case FULLHOUSE: // fullhouse - compare the face in the three of a kind, then the faces in the pair

		locationOfLastCard1 = fullHouse(hand1, handsize); //gives location of last card in the three of a kind(hand is sorted in ascending order)
		locationOfLastCard2 = fullHouse(hand2, handsize);
		threeofKindCompare = hand1[locationOfLastCard1].compareFaces(hand2[locationOfLastCard2]);
		if (threeofKindCompare != 0) {
			return threeofKindCompare;
		}
		// if the three of  akind of both hands have the same face check and compare their pairs 

		locationOfFisrtCard1 = clampWrapInclusive(0, handsize - 1, locationOfLastCard1 + 1); // first card in the pair is the next card(in a 5 card hand)
		locationOfFisrtCard2 = clampWrapInclusive(0, handsize - 1, locationOfLastCard2 + 1);

		// check the and compare the pair remaining
		return hand1[locationOfFisrtCard1].compareFaces(hand2[locationOfFisrtCard2]);

		break;
	case FLUSH: // the hand that has the card with highest face that isn't in the other hand will win, if it all same faces its a tie
		// sort both hands in descending order
		sort(hand1, handsize, &testConditionFaceDecreasing);
		sort(hand2, handsize, &testConditionFaceDecreasing);

		// go through each card in one hand and compare it with card in the same index of other hand 
		i = 0;
		do {
			cmp = hand1[i].compareFaces(hand2[i]);
			//cout << to_string(cmp);
			i++;
		} while (i < handsize && cmp == 0);
		return cmp;
		break;
	case STRAIGHT: //perform same operation as straight flush
		sort(hand1, handsize, &testConditionFaceDecreasing);
		sort(hand2, handsize, &testConditionFaceDecreasing);
		cmp = hand1[0].compareFaces(hand2[0]);
		return cmp;
		break;
	case THREEOFAKIND:
		// find location of last card in the three cards
		locationOfLastCard1 = threeOfAKind(hand1, handsize);
		locationOfLastCard2 = threeOfAKind(hand2, handsize);
		// compare them
		threeofKindCompare = hand1[locationOfLastCard1].compareFaces(hand2[locationOfLastCard2]);
		if (threeofKindCompare != 0) {
			return threeofKindCompare;
		}

		// three of a kind could be in the middle of a sorted deck of 5 card (1s 2s 2d 2h 3d) vs (2s 2h 2d 3h 4d) 

		// check the and compare the remaining cards
		newhandsize = handsize - 3;
		subHand1 = new Card[newhandsize];
		subHand2 = new Card[newhandsize];
		copyHand(hand1, handsize, subHand1, newhandsize, locationOfLastCard1 + 1);
		copyHand(hand2, handsize, subHand2, newhandsize, locationOfLastCard2 + 1);

		sort(subHand1, newhandsize, &testConditionFaceDecreasing);
		sort(subHand2, newhandsize, &testConditionFaceDecreasing);
		i = 0;
		do {
			cmp = subHand1[i].compareFaces(subHand2[i]);
			i++;
		} while (i < newhandsize && cmp == 0);
		delete[] subHand1;
		delete[] subHand2;
		return cmp;

		break;
	case TWOPAIR:
		// in a two pair hand there is a lower pair (lower face value) and a higher pair
		// first compare the higher pair then the lower pair and then the final card
		// to determine winner in a two pair tie

		// find location of last card in first pair (lower pair)
		locationOfLastCard1 = containsTwoPair(hand1, handsize);
		locationOfLastCard2 = containsTwoPair(hand2, handsize);
		newhandsize = handsize - 2;

		// create a subhand of remaining cards for both hand to find higher pair
		subHand1 = new Card[newhandsize];
		subHand2 = new Card[newhandsize];
		copyHand(hand1, handsize, subHand1, newhandsize, locationOfLastCard1 + 1);
		copyHand(hand2, handsize, subHand2, newhandsize, locationOfLastCard2 + 1);
		sort(subHand1, newhandsize, &testConditionFaceDecreasing);
		sort(subHand2, newhandsize, &testConditionFaceDecreasing);

		// find the location of the higher pair in the new hand and compare their faces
		locationOfFisrtCard1 = containsPair(subHand1, newhandsize);
		locationOfFisrtCard2 = containsPair(subHand2, newhandsize);
		// compare them
		cmp = subHand1[locationOfFisrtCard1].compareFaces(subHand2[locationOfFisrtCard2]);
		// compare the higher pair
		if (cmp != 0) {
			delete[] subHand1;
			delete[] subHand2;
			return cmp;
		}

		//compare lower pair which is in the original hand
		cmp = hand1[locationOfLastCard1].compareFaces(hand2[locationOfLastCard2]);
		if (cmp != 0) {
			delete[] subHand1;
			delete[] subHand2;
			return cmp;
		}
		// compare the last card which is in the subhand
		fifthcard1 = clampWrapInclusive(0, newhandsize - 1, locationOfFisrtCard1 + 1); // will eiter return 2(last index) or 0(first index) in a 3 card subhand
		fifthcard2 = clampWrapInclusive(0, newhandsize - 1, locationOfFisrtCard2 + 1);
		cmp = subHand1[fifthcard1].compareFaces(subHand2[fifthcard2]);
		delete[] subHand1;
		delete[] subHand2;
		return cmp;



	case PAIR: // campare the faces of the pair in both hand, if they are the same compare the remaining cards


		// find location of last card in pair
		locationOfLastCard1 = containsPair(hand1, handsize);
		locationOfLastCard2 = containsPair(hand2, handsize);
		// compare them
		cmp = hand1[locationOfLastCard1].compareFaces(hand2[locationOfLastCard2]);
		if (cmp != 0) {
			return cmp;
		}

		// check the and compare the remaining cards

		newhandsize = handsize - 2;
		subHand1 = new Card[newhandsize];
		subHand2 = new Card[newhandsize];
		copyHand(hand1, handsize, subHand1, newhandsize, locationOfLastCard1 + 1);
		copyHand(hand2, handsize, subHand2, newhandsize, locationOfLastCard2 + 1);

		sort(subHand1, newhandsize, &testConditionFaceDecreasing);
		sort(subHand2, newhandsize, &testConditionFaceDecreasing);
		i = 0;
		do {
			cmp = subHand1[i].compareFaces(subHand2[i]);
			i++;
		} while (i < newhandsize && cmp == 0);
		delete[] subHand1;
		delete[] subHand2;
		return cmp;

		break;
	default: // compare all cards to find the high card in one hands that beat the high in another
		// sort and find highest card face
		sort(hand1, handsize, &testConditionFaceDecreasing);
		sort(hand2, handsize, &testConditionFaceDecreasing);
		i = 0;
		do {
			cmp = hand1[i].compareFaces(hand2[i]);
			//cout << to_string(cmp);
			i++;
		} while (i < handsize && cmp == 0);
		return cmp;
		break;

	}

	return hands->at(0)->compareFaces(*hands->at(1));
}









