#pragma once
#include "DeckOfCards.h"
#include "HandFunctions.h"
#include "MyCommonFunctions.h"
#define CARDS_IN_HAND 5

/* Prompts the user for a number between minIndex and maxIndex (inclusively) and gives an error message if the user doesn't supply a correct index.
It returns the index that the user types in*/
int promptForCardInHand(string message, int minIndex, int maxIndex) {
	string response;
	prompt(message, &response, 1);
	int ind = stoi(response);
	while (ind < minIndex || ind > maxIndex) {
		cout << "Wrong index! There are only " + to_string(abs(maxIndex - minIndex) + 1) << " cards." << endl
			<< "Enter a number between " + to_string(minIndex) << " and " + to_string(maxIndex) << "." << endl;
		prompt("What is the index?", &response, 1);
		ind = stoi(response);
	}
	return ind;
}



/* Prompts the player with the name playerName to change cards in their hand and peroforms the chanage*/
void changeCards(Card hand[], int size, string playerName, Card communityCards[], int communitysize) {
	cout << "Community Cards:" << endl;
	printHand(communityCards, communitysize);
	cout << endl << playerName + "'s Cards:" << endl;
	printHand(hand, size);
	string response, message;

	int n = 0;
	message = "Would you like to change cards " + playerName;
	message += "?\n(Remember you get to change up to 3 cards)";

	bool hasAccepted = accept(message, 'y');
	if (!hasAccepted) {
		return;
	}
	int playerHandIndex, communityHandIndex;
	while (hasAccepted && n < 2) {
		playerHandIndex = promptForCardInHand("Which card in your hand would you like to change?(1 = first, 2 = second, ...)", 1, size) - 1;
		communityHandIndex = promptForCardInHand("Which of the community cards would you like to change with?", 1, communitysize) - 1;
		swapCards(&hand[playerHandIndex], &communityCards[communityHandIndex]);
		// print the new hand and the community hand
		cout << "Community Cards:" << endl;
		printHand(communityCards, communitysize);
		cout << endl << playerName + "'s new Cards:" << endl;
		printHand(hand, size);
		// ask to change again
		message = "Would you like to change cards again?";
		hasAccepted = accept(message, 'y');
		n++;
	}
	// last check
	if (hasAccepted) {
		playerHandIndex = promptForCardInHand("Which card in would you like to change ? ", 1, size) - 1;
		communityHandIndex = promptForCardInHand("Which of the community cards would you like to change with?", 1, communitysize) - 1;
		swapCards(&hand[playerHandIndex], &communityCards[communityHandIndex]);
		// print the new hand and the community hand
		cout << "Community Cards:" << endl;
		printHand(communityCards, communitysize);
		cout << endl << playerName + "'s new Cards:" << endl;
		printHand(hand, size);
	}
}
/**/
void printScoring(int * p1Total, int * p2Total, int * p1Current, int * p2Current) {
	cout << endl << "Current Score:" << endl;
	cout << endl << "P1: " << "Games Won - " + to_string(*p1Total) << ", # of Series Won - " + to_string(*p1Current) << endl;
	cout << endl << "P2: " << "Games Won - " + to_string(*p2Total) << ", # of Series Won - " + to_string(*p2Current) << endl;
}

