#pragma once
#include <vector>
#include "AudioManager.h"
#include "FSM.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "RankManager.h"

using namespace std;

class Game
{
private:
	bool m_bRunning;
	bool m_bGotTick;
	bool m_bLeftMouse = false; // Keeps track of left mouse button state.
	int m_iMouseX, m_iMouseY;  // Variables to hold mouse positions.
	const Uint8* m_iKeystates;
	SDL_Window* m_pWindow;
	SDL_Texture* m_pSprText;
	SDL_Surface* m_pSprSurf;
	SDL_Renderer* m_pRenderer;
	AudioManager* m_pAM;
	StateMachine* m_pFSM; // Pointer to the StateMachine object created dynamically.
	Game() :m_bRunning(false), m_bGotTick(false) {}

	// new members
	Player * m_pCurrentPlayer; // stores a pointer to the current player located in the ranker
	RankManager * m_pRanker; // pointer to the ranker/rank manager
	string m_sLastInput; // stores the last input recieved as text, once it has been read, lastinput is cleared
	bool m_bTextInputOpened; // store a bool value represent whether SDL can recieve text input


public:
	static const int CARD_WIDTH = 72, CARD_HEIGHT = 96;
	static const string DEFUALT_FONT, KVF, KVFT; // static constanst representing the name/path of the fonts that the game uses
	static Game* Instance();
	SDL_Window* GetWindow() { return m_pWindow; }
	SDL_Renderer* GetRenderer() { return m_pRenderer; }
	SDL_Texture* GetSprTexture() { return m_pSprText; }
	AudioManager* GetAM() { return m_pAM; }
	StateMachine* GetFSM() { return m_pFSM; }
	int GetMouseX() { return m_iMouseX; }
	int GetMouseY() { return m_iMouseY; }
	int GetLeftMouse() { return m_bLeftMouse; }
	void SetLeftMouse(bool b) { m_bLeftMouse = b; }
	void QuitGame() { m_bRunning = false; }
	bool Init(const char* title, const int xpos, const int ypos,
		const int width, const int height, const int flags);
	bool Running();
	bool KeyDown(SDL_Scancode c);
	bool Tick();
	void Update();
	void HandleEvents();
	void Render();
	void Clean();



	Player* LoginPlayer(Player *); // Login a player 
	Player * CheckUserPassword(string, string); // check username/password of player managed by ranker
	void SavePlayerToFile(); // saves the players score/info to their file

	/*Get the current player, may be nullptr*/
	Player inline * GetCurrentPlayer() { return m_pCurrentPlayer; }
	/*Get the pointer to the rank manager*/
	RankManager * GetRankManager() { return m_pRanker; } 
	/*Starts SDL text input handling*/
	void inline OpenTextInput() { SDL_StartTextInput(); m_bTextInputOpened = true; }
	/*Stops SDL text input handling*/
	void inline CloseTextInput() { SDL_StopTextInput(); m_bTextInputOpened = false; }
	/*Returns whether or not SDL is handling text input*/
	bool inline IsTextInputOpened() { return m_bTextInputOpened; }
	/*Returns whether or not there is text input*/
	bool inline HasInput() { return m_sLastInput != ""; }
	/*Returns the last text input/character that was typed and clears it*/
	string inline GetInput() { string input = m_sLastInput; m_sLastInput = ""; return input; }

	/*Set a new score for the current player. Returns wheter or not the score was set. 
	It will be false if the current player is nullptr*/
	bool inline SetPlayerScore(int score) {
		if (m_pCurrentPlayer != nullptr) {
			m_pCurrentPlayer->setScore(score);
			return true;
		}
		return false;
	}
};