#pragma once
#include "SDL.h"

class Button
{
protected:
	enum state { MOUSE_UP, MOUSE_OVER, MOUSE_DOWN };
	bool m_bReleased;
	int m_iFrame, m_ilastFrame;
	SDL_Rect m_rSrc;
	SDL_Rect m_rDst, m_rFontRect;
	SDL_Surface* m_pSurf;
	SDL_Texture* m_pText;

public:
	Button(const char*, SDL_Rect, SDL_Rect);
	~Button();
	void Update();
	void virtual Render();
	bool Clicked() { return m_iFrame == 2; }

};


class LabeledButton : public Button
{
private:
	
	SDL_Texture* m_pFontText;
	SDL_Rect m_rFontRect;
	
public:
	LabeledButton(const char* c, SDL_Rect src, SDL_Rect des, const char* text, const char * font, int fontsize, SDL_Color color);
	//void setText(string, string, SDL_Color, int);
	void Render();
	~LabeledButton();
};