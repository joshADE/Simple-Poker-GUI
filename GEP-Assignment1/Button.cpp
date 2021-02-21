#include "Button.h"
#include "FSM.h"
#include "Game.h"
#include "SDL.h"
#include "SDL_image.h"

Button::Button(const char* s, SDL_Rect src, SDL_Rect dst)
{
	// Set the button image. You should have some fail checking just in case. 
	m_pSurf = IMG_Load(s);
	m_pText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);
	// Do the rest.
	m_bReleased = true;
	m_rSrc = src;
	m_rDst = dst;
	m_ilastFrame = 99;
}



Button::~Button()
{
	SDL_DestroyTexture(m_pText);
}

void Button::Update()
{
	int mx = Game::Instance()->GetMouseX();
	int my = Game::Instance()->GetMouseY();
	if (mx < (m_rDst.x + m_rDst.w) && mx > m_rDst.x && // If cursor is within X bounds
		my < (m_rDst.y + m_rDst.h) && my > m_rDst.y)   // And cursor is within Y bounds
	{
		if (Game::Instance()->GetLeftMouse() && m_bReleased)
		{
			m_iFrame = MOUSE_DOWN;
			m_bReleased = false;
			Game::Instance()->GetAM()->PlaySound(0); // Play the only sfx at index 0.
		}
		else if (!Game::Instance()->GetLeftMouse())
		{
			m_bReleased = true;
			m_iFrame = MOUSE_OVER;
		}
	}
	else
		m_iFrame = MOUSE_UP;
}

void Button::Render()
{



	m_rSrc.x = m_rSrc.w * m_iFrame;
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pText, &m_rSrc, &m_rDst);
	

}

// Labeled Button

LabeledButton::LabeledButton(const char* c, SDL_Rect src, SDL_Rect des, const char* text, const char * font, int fontsize, SDL_Color color)
	: Button(c, src, des) {
	TTF_Font * m_Font;
	m_Font = TTF_OpenFont(font, fontsize);
	SDL_Surface* fontSurf = TTF_RenderText_Solid(m_Font, text, color);
	m_pFontText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), fontSurf);
	SDL_FreeSurface(fontSurf);
	m_rFontRect = m_rFontRect = { des.x + des.w/4, des.y + des.h / 4, des.w / 2, des.h /2 };

}

void LabeledButton::Render() {
	this->Button::Render();
	SDL_SetRenderDrawBlendMode(Game::Instance()->GetRenderer(), SDL_BlendMode::SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pFontText, 0, &m_rFontRect);
}

LabeledButton::~LabeledButton() {
	SDL_DestroyTexture(m_pFontText);
}




