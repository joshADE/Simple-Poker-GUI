#include "TextInput.h"
#include "Game.h"


TextInput::TextInput(SDL_Rect src, SDL_Rect dst)
{

	m_bReleased = true;
	renderText = true; 
	m_rSrc = src;
	m_rDst = dst;
	m_iFrame = NOT_SELECTED;
	
}


TextInput::~TextInput()
{
	SDL_DestroyTexture(m_pText);
}

void TextInput::Update()
{
	int mx = Game::Instance()->GetMouseX();
	int my = Game::Instance()->GetMouseY();
	if (mx < (m_rDst.x + m_rDst.w) && mx > m_rDst.x && // If cursor is within X bounds
		my < (m_rDst.y + m_rDst.h) && my > m_rDst.y)   // And cursor is within Y bounds
	{
		if (Game::Instance()->GetLeftMouse() && m_bReleased)
		{
			m_bReleased = false;
			m_iFrame = SELECTED;
			Game::Instance()->GetAM()->PlaySound(0); // Play the only sfx at index 0.
		}
		else if (!Game::Instance()->GetLeftMouse())
		{
			m_bReleased = true;
		}
	}

	if (m_iFrame == SELECTED) {
		// Game::Instance()->OpenTextInput(); // done in the games state
		// sdl-textinput event handling must be started to recieve textinput events

		if (Game::Instance()->Tick() && Game::Instance()->KeyDown(SDL_SCANCODE_BACKSPACE) == 1 && inputText.length() > 0) {
			inputText.pop_back(); // remove the last character from input text
			renderText = true;	// allow rendering of the new input
			
		}else if (Game::Instance()->HasInput()) {
			
				inputText += Game::Instance()->GetInput();	// add the last input
				//cout << inputText << endl;
				renderText = true;
			
		}
		
	}
	//else if (m_iFrame == NOT_SELECTED) {
	 //Game::Instance()->CloseTextInput(); // done in game state when leaving
	//}
}

void TextInput::Render()
{
	SDL_SetRenderDrawBlendMode(Game::Instance()->GetRenderer(), SDL_BLENDMODE_BLEND);

	if (m_iFrame == SELECTED) {
		// draw an outer rectangle that shows that the input is selected
		SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 125, 0, 0, 255);
		SDL_Rect rect2 = { m_rDst.x - 20, m_rDst.y - 20, m_rDst.w + 40, fontsize + 40 };
		SDL_RenderFillRect(Game::Instance()->GetRenderer(), &rect2);
	}

	// Background rectangle of the input - black 
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 0, 0, 255);
	SDL_Rect rect = { m_rDst.x - 10, m_rDst.y - 10, m_rDst.w + 20, fontsize + 20 };
	SDL_RenderFillRect(Game::Instance()->GetRenderer(), &rect);
	
	if (renderText) {
	
		TTF_Font *m_Font = TTF_OpenFont(INPUT_FONT.c_str(), fontsize);
		m_rSrc.x = m_rSrc.w * m_iFrame;	// may be used for later purpose
		SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pText, &m_rSrc, &m_rDst);
		// White text.
		SDL_Surface* fontSurf;
		if (inputText != "")
			fontSurf = TTF_RenderText_Solid(m_Font, inputText.c_str(), m_textColor);
		else
			fontSurf = TTF_RenderText_Solid(m_Font, " ", m_textColor);

		SDL_DestroyTexture(m_pText); // Need to de-allocate previous font texture.
		m_pText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), fontSurf);
		
		m_rFontRect = { m_rDst.x, m_rDst.y, fontSurf->w, fontSurf->h };
		SDL_FreeSurface(fontSurf);
		renderText = false;
	}
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pText, 0, &m_rFontRect);
}

