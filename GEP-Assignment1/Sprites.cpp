#include <algorithm> // For min/max.
#include <cmath>	 // For cos/sin.
#include "Sprites.h"
#include "Game.h"
using namespace std;



// CardSprite

CardSprite::CardSprite(Card * card, bool faceUp, state initialstate, SDL_Rect frontsrc, SDL_Rect backsrc, SDL_Rect des, double destinationX, double destinationY, double speed)
	: Sprite(frontsrc, des), m_pCard(card), m_bIsFaceUp(faceUp), m_rSrcBack(backsrc), m_dSpeed(speed) {
	checkIfAtDestination(destinationX, destinationY);
	changeState(initialstate);
}

CardSprite::CardSprite(const CardSprite &other) 
:CardSprite(other.m_pCard, other.m_bIsFaceUp, other.m_eCurrentState, other.m_rSrc, other.m_rSrcBack, other.m_rDst, other.m_destX, other.m_destY, other.m_dSpeed){
}

void CardSprite::checkIfAtDestination(double desX, double desY) {
	if (desX != (m_rDst.x - m_rDst.w / 2.0)) {
		m_destX = (desX - m_rDst.w / 2.0);
		m_dX = m_dSpeed * (desX - m_rDst.x - m_rDst.w / 2);// / fabs(desX - m_rDst.x);
	}
	if (desY != (m_rDst.y - m_rDst.h / 2.0)) {
		m_destY = (desY - m_rDst.h / 2.0);
		m_dY = m_dSpeed * (desY - m_rDst.y - m_rDst.h / 2);// / fabs(desY - m_rDst.y);
	}

}


void CardSprite::Update() {
	
	if (m_dX != 0.0f && (fabs(m_destX - m_rDst.x) <= fabs(m_dX) || fabs(m_destX - m_rDst.x) <= 1)) {
		m_rDst.x = (int)m_destX;
		m_dX = 0;
		cout << "Done X" << endl;
	}
	

	if (m_dY != 0.0f && (fabs(m_destY - m_rDst.y) <= fabs(m_dY) || fabs(m_destY - m_rDst.y) <= 1)) {
		m_rDst.y = (int)m_destY;
		m_dY = 0;
		cout << "Done Y" << endl;

	}
	
	

		
	m_rDst.x += (int)m_dX;
	m_rDst.y += (int)m_dY;
		
	

}


void CardSprite::Render() {
	if (m_bIsFaceUp) {
		SDL_RenderCopy(Game::Instance()->GetRenderer(), Game::Instance()->GetSprTexture(),
			&m_rSrc, &m_rDst);
	}
	else {
		SDL_RenderCopy(Game::Instance()->GetRenderer(), Game::Instance()->GetSprTexture(),
			&m_rSrcBack, &m_rDst);
	}
}


void CardSprite::SetToPosition(int newX, int newY) {

	if (m_eCurrentState == dealing)
		return;

	checkIfAtDestination(newX, newY);
	
}

void CardSprite::SetPosition(int x, int y) {
	m_rDst.x = x - m_rDst.w / 2;
	m_rDst.y = y - m_rDst.h / 2;
}

void CardSprite::changeState(state newstate) {

	if (m_eCurrentState == newstate)
		return;

	

	if (m_eCurrentState != dealing && newstate == dealing) {
		// modify dx and dy to make them slower
		// so the card does not snap to the destination position quickly
		m_dX *= 0.1; 
		m_dY *= 0.1; 

		// Note: The position of the cards on the screen uses integer but dx/dy are doubles
		// If dx/dy are <1 in magnitude and you add/sub them to the integer position the result may sometimes
		// be rounded to give the exact same integer where dx/dy will be treated either as 0/-1
		// depending on if it is postive or negative
		// This will cause problems with the other methods in the class, such as when the cards current position
		// and the position it will be moving to (destX/destY) has a very small difference.
		// This may also happen a lot when the above two lines are executed,
		// so if dy/dy is <1 in magnitude, round it to 1 but keep the sign.
		if (fabs(m_dX) < 1) {
			m_dX = m_dX / fabs(m_dX);
		}
		if (fabs(m_dY) < 1) {
			m_dY = m_dY / fabs(m_dY);
		}
	}
	else {
		checkIfAtDestination(m_destX, m_destY);//reset dX/dY
	}

	m_eCurrentState = newstate;
}

