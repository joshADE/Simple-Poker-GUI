#include <string>
#include <time.h>
#include "FSM.h"
#include "Game.h"
#include "HandFunctions.h"
#define ROWS 12
#define COLS 16
#define SIZE 64


/*Iterates through the textchanged vector and set the values to false. 
This should be called after everytime text using m_vAllText has been renderd to 
tell the program state that there are no changes to be rendered anymore.*/
void State::PerformTextUpdated() {
	for (int i = 0; i < (int)textChanged.size(); i++) {
		textChanged[i] = false;
	}
}

/* Adds a text to m_vAllText and a new bool value in textChanged vector. This should be call everytime a customized text 
needs to be added.*/
void State::AddText(TextStruct text) {
	m_vAllText.push_back(new TextStruct(text));
	textChanged.push_back(true);
}

/* Changes a customized text in vectore m_vAllText at an index*/
void State::ChangeText(TextStruct text, int index) {
	delete m_vAllText[index];
	m_vAllText[index] = nullptr;
	m_vAllText[index] = new TextStruct(text);
	textChanged[index] = true;
}

/* Deletes all the customized text in m_vAllText. Should be called when leaving the state.*/
void State::CleanUpAllText() {
	for (int i = 0; i < (int)m_vAllText.size(); i++) {
		delete m_vAllText[i];
		m_vAllText[i] = nullptr;
	}

	m_vAllText.clear();
	m_vAllText.shrink_to_fit();

	textChanged.clear();
	textChanged.shrink_to_fit();

}

void State::RenderText(bool r, TextStruct* text) {
	if (r) {
		m_Font = TTF_OpenFont(Game::DEFUALT_FONT.c_str(), text->m_iFontsize);
		// White text.
		SDL_Surface* fontSurf = TTF_RenderText_Solid(m_Font, text->m_sText, text->m_cColor);
		if (text->m_pFontText != nullptr) {
			SDL_DestroyTexture(text->m_pFontText);
		}
		text->m_pFontText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), fontSurf);
		text->m_rTextRect = { text->m_rTextRect.x, text->m_rTextRect.y, fontSurf->w, fontSurf->h };
		SDL_FreeSurface(fontSurf);
	}
	SDL_RenderCopy(Game::Instance()->GetRenderer(), text->m_pFontText, 0, &text->m_rTextRect);
}
/* Renders a text at an index in m_vAlltext*/
void State::RenderSavedText(int index) {
	RenderText(textChanged[index], m_vAllText[index]);
}

void State::RenderAllText() {
	for (int i = 0; i < (int)m_vAllText.size(); i++) {
		RenderSavedText(i);
	}
}



void State::Render()
{
	SDL_RenderPresent(Game::Instance()->GetRenderer());
}

void State::RenderFont(bool r, const char* c, int x, int y)
{
	/*
	if (r)
	{
		m_Font = TTF_OpenFont("Img/LTYPE.TTF", 20);
		SDL_Color textColor = { 255, 255, 255, 0 }; // White text.
		SDL_Surface* fontSurf = TTF_RenderText_Solid(m_Font, c, textColor);
		SDL_DestroyTexture(m_pFontText); // Need to de-allocate previous font texture.
		m_pFontText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), fontSurf);
		m_rFontRect = { x, y, fontSurf->w, fontSurf->h };
		SDL_FreeSurface(fontSurf);
	}
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pFontText, 0, &m_rFontRect);
	*/
	SDL_Color textColor = { 255, 255, 255, 0 };
	RenderFont(r, c, x, y, Game::Instance()->DEFUALT_FONT, 20, textColor);
}

void State::RenderFont(bool r, const char* c, int x, int y, string font, int fontsize, SDL_Color textColor)
{
	if (r)
	{
		m_Font = TTF_OpenFont(font.c_str(), fontsize);
		 // White text.
		SDL_Surface* fontSurf = TTF_RenderText_Solid(m_Font, c, textColor);
		SDL_DestroyTexture(m_pFontText); // Need to de-allocate previous font texture.
		m_pFontText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), fontSurf);
		m_rFontRect = { x, y, fontSurf->w, fontSurf->h };
		SDL_FreeSurface(fontSurf);
	}
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pFontText, 0, &m_rFontRect);
}