/*Part of the game where user draws the card and a winner is picked based on the hand*/
int performASeries(DeckOfCards * myDeck, int size, int * p1Score, int * p2Score) {
	cout << "Shuffling the deck..." << endl << endl;

	myDeck->shuffle();


	Card * hand1 = new Card[size];
	Card * hand2 = new Card[size];
	Card * communityCards = new Card[size];

	cout << "Dealing " << size << " cards for P1/P2" << endl;

	for (int i = 0; i < size; i++) {

		hand1[i] = *myDeck->dealCard();
		cout << "P1:" << hand1[i].toString() << endl;
		hand2[i] = *myDeck->dealCard();
		cout << "P2:" << hand2[i].toString() << endl;
	}
	// deal community cards for exchanging 
	for (int i = 0; i < size; i++) {
		communityCards[i] = *myDeck->dealCard();
	}

	// ask p1 to change their card
	cout << endl << endl;
	changeCards(hand1, size, "P1", communityCards, size);
	cout << endl << endl << endl;
	// ask p2 to change their card
	changeCards(hand2, size, "P2", communityCards, size);

	// for testing the individual cards - make sure to comment out the shuffle at the begining of the function
	//hand1[0] = myDeck->deck[20];
	//hand1[1] = myDeck->deck[7];
	//hand1[2] = myDeck->deck[21];
	//hand1[3] = myDeck->deck[34];
	//hand1[4] = myDeck->deck[4];
	//
	//hand2[0] = myDeck->deck[4];
	//hand2[1] = myDeck->deck[7];
	//hand2[2] = myDeck->deck[6];
	//hand2[3] = myDeck->deck[22];
	//hand2[4] = myDeck->deck[35];
	//

	cout << "Final Results:" << endl;
	cout << endl << "P1 hand:" << endl;
	printHand(hand1, size);
	cout << endl << "P2 hand:" << endl;
	printHand(hand2, size);

	vector<Card*> playerhands;
	playerhands.push_back(hand1);
	playerhands.push_back(hand2);

	vector<int> handStr;
	handStr.push_back(describeHand(hand1, size));
	handStr.push_back(describeHand(hand2, size));

	int result = compareHand(&handStr, &playerhands, size);


	// cout << endl <<"RESULT: " + to_string(result) << endl
	// 	<< endl << "P1: P2";

	cout << endl << "P1's hand contained - " + readStrengthDescription(handStr.at(0)) << " " << endl;
	cout << endl << "P2's hand contained - " + readStrengthDescription(handStr.at(1)) << " " << endl;

	cout << "Series Result" << endl;
	if (result > 0)
	{
		*p1Score += 1;
		cout << endl << "P1 wins" << endl;
	}
	else if (result < 0) {
		*p2Score += 1;
		cout << endl << "P2 wins" << endl;
	}
	else {
		cout << endl << "Its a tie" << endl;
	}


	delete[] hand1;
	delete[] hand2;
	delete[] communityCards;
	return result;
}
/* In a game, user must win best of 5 series.
If they win 3/5 series they win a game and a their total score is incremented.
After 3 game wins the winner is declared*/
int performAGame(int * totalP1Score, int * totalP2Score) {

	DeckOfCards myDeck;

	// cout << "Original Deck:" << endl << myDeck.toString() << endl << endl;
	// cout << "Shuffled deck:" << endl << endl;
	// cout << myDeck.toString() << endl << endl;
	// cout << string(50, '#') << endl;

	string rep;
	int p1Score = 0, p2Score = 0, size = CARDS_IN_HAND, result = 0;
	size_t i;
	for (i = 0; i < 5; i++)
	{
		cout << string(50, '#') << endl;
		cout << "Series #" + to_string(i + 1) << endl << endl;
		result = performASeries(&myDeck, size, &p1Score, &p2Score);
		myDeck = DeckOfCards();
		printScoring(totalP1Score, totalP2Score, &p1Score, &p2Score);
		cout << endl << "Enter any key to continue: " << endl;
		getline(std::cin, rep);
		Clear();
		if (p1Score >= 3 || p2Score >= 3) {
			break;
		}
	}

	if (p1Score > p2Score) {
		(*totalP1Score)++;
		cout << "P1 won best of 5 series" << endl;
		return 1;
	}
	else if (p2Score > p1Score) {
		(*totalP2Score)++;
		cout << "P2 won best of 5 series" << endl;
		return -1;
	}
	else {
		cout << "P2 won best of 5 series" << endl;
		return 0;
	}
}
/*
int main() {


	int p1Total = 0, p2Total = 0, winnerScore, gameNumber = 0;

	string winner = "*";
	bool cont = true;
	while(cont){
		while (p1Total < 3 && p2Total < 3)
		{
			cout << string(50, '#') << endl;
			cout << "Game #" + to_string(gameNumber + 1) << endl << endl;
			performAGame(&p1Total, &p2Total);
			cout << endl << endl;
			gameNumber++;
		}
		Clear();
		if (p1Total >= 3) {
			winner = "P1";
			winnerScore = p1Total;
		}
		else if (p2Total >= 3) {
			winner = "P2";
			winnerScore = p2Total;
		}

		cout << string(75, '#') << endl;
		cout << endl << "Game Over. \nFinal Winner: " << winner << " at " + to_string(winnerScore) << " wins" << endl;

		cont = accept("Do you want to continue?", 'y');

			gameNumber = 0;
			p1Total = 0;
			p2Total = 0;
			winnerScore = 0;
			winner = "*";

	}

	// DeckOfCards myDeck;
	// performASeries(&myDeck, 5, &p1Total, &p2Total);

	system("pause");
	return 0;
}


*/