CardSprite::~CardSprite() {
	if (m_pCard != nullptr) {
		//delete m_pCard;
		//m_pCard = nullptr;
	}
}
// End of CardSprite

// Hand

Hand::Hand(int x, int y, int numOfCards)
	: m_X(x), m_Y(y), m_iNumberOfCards(numOfCards) {
	if (m_iNumberOfCards <= 0)
		m_iNumberOfCards = 1;
	m_vCards = vector<CardSprite*>(m_iNumberOfCards);
	slots = vector<SDL_Rect>(m_iNumberOfCards);
	int mid;
	mid = m_iNumberOfCards / 2 * Game::CARD_WIDTH + m_iPaddingX;
	for (int i = 0; i < m_iNumberOfCards; i++) {
		x = (int)m_X - mid;
		y = m_Y + 100; // cards will be 100 units down from position of hand

		x += (i * (Game::CARD_WIDTH + m_iPaddingX));

		slots[i] = { x, y, Game::CARD_WIDTH, Game::CARD_HEIGHT };

	}
}


SDL_Rect Hand::GetRectForSlot(int slot) {
	if (!ValidSlot(slot)) {
		return {0, 0, 0, 0};
	}
	else
		return slots[slot];
}
void Hand::Update() {
	for (int i = 0; i < (int)m_vCards.size(); i++) {
		if (m_vCards[i] != nullptr) {
			m_vCards[i]->Update();
		}
	}	
}

void Hand::SetColor(SDL_Color color) {
	m_cSlotColor = color;
}
void Hand::Render() {
	SDL_Rect rect;

	for (int i = 0; i < (int)m_vCards.size(); i++) {
		SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), m_cSlotColor.r, m_cSlotColor.g, m_cSlotColor.b, m_cSlotColor.a);
		rect = GetRectForSlot(i);
		rect.x -= 6; rect.y -= 6;
		rect.w += 12; rect.h += 12;
		SDL_RenderFillRect(Game::Instance()->GetRenderer(), &rect);
		SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 0, 0, 255);
		rect = GetRectForSlot(i);
		rect.x -= 2; rect.y -= 2;
		rect.w += 4; rect.h += 4;
		SDL_RenderFillRect(Game::Instance()->GetRenderer(), &rect);
		
		/*
		if (m_vCards[i] != nullptr) {
			m_vCards[i]->Render();
		}
		*/
		// cards have to be rendered on top all other slots in this hand
		// as well as other slots in the other hands so
		// the rendercards method for each hand must be called after all hands have been rendered using this method
	}
}

void Hand::RenderCards() {
	for (int i = 0; i < (int)m_vCards.size(); i++) {
		if (m_vCards[i] != nullptr) {
			m_vCards[i]->Render();
		}
	}
}

Hand::~Hand() {
	cout << "Destroying Hand" << endl;
	for (int i = 0; i < (int)m_vCards.size(); i++) {
		m_vCards[i] = nullptr;
	}
	m_vCards.shrink_to_fit();
	//delete slots;
}

int Hand::AddCard(CardSprite * card) {
	for (int i = 0; i < m_iNumberOfCards; i++) {
		if (SetCardAtSlot(i, card)) {
			return i;
		}
	}
	return -1;
}

void Hand::SetFaceUp(bool faceup) {
	for (int i = 0; i < (int)m_vCards.size(); i++) {
		if (m_vCards[i] != nullptr)
			m_vCards[i]->m_bIsFaceUp = faceup;
	}
}

CardSprite * Hand::GetCardForSlot(int slot) {
	if (IsSlotFilled(slot))
		return m_vCards[slot];
	else
		return nullptr;
}

CardSprite * Hand::RemoveCard(int slot) {
	if (IsSlotFilled(slot)) {
		CardSprite * toBeRealeased;
		toBeRealeased = m_vCards[slot];
		m_vCards[slot] = nullptr;
		return toBeRealeased;
	}
	else
		return nullptr;
}
/* This method returns a vector contaning a copy of all the cards in the hand.
  There may not be exacty 5 cards returned. */
vector<Card> Hand::GetCopyOfAllCards() {
	vector<Card> cardcopies;
	for (CardSprite *c : m_vCards) {
		if (c) {
			cardcopies.push_back(Card(*c->GetCard()));
		}
	}
	return cardcopies;
}




// End Hand

// SwappableHand