void State::RenderMessageArea(bool r, SDL_Rect destRect, string font, int fontsize, SDL_Color color = {255, 255, 255, 255}) {
	SDL_SetRenderDrawBlendMode(Game::Instance()->GetRenderer(), SDL_BLENDMODE_BLEND);

	
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 125, 0, 255);
	SDL_Rect rect2 = { destRect.x - 20, destRect.y - 20, destRect.w + 40, destRect.h + 40 };
	SDL_RenderFillRect(Game::Instance()->GetRenderer(), &rect2);

	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 0, 0, 255);
	SDL_Rect rect = { destRect.x - 10, destRect.y - 10, destRect.w + 20, destRect.h + 20 };
	SDL_RenderFillRect(Game::Instance()->GetRenderer(), &rect);

	RenderFont(r, m_sMessage.c_str(), destRect.x, destRect.y, font, fontsize, color);
}

// Begin TitleState
void TitleState::Enter()
{
	cout << "Entering Title..." << endl;
	Game::Instance()->SetLeftMouse(false);
	m_vButtons.push_back(new Button("Img/play2.png", { 0,0,400,100 }, { 312,150,400,100 }));
	m_vButtons.push_back(new Button("Img/exit2.png", { 0,0,400,100 }, { 312,270,400,100 }));
	
	
	m_sMessage = "Messages will display here";
}

void TitleState::Update()
{
	// Update buttons. Allows for mouseovers.
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Update();
	// Parse buttons. Make sure buttons are in an if..else structure.
	if (m_vButtons[btn::play]->Clicked()) {
			int score = (Game::Instance()->GetCurrentPlayer() == nullptr) ? 0 : Game::Instance()->GetCurrentPlayer()->getScore();
			Game::Instance()->GetFSM()->ChangeState(new GameState(score));
	}
	else if (m_vButtons[btn::exit]->Clicked())
	{
		SDL_Delay(500); // Just pause to let the button sound play.
		Game::Instance()->QuitGame();
	}
	
	

}

void TitleState::Resume() {

}

void TitleState::Render()
{
	cout << "Rendering Title..." << endl;
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 255, 128, 0, 255);
	SDL_RenderClear(Game::Instance()->GetRenderer());
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Render();


	//RenderFont(0, m_sUserText.c_str(), 20, 20);
	RenderMessageArea((m_sMessage != ""), { 750, 650, 200, 100 }, Game::Instance()->KVF, 10);

	RenderAllText();
	PerformTextUpdated();

	State::Render();
}

void TitleState::Exit()
{
	cout << "Exiting Title..." << endl;
	for (int i = 0; i < (int)m_vButtons.size(); i++)
	{
		delete m_vButtons[i];
		m_vButtons[i] = nullptr;
	}
	m_vButtons.clear();
	m_vButtons.shrink_to_fit();

	CleanUpAllText();
}
// End TitleState



