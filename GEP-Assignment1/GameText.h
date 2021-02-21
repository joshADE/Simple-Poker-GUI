#pragma once
#include "SDL.h"
#include <string>
#include "SDL_ttf.h"

using namespace std;
/*Class based on the TextStruct in State Class. In order for the text to be displayed,
 the variable that contains the text that is passed into the constructor must not be local to a function/method, it must be a member variable and 
 must be stored seperately outside (and for duration of the lifetime) of the Gametext object */
class GameText
{
protected:

	string m_sText;
	int m_iFontsize;
	string m_sFont;
	TTF_Font *m_Font;
	SDL_Texture * m_pFontText;
	SDL_Color m_cColor;
	bool m_bChanged;

public:
	SDL_Rect m_rTextRect;
	GameText(SDL_Rect rectangle, const char * texttodisplay, int fsize, const char * fonttype, SDL_Color fontcolor)
		: m_rTextRect(rectangle), m_sText(texttodisplay), m_iFontsize(fsize), m_sFont(fonttype), m_cColor(fontcolor), m_pFontText(nullptr), m_bChanged(true) {}
	GameText(SDL_Rect rectangle, string texttodisplay, int fsize, string fonttype, SDL_Color fontcolor)
		: m_rTextRect(rectangle), m_sText(texttodisplay), m_iFontsize(fsize), m_sFont(fonttype), m_cColor(fontcolor), m_pFontText(nullptr), m_bChanged(true) {}

	GameText();
	~GameText();
	void Update();
	void Render();

	string getText() {return m_sText;}
	string getFont() { return m_sFont; }
	int getFontSize() { return m_iFontsize; }
	SDL_Texture* getTexture() { return m_pFontText; }
	SDL_Color getColor() { return m_cColor; }

	void setText(const char*);
	void setFont(const char*);
	void setFontSize(int);
	//void setTexture(SDL_Texture*);
	void setColor(SDL_Color);


};

