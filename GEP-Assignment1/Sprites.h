#pragma once
#include <vector>
#include "SDL.h"
#include "Card.h"
#include "DeckOfCards.h"
#include <memory>
#include <queue>
#include <list>

using namespace std;

class Sprite
{
protected:
	SDL_Rect m_rSrc; // Rectangle for source pixels.
	SDL_Rect m_rDst; // Rectangle for destination window.

public:
	Sprite(SDL_Rect s, SDL_Rect d) :m_rSrc(s), m_rDst(d) {}
	SDL_Rect* GetSrcP() { return &m_rSrc; }
	SDL_Rect* GetDstP() { return &m_rDst; }
};


// CardSprite


class CardSprite : public Sprite {
protected:
	SDL_Rect m_rSrcBack;
	double
		m_dSpeed,	// Speed in pixels to move per change.
		m_dX, m_dY,
		m_destX, m_destY;
	Card * m_pCard;
	int m_iFrame = 0,	// Frame counter.
		m_iFrameMax = 60;	// Used to determing how fast the scroller moves. The number of frames that each sprites should wait before moving


	void checkIfAtDestination(double, double);
	
public:
	enum state { dealing, swapping , none};
	
	bool m_bIsFaceUp = false;
	CardSprite(Card *, bool, state, SDL_Rect, SDL_Rect, SDL_Rect, double, double, double);
	CardSprite(const CardSprite &);
	~CardSprite();
	bool isAtToPosition() { return (m_rDst.x == (int)m_destX && m_rDst.y == (int)m_destY); }
	void Update();
	void Render();
	void SetToPosition(int newX, int newY); // sets the postion card will be moving to each frame when it renders
	void SetPosition(int x, int y); // destinaton rect controls the current position and it is protected so not assessible 
	void changeState(state);
	state getCurrentState() { return m_eCurrentState; }
	bool Moving() { return (m_dX != 0 && m_destY != 0); }
	Card* GetCard() { return m_pCard; }
	

protected:
	state m_eCurrentState = state::none; // when the state changes to dealing through changestate() method, the speed is lowered to create a dealing like animation, which is different from when user swaps 
};




class Hand {
protected:
	int m_iNumberOfCards,
		m_X,
		m_Y,
		m_iPaddingX = 50; // must be >40

	vector<CardSprite *> m_vCards;
	vector<SDL_Rect> slots;
	SDL_Color m_cSlotColor = {255, 255, 255, 255};
public:
	Hand(int, int, int);
	void Update();
	 void Render();
	void RenderCards();
	virtual ~Hand();
	bool ValidSlot(int slot) { return slot >= 0 && slot < m_iNumberOfCards; }
	SDL_Rect GetRectForSlot(int);
	bool IsSlotFilled(int slot) { return m_vCards[slot] != nullptr; }
	bool SetCardAtSlot(int slot, CardSprite *card) { if (!ValidSlot(slot) || IsSlotFilled(slot)) return false; else { m_vCards[slot] = card; card = nullptr; return true; } }
	int AddCard(CardSprite *);
	void SetFaceUp(bool);
	CardSprite * GetCardForSlot(int);
	CardSprite * RemoveCard(int);
	int GetSlotCount() { return m_iNumberOfCards; }
	void SetColor(SDL_Color);
	/* Gets A copy of all the cards that are in the hand*/
	vector<Card> GetCopyOfAllCards();
};
/* A type of hand that can keep track of cards that it recieves and the cards it gives out when swapping to prevent swapping of the same cards*/
class SwappableHand : public Hand {

protected:
	vector<Card*> m_vSwappedCards, m_vSwappedCardsIn;
	// static const int MAX_SWAPS = 3; // for keeping track of swaps

public:
	SwappableHand(int x, int y, int numberofcards)
		:Hand(x, y, numberofcards) {
		m_cSlotColor = { 255, 0, 0, 255 };
	}
	bool SwapCards(SwappableHand *, int, int);
	void ResetSwaps();
	virtual ~SwappableHand();
};


class DeckSprite : public Sprite {
protected:
	DeckOfCards m_dDeck;
	int m_iDealingDelayFrames = 60;	// Delay in number of frames that it deals the cards in
	bool m_bIsFaceUp = false;
	SDL_Rect GetCardFaceUp(Card *);
	SDL_Rect GetCardFaceDown(Card *);
	int m_X,
		m_Y,
		m_iCardWidth,
		m_iCardHeight,
		m_iCardSpacingX = 7,
		m_iFrame = 0,
		m_iFrameMax = 600;
	bool m_bIsShuffling = false;

	queue<CardSprite*, deque<CardSprite*> > m_qCardQueue;
	list<CardSprite*> m_lCardList;
	CardSprite* temp;

	void AddNewCardToQueue(int);

public:
	DeckSprite(int, int, int, bool, bool);
	void Update();
	void Render();
	~DeckSprite();
	void Shuffle();
	void AnimateShuffling();
	void SetLocationOfCards();
	CardSprite * GetSpriteForCard(Card*);
	void DealCard(Hand *);
	

};