// Begin GameState
int GameState::m_iPlayerScore = 0;
int GameState::m_iCompScore = 0;
void GameState::Enter()
{
	cout << "Entering Game..." << endl;
	Game::Instance()->GetAM()->LoadMusic("Aud/game.mp3");
	//Game::Instance()->GetAM()->LoadSound("Aud/explode.wav");
	Game::Instance()->GetAM()->PlayMusic();
	m_Font = TTF_OpenFont("Img/LTYPE.TTF", 20);
	m_pStarsSurf = IMG_Load("Img/background.png");
	
	m_pStarsText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pStarsSurf);
	SDL_FreeSurface(m_pStarsSurf);
	m_vBGs = new Sprite({ 0,0,1024,768 }, { 0,0,1024,768 });
	//deck = DeckOfCards();
	//deck.shuffle();
	//m_pCard = new CardSprite(deck.dealCard(), false, CardSprite::dealing, { 0, 0, 71, 95 }, { 935, 96, 71, 95 }, { 100, 100, 71, 95}, 200, 200, 1);
	m_pDeck = new DeckSprite(850, 300, playerhandsize, true, false); // secondlast arg = shuffle deck / last arg = faceup
	m_pCommunity = new SwappableHand(400, 200, playerhandsize);
	m_vPlayers.push_back(SwappableHand(400, 50, playerhandsize)); //player 1
	m_vPlayers.push_back(SwappableHand(400, 350, playerhandsize)); //player 2
	//m_vPlayers.push_back(SwappableHand(400, 490, playerhandsize)); //player 3 - test

	m_vGameText = vector<GameText>();
	m_sNumberOfSwapsForCurrentPlayer = "Number of swaps left: " + to_string(m_iNumberOfSwapsForCurrentPlayer);
	m_vGameText.push_back(GameText({ 300, 80, 500, 50 }, m_sNumberOfSwapsForCurrentPlayer.c_str(), 10, Game::KVF, { 255, 255, 255, 255 }));


	for (int i = 0; i < (int)m_vPlayers.size(); i++) {
		m_vPlayerNames.push_back("Player" + to_string(i + 1));
	}
	
	for (int i = 0; i < phase::comparing + 1; i++) {
		m_vUserButtons.push_back(vector<Button*>());
	}
	
	m_vUserButtons[phase::initial].push_back(new LabeledButton("Img/BlueButton.png", { 0, 0, 190, 49 }, { 100,700,190,49 }, "start", Game::KVF.c_str(), 10, { 255, 255, 255, 255 })); // start button
	m_vUserButtons[phase::dealing].push_back(new LabeledButton("Img/BlueButton.png", { 0, 0, 190, 49 }, { 250,700,190,49 }, "swap", Game::KVF.c_str(), 10, { 255, 255, 255, 255 })); // swap button
	m_vUserButtons[phase::swapping].push_back(new LabeledButton("Img/BlueButton.png", { 0, 0, 190, 49 }, { 400,700,190,49 }, "end turn", Game::KVF.c_str(), 8, { 255, 255, 255, 255 })); // endturn button
	m_vUserButtons[phase::comparing].push_back(new LabeledButton("Img/BlueButton.png", { 0, 0, 190, 49 }, { 550,700,190,49 }, "compare cards", Game::KVF.c_str(), 8, { 255, 255, 255, 255 })); // endturn button
	srand(time(NULL)); // set the seed for rand() when we sellect the first player to swap
	m_iSelectedSwappingPlayer = rand() % m_vPlayers.size(); // choose a random number to select which player starts swapping first

}


