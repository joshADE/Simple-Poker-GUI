#include <chrono>
#include <iostream>
#include "Game.h"
#define FPS 30
using namespace std;
using namespace chrono;

const string Game::DEFUALT_FONT = "Img/LTYPE.TTF", Game::KVF = "Img/kenvector_future.TTF", Game::KVFT = "Img/kenvector_future_thin.TTF";


Game* Game::Instance()
{
	static Game* instance = new Game();
	return instance;
}

bool Game::Init(const char* title, const int xpos, const int ypos,
	const int width, const int height, const int flags)
{
	// Attempt to initialize SDL.
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) // 0 is error code meaning success.
	{
		cout << "SDL init success!" << endl;
		// Initialize the window
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (m_pWindow != nullptr) // Window init success. 
		{
			cout << "Window creation successful!" << endl;
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
			if (m_pRenderer != nullptr) // Renderer init success. 
			{
				SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
				cout << "Renderer creation success!" << endl;
				if (IMG_Init(IMG_INIT_PNG) != 0)
				{
					// Nothing needed for images right now.
					cout << "Image creation success!" << endl;
					m_pSprSurf = IMG_Load("Img/cards.png");
					m_pSprText = SDL_CreateTextureFromSurface(m_pRenderer, m_pSprSurf);
					SDL_FreeSurface(m_pSprSurf);
					if (Mix_Init(MIX_INIT_MP3) != 0) // Mixer init success.
					{
						Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096);
						Mix_AllocateChannels(16);
						cout << "Mixer creation success!" << endl;
						if (TTF_Init() == 0) // Font init success.
							cout << "Font init success!" << endl;
						else
						{
							cout << "Font init fail!" << endl;
							return false; // Font init fail.
						}
					}
					else
					{
						cout << "Mixer init fail!" << endl;
						return false; // Mixer init fail.
					}
				}
				else
				{
					cout << "Image init fail!" << endl;
					return false;// Image init fail.
				}
			}
			else
			{
				cout << "Renderer init fail!" << endl;
				return false; // Renderer init fail. 
			}
		}
		else
		{
			cout << "Window init fail!" << endl;
			return false; // Window init fail. 
		}
	}
	else
	{
		cout << "SDL init fail!" << endl;
		return false; // SDL init fail. 
	}
	srand((unsigned)time(NULL));
	m_iKeystates = SDL_GetKeyboardState(nullptr);
	m_pAM = new AudioManager(); // Creates the audio manager object.
	m_pAM->SetMusicVolume(10); // Set low volume so we can hear sfx.
	m_pAM->LoadSound("Aud/button.wav"); // A sound for all button presses.
	m_pFSM = new StateMachine(); // Creates the state machine object/instance.
	
	m_pRanker = new RankManager();
	m_pCurrentPlayer = nullptr; // m_pRanker->createNewPlayer("JoshThe1", "magicjosh", "1234", 5);
	
	m_pFSM->ChangeState(new TitleState()); // Invoking the ChangeState method to set the initial state, Title.
	m_bRunning = true;
	return true;
}

bool Game::Running()
{
	return m_bRunning;
}

bool Game::KeyDown(SDL_Scancode c)
{
	if (m_iKeystates != nullptr)
	{
		if (m_iKeystates[c] == 1)
			return true;
		else
			return false;
	}
	return false;
}

bool Game::Tick()
{
	auto duration = steady_clock::now().time_since_epoch();
	auto count = duration_cast<microseconds>(duration).count();
	int tick = 1000000 / FPS;
	if (count % tick < 100) // Margin of error for modulus.
	{
		if (m_bGotTick == false) // Drops potential duplicate frames.
			m_bGotTick = true;
	}
	else m_bGotTick = false;
	return m_bGotTick;
}

void Game::Update()
{
	GetFSM()->Update(); // Invokes the update of the state machine.
}

void Game::HandleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) // SDL_PollEvent invokes SDL_PumpEvents().
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_bRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
				m_bLeftMouse = true;
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
				m_bLeftMouse = false;
			break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&m_iMouseX, &m_iMouseY);
			break;
		// new case for text input - only works if game starts input hanling with sdl (Openstextinput() method called)
		case SDL_TEXTINPUT:
				m_sLastInput = event.text.text;
			break;
		}
		
	}
}

void Game::Render()
{
	GetFSM()->Render();
}

void Game::Clean()
{
	cout << "Cleaning game. Bye!" << endl;
	delete m_pFSM; // Force StateMachine to destruct.
	delete m_pAM;  // Force AudioManager to destruct.
	delete m_pRanker; // Destruct the rank manager
	SDL_DestroyTexture(m_pSprText);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	TTF_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

/*This will save the current player's score/password detail to their score file. Their score file's path/name is 
unique for the player and is based on their username. The partial path is defined in the RankManager*/
void Game::SavePlayerToFile() {
	string completeFile = m_pRanker->partialUserScoresFileName + m_pCurrentPlayer->getUserName();
	completeFile += m_pRanker->fileExtention; // find the right file for the player]
	// player ranking is already saved by the RankManager once player is created by manager
	// only their scoring info needs to be saved in a seperate score file
	m_pRanker->setOrUpdateScoreToFile(m_pCurrentPlayer, completeFile); // save the player scoring info
}

/*Logs the specified player in by setting the new player as the current player
and returns a pointer to the player that was logged out. Nullptr if no player.*/
Player* Game::LoginPlayer(Player * newPlayer) {
	
	if (m_pCurrentPlayer != nullptr) {
		Player * temp = m_pCurrentPlayer;
		m_pCurrentPlayer = newPlayer;
		return temp;
	}
	m_pCurrentPlayer = newPlayer;
	return nullptr;
	

}
/* Given a username and password, check if a player with the specified username/password exist. 
 If the player exists, return a pointer to the player managed by the ranker/rank manager. If they don't, return nullptr.*/
Player * Game::CheckUserPassword(string username, string password) {
	Player * player = m_pRanker->userExist(*m_pRanker->getAllPlayerSorted(), username);
	if (player == nullptr)
		return nullptr;

	if (player->authenticate(password)) {
		return player;
	}
	return nullptr;

}