bool SwappableHand::SwapCards(SwappableHand * hand, int handslot, int communslot) {

	/*if (m_vSwappedCards.size() >= MAX_SWAPS) {
		return false;
	}*/


	CardSprite * cshand = hand->GetCardForSlot(handslot);
	CardSprite * cscommunity = this->GetCardForSlot(communslot);
	if (!cscommunity || !cshand) {
		return false;
	}
	// Check if the card comming in is one that left before
	Card * card = cshand->GetCard();
	for (Card * c : m_vSwappedCards) {
		if (c->cardFace == card->cardFace && c->cardSuit == card->cardSuit) {
			return false;
		}
	}
	// Check if the card leaving out is one that came in before
	Card * cardcommunity = cscommunity->GetCard();
	for (Card * c : m_vSwappedCardsIn) {
		if (c->cardFace == cardcommunity->cardFace && c->cardSuit == cardcommunity->cardSuit) {
			return false;
		}
	}



	m_vSwappedCards.push_back(cscommunity->GetCard());
	m_vSwappedCardsIn.push_back(cshand->GetCard());

	hand->m_vSwappedCards.push_back(cshand->GetCard());
	hand->m_vSwappedCardsIn.push_back(cscommunity->GetCard());

	cscommunity = this->RemoveCard(communslot);
	cshand = hand->RemoveCard(handslot);
	hand->SetCardAtSlot(handslot, cscommunity);
	this->SetCardAtSlot(communslot, cshand);

	SDL_Rect rec = hand->GetRectForSlot(handslot);
	cscommunity->changeState(CardSprite::none);
	cscommunity->SetToPosition(rec.x + rec.w / 2, rec.y + rec.h / 2);
	cscommunity->changeState(CardSprite::dealing);
	rec = this->GetRectForSlot(communslot);
	cshand->changeState(CardSprite::none);
	cshand->SetToPosition(rec.x + rec.w / 2, rec.y + rec.h / 2);
	cshand->changeState(CardSprite::dealing);


	return true;
}

void SwappableHand::ResetSwaps() {
	// delete record of outbount cards
	for (int i = 0; i < (int)m_vSwappedCards.size(); ++i) {
		//delete m_vSwappedCards[i];
		m_vSwappedCards[i] = nullptr;
	}
	m_vSwappedCards.clear();
	m_vSwappedCards.shrink_to_fit();

	// delete record of inbount cards
	for (int i = 0; i < (int)m_vSwappedCardsIn.size(); ++i) {
		//delete m_vSwappedCardsIn[i];
		m_vSwappedCardsIn[i] = nullptr;
	}
	m_vSwappedCardsIn.clear();
	m_vSwappedCardsIn.shrink_to_fit();
}

SwappableHand::~SwappableHand() {
	cout << "Destroying SwappableHand" << endl;
	ResetSwaps();
}

// End SwappableHand


// DeckSprite

DeckSprite::DeckSprite(int x, int y, int numberofcardstodisplay, bool shuffled, bool facedup) 
	: m_X(x), m_Y(y), Sprite({ 0, 0, 0, 0 }, { x, y, (Game::CARD_WIDTH + m_iCardSpacingX) * numberofcardstodisplay , Game::CARD_HEIGHT}),
	m_bIsFaceUp(facedup){
	m_dDeck = DeckOfCards();

	m_iCardWidth = Game::CARD_WIDTH;
	m_iCardHeight = Game::CARD_HEIGHT;
	if (shuffled)
		m_dDeck.shuffle();

	CardSprite * card;
	for (int i = 0; i < numberofcardstodisplay; i++) {
		/*
		card = GetSpriteForCard(m_dDeck.dealCard());
		card->GetDstP()->x += (i * m_iCardSpacingX);
		card->SetToPosition((card->GetDstP()->x + i * m_iCardSpacingX), card->GetDstP()->y);
		m_qCardQueue.push(card);
		*/
		AddNewCardToQueue(i);
	}
	this->GetDstP()->w = (numberofcardstodisplay * m_iCardSpacingX ) + m_iCardWidth;
	//this->GetDstP()->y -= 3 * m_iCardHeight / 2 ;
	SetLocationOfCards();
}

void DeckSprite::AddNewCardToQueue(int index) {
	CardSprite * card;
	card = GetSpriteForCard(m_dDeck.dealCard());
	card->GetDstP()->x += (index * m_iCardSpacingX);
	
	card->SetToPosition((card->GetDstP()->x + index * m_iCardSpacingX), card->GetDstP()->y);
	//m_qCardQueue.push(card);
	m_lCardList.push_front(card);
}