void GameState::Update()
{
	if (Game::Instance()->KeyDown(SDL_SCANCODE_P) == 1)
	{
		Game::Instance()->GetFSM()->PushState(new PauseState());
		return;
	}
	for (int i = 0; i < (int)m_vUserButtons[m_eCurrentPhase].size(); i++)
		m_vUserButtons[m_eCurrentPhase][i]->Update();

	switch (m_eCurrentPhase) {
	case initial:
		
		if (m_vUserButtons[m_eCurrentPhase][0]->Clicked() && m_bReleased) {	// if the player cliks the start button then the deck will start dealing card
			m_bStartDealing = true;
		}

		if (m_bStartDealing) {
			//m_pDealingHand; // clear dealing hand
			if (m_iTimeCtr % m_iDealingSpeedInFrames == 0) {	// after the time counter reaches a certain value deal a card
				
				if (m_iCardsDealt < m_vPlayers.size() * playerhandsize) {	// all the players are given cards first  
					m_pDealingHand = &m_vPlayers[m_iCardsDealt % m_vPlayers.size()];	// m_pDealingHand hold a reference to the hand that should be dealt to wich is givven a card later
				}
				
				else if (m_iCardsDealt < (m_vPlayers.size() * playerhandsize) + 5) {	// after all the players are dealt cards, community is dealt 
					m_pDealingHand = m_pCommunity;
				}
				else {
					
					m_pCommunity->SetFaceUp(true);
					m_eCurrentPhase = dealing; // go to the next phase
					break;
				}

				m_pDeck->DealCard(m_pDealingHand);	// deal the card to the dealing hand
				m_iCardsDealt++;
			}
		}
		
		break;
	case dealing:
		if (m_vUserButtons[m_eCurrentPhase][0]->Clicked() && m_bReleased) { // uuser clicks swap when ready to swap
			if (m_iTotalPlayersSwapped < m_vPlayers.size()) {
				for (Hand p : m_vPlayers) {
					p.SetFaceUp(false);	// set each players cards to face down
					p.SetColor({255, 255, 255, 255});	// sey thier color to white showing they can't swap
				}
				m_vPlayers[m_iSelectedSwappingPlayer].SetFaceUp(true); // if current swapping hand is not faceup set it to face up
				m_vPlayers[m_iSelectedSwappingPlayer].SetColor({ 255, 139, 195, 74 }); // set the current swapping players background to green
				ClearSwappingSet();	// clear the set of hand that can have their cards clikked/dragged for swapping
				m_vSwappingSet.push_back(&m_vPlayers[m_iSelectedSwappingPlayer]); // add the community hand and the swapping players hand
				m_vSwappingSet.push_back(m_pCommunity);
				m_eCurrentPhase = swapping;
				// rest of swapping algorithm is below
			}
			else {
				m_eCurrentPhase = comparing; // if all the players have swapped their cards go to next phase
			}
		}
		break;
	case swapping:

		if (m_vUserButtons[m_eCurrentPhase][0]->Clicked() && m_bReleased) {
			m_eCurrentPhase = dealing;
			m_iNumberOfSwapsForCurrentPlayer = MAX_NUMBER_OF_SWAPS; // reset the current number swappable card
			// Need the reset the history of the cards that were swapped which is stard inside the player/community hand
			// Note: that in a standard game, there will only be one of each card so
			// may or may not want to clear the history of swaps for each hand involved in the swapping (m_vSwappingSet)
			m_pCommunity->ResetSwaps();	// Since each hand keeps track of the cards comming in/ leaving when swapping 
										// we need to clear the history for the community hand when a swapping turn has ended but
			m_iSelectedSwappingPlayer = clampWrapInclusive(0, m_vPlayers.size() - 1, m_iSelectedSwappingPlayer + 1);	// go to next player using a speacial function in my handfunctions header
			m_iTotalPlayersSwapped++; // keeps track of the players that have been given a chance to swap

			if (m_iTotalPlayersSwapped >= m_vPlayers.size()) {
				for (Hand p : m_vPlayers) {
					p.SetFaceUp(true);	// set each players cards to face up
					p.SetColor({ 255, 255, 255, 255 });	// set thier color to white showing they can't swap
				}
				m_eCurrentPhase = comparing;	// go to next phase
			}
			else {
				for (Hand p : m_vPlayers) {
					p.SetFaceUp(false);	// set each players cards to face down
					p.SetColor({ 255, 255, 255, 255 });	// set thier color to white showing they can't swap
				}

			}
			return;
		}
		break;
	case comparing: 
		if (m_vUserButtons[m_eCurrentPhase][0]->Clicked() && m_bReleased) {
			
			Game::Instance()->GetFSM()->PushState(new ResultState(m_vPlayers, m_vPlayerNames));
		}
		break;
	}
	

	int mx = Game::Instance()->GetMouseX();
	int my = Game::Instance()->GetMouseY();

	// Commented below is code the to deal cards when deck is clicked

	//
	//if (mx < (m_pDeck->GetDstP()->x + m_pDeck->GetDstP()->w) && mx > m_pDeck->GetDstP()->x && // If cursor is within X bounds
	//	my < (m_pDeck->GetDstP()->y + m_pDeck->GetDstP()->h) && my > m_pDeck->GetDstP()->y)   // And cursor is within Y bounds
	//{
	//	if (Game::Instance()->GetLeftMouse() && m_bReleased)
	//	{
	//		//m_pDeck->Shuffle();
	//		if (count < (int)m_vPlayers.size()) {
	//			// deal to player
	//		}
	//		else if (count == (int)m_vPlayers.size()) {
	//			// deal to community
	//		}
	//		else {
	//			count = -1; // reset
	//		}
	//		count++;
	//		
	//		
	//		m_bReleased = false;
	//		Game::Instance()->GetAM()->PlaySound(0); // Play the only sfx at index 0.
	//	}
	//	else if (!Game::Instance()->GetLeftMouse()) {
	//		m_bReleased = true;
	//	}
	//}
	

	if (m_eCurrentPhase == swapping) {
		SDL_Rect rect, *rectSel;
		int slots;
		bool hasSwapped = false;

		if (Game::Instance()->GetLeftMouse() && m_bReleased)
		{

			for (int i = 0; i < (int)m_vSwappingSet.size(); i++) {

				slots = m_vSwappingSet[i]->GetSlotCount();

				for (int j = 0; j < slots; j++) {
					rect = m_vSwappingSet[i]->GetRectForSlot(j);
					if (mx < (rect.x + rect.w) && mx > rect.x && // If cursor is within X bounds
						my < (rect.y + rect.h) && my > rect.y)   // And cursor is within Y bounds
					{
						if (m_bReleased) {
							selectedCard = m_vSwappingSet[i]->GetCardForSlot(j);
							if (selectedCard) {
								selectedHand = m_vSwappingSet[i];
								selectedIndex = j;
								selectedCard->changeState(CardSprite::swapping);
							}

							m_bReleased = false;
							Game::Instance()->GetAM()->PlaySound(0); // Play the only sfx at index 0.

						}

					}
				}
			}
			m_bReleased = false;
		}
		else if (!Game::Instance()->GetLeftMouse()) {
			m_bReleased = true;
		}
		if (selectedCard) {
			rect = selectedHand->GetRectForSlot(selectedIndex);
			rectSel = selectedCard->GetDstP();
			if ( m_bReleased) {
				if (rectSel->x < (rect.x + rect.w) && rectSel->x > rect.x && // If the selected card is at their slot then the are no longer seleted 
					rectSel->y < (rect.y + rect.h) && rectSel->y > rect.y)   // It is imprtant to make sure 
				{
					
				}

				hasSwapped = false;
				if (m_iNumberOfSwapsForCurrentPlayer > 0) {
					for (int i = 0; i < (int)m_vSwappingSet.size(); i++) {
						if (m_vSwappingSet[i] == selectedHand) {	// ignore the hand where the selected card came from when detecting where to place the card
							continue;
						}
						slots = m_vSwappingSet[i]->GetSlotCount();
						for (int j = 0; j < slots; ++j) {
							rect = m_vSwappingSet[i]->GetRectForSlot(j);
							if (mx < (rect.x + rect.w) && mx > rect.x && // If cursor is within X bounds
								my < (rect.y + rect.h) && my > rect.y)   // And cursor is within Y bounds
							{
								hasSwapped = m_vSwappingSet[i]->SwapCards((static_cast<SwappableHand*>(selectedHand)), selectedIndex, j);	// returns false only when the card has already been with the hand before
								
								// regardless of whether the swap is successful/not, user has realeased the mouse on a valid/invadil slot so exit all loops
								i = m_vSwappingSet.size(); // set it so that it immediately exits outer loop;
								break; // break inner loop (or use the code in line above)
							}
						}
					}
				}

				if (!hasSwapped) {
					rect = selectedHand->GetRectForSlot(selectedIndex);
					selectedCard->SetToPosition(rect.x + rect.w / 2, rect.y + rect.h / 2);
					selectedCard->changeState(CardSprite::dealing);
				}
				else {
					
					m_iNumberOfSwapsForCurrentPlayer--;
				}

				selectedCard = nullptr;
			}
			else {
				// Note: Both line of code below are equivalent
				//selectedCard->SetPosition(mx + rect.w / 2.0, my + rect.h / 2.0);
				
				selectedCard->SetToPosition(mx, my);
			}
		}
	}
	



	for (SwappableHand & ply : m_vPlayers) {
		ply.Update();
	}


	m_pDeck->Update();
	m_pCommunity->Update();


	for (int i = 0; i < (int)m_vGameText.size(); i++)
		m_vGameText[i].Update();
	

	m_iTimeCtr++;

	
}

