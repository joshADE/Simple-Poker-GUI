#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Button.h"
#include "Sprites.h"
#include "SDL_ttf.h"
#include "Player.h"
#include "TextInput.h"
#include "DeckOfCards.h"
#include "GameText.h"
#include "HandFunctions.h"
using namespace std;

class State // This is the abstract base class for all specific states.
{
public:

	// A struct to hold details about text that needs to be displayed on the screen
	struct TextStruct {

		const char * m_sText;
		int m_iFontsize;
		const char * m_sFont;
		TTF_Font *m_Font;
		SDL_Texture * m_pFontText;
		SDL_Color m_cColor;
		int m_iIndex;
		SDL_Rect m_rTextRect;

		TextStruct(SDL_Rect rectangle, const char * texttodisplay, int fsize, const char * fonttype, SDL_Color fontcolor, int index)
			: m_rTextRect(rectangle), m_sText(texttodisplay), m_iFontsize(fsize), m_sFont(fonttype), m_cColor(fontcolor), m_pFontText(nullptr), m_iIndex(index) {}

	};

protected:
	TTF_Font *m_Font;
	SDL_Texture* m_pFontText;
	SDL_Rect m_rFontRect;

	// members used for messaging area(usually a green bordered area on the screen) to display messages to the player when they perform an action
	string m_sMessage = "";	// stores the message that will be displayed in a message area that all states could use. Will  be cleared after a certain duration 
	const int MESSAGE_DURATION_FRAMES = 600; // specifies the number of frames that the message stays on the screen for
	int m_iCurrFrame = 0; // keeps track of the current frame, up to the duration

	// members used for displaying multiple customized text on the screen
	vector<TextStruct*> m_vAllText; // a vector to store multiple TextStruct text that could be rendered on the screen
	vector<bool> textChanged;	// a parallel vector to store whether a change has been made to a text in m_vAllText

	// method used for displaying customized text
	void PerformTextUpdated(); // will set textchanged vector back to false for all the displayed text. Should be called right after a text has been rendered
	void AddText(TextStruct text); // will add a text to the vector of text to be displayed
	void ChangeText(TextStruct, int); // will change a text at an index in the vector, index is stored inside the textstruct when it has been created. Also, you can change the TextStruct's sdl rectangle without calling this method
	void CleanUpAllText(); // will cleanup all the text when the state is done 
	void RenderText(bool, TextStruct* ); // renders a text
	void RenderSavedText(int); // renders the text at an index in the vector m_vAllText
	void RenderAllText(); // renders all the text in the vector m_vAllText


public:
	virtual void Update() = 0; // A 'pure virtual' method.
	virtual void Render();     
	void RenderFont(bool, const char*, int, int);
	void RenderFont(bool, const char*, int, int, string, int, SDL_Color); // overloaded renderfort to specify the fonttype, size and color
	void RenderMessageArea(bool, SDL_Rect, string, int, SDL_Color); // renders the message area
	virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual void Resume() = 0;


};


class PauseState : public State
{
private:
	vector<Button*> m_vButtons;
	enum btn { resume, exit };

public:
	PauseState() {}
	void Update(); // Method prototype.
	void Render();
	void Enter();
	void Exit();
	void Resume() {}
};


class GameState : public State
{
private: 
	Sprite* m_vBGs;
	int m_iTime, m_iLastTime, m_iTimeCtr, m_iScore;
	SDL_Texture* m_pStarsText;
	SDL_Surface* m_pStarsSurf;
	string m_sTime, m_sScore, m_sText, m_sHelpText1, m_sHelpText2, m_sNumberOfSwapsForCurrentPlayer;

	// Variables that are used during the entire gamestate execution
	CardSprite *m_pCard;
	DeckOfCards *deck;
	DeckSprite *m_pDeck;
	SwappableHand *m_pCommunity;
	vector<SwappableHand> m_vPlayers;
	int playerhandsize = 5;
	bool m_bReleased = true;
	vector<string> m_vPlayerNames;
	vector<TextInput*> m_vPlayerNameInput;
	vector<GameText> m_vGameText;
	

	// Controls the phases of the gamestate: dealing->swapping->result
	// Phases are kind of substates within gamestate 
	enum phase { initial, dealing, swapping, comparing };
	phase m_eCurrentPhase = phase::initial;
	// Button control variables -  Must be parallel to the phase enum
	vector<vector<Button*>> m_vUserButtons;
	enum btn { start, swap, endturn, comparecards};


	// Variables for when the players are swapping
	const int  MAX_NUMBER_OF_SWAPS = 3;
	CardSprite * selectedCard;
	Hand * selectedHand;
	int selectedIndex;
	int m_iSelectedSwappingPlayer = -1,	// no player is selected at first
		m_iTotalPlayersSwapped = 0,	// random number will be used to select player so must keep track of how many plyers swapped
		m_iNumberOfSwapsForCurrentPlayer = MAX_NUMBER_OF_SWAPS; // we count down and reduce number of swapps
	vector<SwappableHand*> m_vSwappingSet; // a set to hold all the entities that can have their cards dragged/placed when swapping 
											// for the game it usually only contains the current player's hand and the community hand
	
	
	// Variables for when the cards are being dealt to each player
	int count = 0; // Used for testing dealing of the cards only
	int m_iCardsDealt = 0;
	Hand *m_pDealingHand;
	bool m_bStartDealing = false;
	int m_iDealingSpeedInFrames = 5; // lower number = faster dealing speed
	
	// Text control variables
	enum texts { help1, help2};
	/* Clears the list of hand that can have thier cards click and dragged when swapping */
	void ClearSwappingSet(); 
public:
	static int m_iPlayerScore, m_iCompScore;
	GameState(int playerScore) : m_iTime(0), m_iLastTime(-1),
		m_iTimeCtr(0), m_iScore(0) {
		GameState::m_iPlayerScore = playerScore;
	}
	void Update();
	void Render();
	void Enter(); 
	void Exit();
	void Resume() { cout << "Resuming Game..." << endl; }
};

class TitleState : public State
{
private:
	vector<Button*> m_vButtons;
	enum btn { play, exit };
	enum textarea { loggedinarea };
	string m_sUser, m_sUserText, m_sScorestring;

public:
	TitleState() {}
	void Update();
	void Render();
	void Enter();
	void Exit();
	void Resume();
};

class ResultState : public State
{
private:
	vector<Button*> m_vButtons;
	enum btn { cont, quit };
	enum textarea { playerchoice, compchoice, resultsforplayer };

	shared_ptr<vector<SwappableHand>> m_vPlayers;
	vector<string> m_vPlayerNames;
	vector<GameText> m_vGameText;
	vector<Ranking> m_vPlayerRanking;
	vector<string> m_vResultTexts;

public:
	ResultState() {}
	ResultState(vector<SwappableHand> players, vector<string> names) : m_vPlayerNames(names) { m_vPlayers = make_shared<vector<SwappableHand>>(players); }
	void Update();
	void Render();
	void Enter();
	void Exit();
	void Resume() {}

};


class StateMachine
{
private:
	vector<State*> m_vStates;

public:
	~StateMachine();
	void Update();
	void Render();
	void PushState(State* pState);
	void ChangeState(State* pState);
	void PopState();
	void Clean();
	vector<State*>& GetStates() { return m_vStates; }
};