void DeckSprite::SetLocationOfCards() {
	
	int i = 0;
	for (CardSprite * card: m_lCardList) {
		
		card->GetDstP()->x = m_X + (i * m_iCardSpacingX);
		card->GetDstP()->y = m_Y;
		card->SetToPosition((card->GetDstP()->x + i * m_iCardSpacingX), card->GetDstP()->y);
		i++;
	}

}

SDL_Rect DeckSprite::GetCardFaceUp(Card * card) {
	int x = 0, y = 0;
	switch (card->cardSuit)
	{
	case Card::CLUBS:
		y = 0;
		break;
	case Card::SPADES:
		y = m_iCardHeight;
		break;

	case Card::HEARTS:
		y = m_iCardHeight * 2;
		break;
	case Card::DIAMONDS:
		y = m_iCardHeight * 3;
		break;
	}
	x = card->cardFace * m_iCardWidth;
	return { x, y, m_iCardWidth, m_iCardHeight };
}

SDL_Rect DeckSprite::GetCardFaceDown(Card * card) {
	int x = 0, y = 0;
	switch (card->cardSuit)
	{
	case Card::CLUBS:
	case Card::SPADES:
		y = m_iCardHeight;
		break;

	case Card::HEARTS:
	case Card::DIAMONDS:
		y = 0;
		break;
	}
	x = m_iCardWidth * 13;
	return { x, y, m_iCardWidth, m_iCardHeight};
}

CardSprite * DeckSprite::GetSpriteForCard(Card * card) {
	return new CardSprite(card, m_bIsFaceUp, CardSprite::none, GetCardFaceUp(card), GetCardFaceDown(card), { m_X - m_iCardWidth / 2, m_Y - m_iCardHeight /2 , m_iCardWidth, m_iCardHeight }, m_X, m_Y, .5);
}
void DeckSprite::Update() {
	int i = 0;
	for (CardSprite * c : m_lCardList) {
		c->Update();
	}
}

void DeckSprite::Render() {
	if (m_bIsShuffling) {
		AnimateShuffling();
		
	}
	for (CardSprite * c : m_lCardList) {
		c->Render();
	}
}

void DeckSprite::DealCard(Hand * hand) {
	//CardSprite * temp = new CardSprite(*m_qCardQueue.front());
	CardSprite * temp = GetSpriteForCard(m_lCardList.back()->GetCard());
	temp->GetDstP()->x += ((m_lCardList.size() - 1) * m_iCardSpacingX);
	temp->SetToPosition((temp->GetDstP()->x + (m_lCardList.size() - 1) * m_iCardSpacingX), temp->GetDstP()->y);

	int result = hand->AddCard(temp);

	if (result > -1) {
		//m_qCardQueue.pop();
		m_lCardList.pop_back();
		SDL_Rect rect = hand->GetRectForSlot(result);
		temp->SetToPosition(rect.x + rect.w/2, rect.y+ rect.h/2);
		temp->changeState(CardSprite::dealing);
		AddNewCardToQueue(0);
		SetLocationOfCards();
	}
	else {
		
		if (temp != nullptr) {
			delete temp;
			temp = nullptr;
		}
	}
}

void DeckSprite::Shuffle() {
	// shuffle the deckofcards
	//m_dDeck.shuffle();
	m_bIsShuffling = true;
	//m_iFrame = 0;
}

void DeckSprite::AnimateShuffling() {
	m_iFrame++;
	switch (m_iFrame)
	{
	case 1:
		temp = new CardSprite(*m_qCardQueue.front());
		//m_qCardQueue.pop();
		temp->SetToPosition(temp->GetDstP()->x - 20, temp->GetDstP()->y);
			break;

	case 300:

		m_qCardQueue.push(temp);
		temp->SetToPosition(temp->GetDstP()->x + 25, temp->GetDstP()->y);

	default:
		break;
	}
	if (temp != nullptr)
		temp->Render();
	if (m_iFrame > m_iFrameMax) {
		m_iFrame = 0;
		m_bIsShuffling = false;
	}
}

DeckSprite::~DeckSprite() {
	for (CardSprite * c : m_lCardList) {
		delete c;
		c = nullptr;
	}
	if (temp != nullptr) {
		delete temp;
		temp = nullptr;
	}
}

// End of DeckSprite