void GameState::Render()
{
	cout << "Rendering Game..." << endl;
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(Game::Instance()->GetRenderer());
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pStarsText, m_vBGs->GetSrcP(), m_vBGs->GetDstP());
	

	for (int i = 0; i < (int)m_vUserButtons[m_eCurrentPhase].size(); i++)
		m_vUserButtons[m_eCurrentPhase][i]->Render();
	
	/*
	m_iTime =  m_iTimeCtr / 30;
	m_iScore = scroller->m_iSprite;
	m_sScore = "SCORE: " + to_string(m_iScore);
	m_sTime = "TIME: " + to_string(m_iTime);
	m_sTime += " ";
	m_sTime += m_sScore;
	
	*/

	//m_pCard->Render();
	m_pDeck->Render();
	m_pCommunity->Render();
	for (SwappableHand & ply : m_vPlayers) {
		ply.Render();
	}



	// Render the cards ontop
	m_pCommunity->RenderCards();

	for (SwappableHand & ply : m_vPlayers) {
		ply.RenderCards();
	}

	if (selectedCard) {
		selectedCard->Render();
	}


	m_sScore = "Player Score - " + to_string(m_iPlayerScore);
	m_sScore += " | Computer Score - " + to_string(m_iCompScore);
	m_sNumberOfSwapsForCurrentPlayer = "Number of swaps left: " + to_string(m_iNumberOfSwapsForCurrentPlayer);

	
	
	RenderAllText();
	PerformTextUpdated();

	m_vGameText[0].setText(m_sNumberOfSwapsForCurrentPlayer.c_str());
	for (int i = 0; i < (int)m_vGameText.size(); i++)
		m_vGameText[i].Render();

	RenderFont((m_iLastTime < m_iTime ? 1 : 0), m_sScore.c_str(), 300, 50);
	m_iLastTime = m_iTime;
	State::Render();
}

