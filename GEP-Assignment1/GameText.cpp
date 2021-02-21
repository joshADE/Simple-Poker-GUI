#include "GameText.h"
#include "Game.h"

/*Not used in game*/


GameText::GameText()
{
}


GameText::~GameText()
{
	//delete m_pFontText;
	//m_pFontText = nullptr;
}

void GameText::Update() {}
void GameText::Render() {
	if (m_bChanged)
	{
		m_Font = TTF_OpenFont(m_sFont.c_str(), m_iFontsize);
		// White text.
		SDL_Surface* fontSurf = TTF_RenderText_Solid(m_Font, m_sText.c_str(), m_cColor);
		if (m_pFontText != nullptr) {
			SDL_DestroyTexture(m_pFontText); 
		}
		m_pFontText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), fontSurf);
		m_rTextRect = { m_rTextRect.x, m_rTextRect.y, fontSurf->w, fontSurf->h };
		SDL_FreeSurface(fontSurf);
		m_bChanged = false;
	}
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pFontText, 0, &m_rTextRect);
}


void GameText::setText(const char* newtext) {
	m_sText = newtext;
	m_bChanged = true;
}
void GameText::setFont(const char* newfont) {
	m_sFont = newfont;
	m_bChanged = true;
}
void GameText::setFontSize(int fontsize) {
	m_iFontsize = fontsize;
	m_bChanged = true;
}

//void setTexture(SDL_Texture*);

void GameText::setColor(SDL_Color newcolor) {
	m_cColor = newcolor;
	m_bChanged = true;
}
