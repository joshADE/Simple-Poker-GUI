#pragma once
#include "SDL.h"
#include <string>
using namespace std;
// manages text input, the calling state has to start sdl text input for the input to work
class TextInput
{
private:
	SDL_Rect m_rSrc, m_rDst, m_rFontRect; 
	string inputText = "Enter here";	// text to be displayed initialy
	const string INPUT_FONT = "Img/LTYPE.TTF";
	const int fontsize = 20;
	
	bool renderText = false; // determines whether there is a change in the inputText to regenerate the texture
	SDL_Surface* m_pSurf;
	SDL_Texture* m_pText;
	SDL_Texture * m_pGInputTextTexture;
public:
	bool m_bReleased;
	static enum state { SELECTED, NOT_SELECTED };
	int m_iFrame;
	SDL_Color m_textColor = { 255, 255, 255, 255 };
	TextInput(SDL_Rect, SDL_Rect);
	void Update();
	void Render();
	bool Selected() { return m_iFrame == 1; }
	/* returns the text that the player has entered to the state*/
	string getText() { return inputText; }  
	~TextInput();
};