void GameState::Exit()
{
	cout << "Exiting Game..." << endl;
	Game::Instance()->GetAM()->ClearMusic(); // De-allocate the music track.
	TTF_CloseFont(m_Font);
	SDL_DestroyTexture(m_pStarsText);
	SDL_DestroyTexture(m_pFontText);
	for (int i = 0; i < (int)m_vUserButtons.size(); i++)
	{
		for (int j = 0; j < (int)m_vUserButtons[i].size(); j++) {
			delete m_vUserButtons[i][j];
			m_vUserButtons[i][j] = nullptr;
		}
	}
	//delete[] &m_vUserButtons;
	m_vUserButtons.clear();
	m_vUserButtons.shrink_to_fit();

	ClearSwappingSet();

	delete m_pDeck;
	m_pDeck = nullptr;
	delete m_pCommunity;
	m_pCommunity = nullptr;
	
	for (int i = 0; i < (int)m_vPlayers.size(); i++)
	{
		//delete &m_vPlayers[i];
		//m_vPlayers[i] = nullptr;
	}
	//delete[] &m_vPlayers;
	m_vPlayers.clear();
	m_vPlayers.shrink_to_fit();

	m_vGameText.clear();
	m_vGameText.shrink_to_fit();

	CleanUpAllText();
}

/* Simply goes throught list of swapping card set and clears it(set to nullptr), but does not delete/destroy the contents*/
void GameState::ClearSwappingSet() {
	for (int i = 0; i < (int)m_vSwappingSet.size(); i++)
	{
		m_vSwappingSet[i] = nullptr;
	}
	m_vSwappingSet.clear();
	m_vSwappingSet.shrink_to_fit();
	
}
// End GameState

// ResultState - show the users option show the computers option and display results plas total score

void ResultState::Enter()
{
	cout << "Entering Results..." << endl;
	m_vButtons.push_back(new Button("Img/resume.png", { 0,0,200,80 }, { 200,550,200,80 }));
	m_vButtons.push_back(new Button("Img/exit.png", { 0,0,400,100 }, { 600,550,200,80 }));
	Game::Instance()->GetAM()->ToggleMusic(); // Pause the music.
	
	
	//RenderFont(1, m_sDescription.c_str(), 150, 150);
	/*
	TextStruct p1C = TextStruct({ 150, 175, 0, 0}, m_sPlayerText.c_str(), 25, Game::Instance()->DEFUALT_FONT.c_str(), { 255, 255, 125, 255 }, playerchoice);
	TextStruct compC = TextStruct({ 150, 195, 0, 0 }, m_sComputerText.c_str(), 25, Game::Instance()->DEFUALT_FONT.c_str(), { 255, 255, 125, 255 }, compchoice);
	TextStruct res = TextStruct({ 150, 215, 0, 0 }, m_sResultText.c_str(), 25, Game::Instance()->DEFUALT_FONT.c_str(), { 255, 255, 125, 255 }, resultsforplayer);

	AddText(p1C);
	AddText(compC);
	AddText(res);
	*/

	if (m_vPlayers->size() != m_vPlayerNames.size()) {
		return;
	}

	SDL_Rect rect = {200, 300, 500, 50};
	string s;
	vector<Card> cardsforPlayer;
	for (int i = 0; i < (int)m_vPlayers->size(); i++) {
		cardsforPlayer = m_vPlayers->at(i).GetCopyOfAllCards();
		if (cardsforPlayer.size() != 5) {
			break;
		}
		m_vResultTexts.push_back(m_vPlayerNames[i] + ": ");
		m_vPlayerRanking.push_back(static_cast<Ranking>(describeHand(cardsforPlayer.data(), cardsforPlayer.size())));
		m_vResultTexts[i] += readStrengthDescription(m_vPlayerRanking[i]);
		m_vGameText.push_back(GameText({ rect.x, rect.y + 20 * i, rect.w, rect.h }, m_vResultTexts[i].data() , 10, Game::KVF.data(), {255,255,255,255}));
		cout << m_vResultTexts[i] << endl;
	}

}

void ResultState::Update()
{
	// Update buttons. Allows for mouseovers.
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Update();

	for (int i = 0; i < (int)m_vGameText.size(); i++)
		m_vGameText[i].Update();


	// Parse buttons. Make sure buttons are in an if..else structure.
	if (m_vButtons[btn::cont]->Clicked()) {
		Game::Instance()->GetFSM()->PopState();
	}
	else if (m_vButtons[btn::quit]->Clicked())
	{

		Game::Instance()->GetFSM()->Clean(); // Clear all states, including GameState on bottom.
		Game::Instance()->GetFSM()->ChangeState(new TitleState()); // Then change to a new TitleState.
	}

}

void ResultState::Render()
{
	cout << "Rendering Result..." << endl;
	Game::Instance()->GetFSM()->GetStates().front()->Render();
	SDL_SetRenderDrawBlendMode(Game::Instance()->GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 64, 64, 128, 250);
	SDL_Rect rect = { 100, 100, 824, 568 };
	SDL_RenderFillRect(Game::Instance()->GetRenderer(), &rect);

	//RenderFont(0, m_sDescription.c_str(), 150, 150);

	RenderAllText();
	PerformTextUpdated();
	
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Render();

	for (int i = 0; i < (int)m_vGameText.size(); i++)
		m_vGameText[i].Render();

	State::Render();
}

void ResultState::Exit()
{
	cout << "Exiting Result..." << endl;
	Game::Instance()->GetAM()->ToggleMusic(); // Resume the music.
	for (int i = 0; i < (int)m_vButtons.size(); i++)
	{
		delete m_vButtons[i];
		m_vButtons[i] = nullptr;
	}
	m_vButtons.clear();
	m_vButtons.shrink_to_fit();

	m_vResultTexts.clear();
	m_vResultTexts.shrink_to_fit();


	/*
	for (int i = 0; i < (int)m_vGameText.size(); i++)
		m_vGameText[i].Update();
		*/
	m_vGameText.clear();
	m_vGameText.shrink_to_fit();


	CleanUpAllText();
}

// End ResultState

// Begin PauseState
void PauseState::Enter()
{
	cout << "Entering Pause..." << endl;
	m_vButtons.push_back(new Button("Img/resume.png", { 0,0,200,80 }, { 412,200,200,80 }));
	m_vButtons.push_back(new Button("Img/exit.png", { 0,0,400,100 }, { 412,400,200,80 }));
	Game::Instance()->GetAM()->ToggleMusic(); // Pause the music.
}

void PauseState::Update()
{
	// Update buttons. Allows for mouseovers.
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Update();
	// Parse buttons. Make sure buttons are in an if..else structure.
	if (m_vButtons[btn::resume]->Clicked())
		Game::Instance()->GetFSM()->PopState();
	else if (m_vButtons[btn::exit]->Clicked())
	{

		Game::Instance()->GetFSM()->Clean(); // Clear all states, including GameState on bottom.
		Game::Instance()->GetFSM()->ChangeState(new TitleState()); // Then change to a new TitleState.
	}
}

void PauseState::Render()
{
	cout << "Rendering Pause..." << endl;
	Game::Instance()->GetFSM()->GetStates().front()->Render();
	SDL_SetRenderDrawBlendMode(Game::Instance()->GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 64, 64, 128, 128);
	SDL_Rect rect = { 256, 128, 512, 512 };
	SDL_RenderFillRect(Game::Instance()->GetRenderer(), &rect);
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Render();
	State::Render();
}

void PauseState::Exit()
{
	cout << "Exiting Pause..." << endl;
	Game::Instance()->GetAM()->ToggleMusic(); // Resume the music.
	for (int i = 0; i < (int)m_vButtons.size(); i++)
	{
		delete m_vButtons[i];
		m_vButtons[i] = nullptr;
	}
	m_vButtons.clear();
	m_vButtons.shrink_to_fit();
}
// End PauseState


// Begin StateMachine
void StateMachine::Update()
{
	if (!m_vStates.empty()) // empty() and back() are methods of the vector type.
		m_vStates.back()->Update();
}

void StateMachine::Render()
{
	if (!m_vStates.empty())
		m_vStates.back()->Render();
}

void StateMachine::PushState(State* pState)
{
	m_vStates.push_back(pState); // push_back() is a method of the vector type.
	m_vStates.back()->Enter();
}

void StateMachine::ChangeState(State* pState)
{
	if (!m_vStates.empty())
	{
		m_vStates.back()->Exit();
		delete m_vStates.back(); // De-allocating the state in the heap.
		m_vStates.back() = nullptr; // Nullifying pointer to the de-allocated state.
		m_vStates.pop_back(); // Removes the now-null pointer from the vector.
	}
	pState->Enter();
	m_vStates.push_back(pState);
}

void StateMachine::PopState()
{
	if (!m_vStates.empty())
	{
		m_vStates.back()->Exit();
		delete m_vStates.back();
		m_vStates.back() = nullptr;
		m_vStates.pop_back();
	}
	m_vStates.back()->Resume();
}

void StateMachine::Clean()
{
	while (!m_vStates.empty()) // Ensures that ALL states left in the vector are cleaned up.
	{						   
		m_vStates.back()->Exit();
		delete m_vStates.back();
		m_vStates.back() = nullptr;
		m_vStates.pop_back();
	}
}

StateMachine::~StateMachine()
{
	cout << "Destroying FSM..." << endl;
	Clean();
}
// End StateMachine