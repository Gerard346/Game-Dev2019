#ifndef _GUI_STRING_H
#define _GUI_STRING_H

#include "GUIElement.h"
#include "j1Fonts.h"
#include "p2Point.h"

class GUI_String : public GUIElement
{
public:

	GUI_String();
	~GUI_String();

private:
	p2SString		text;
	_TTF_Font* font = nullptr;
	SDL_Texture* texture_text = nullptr;
	SDL_Color		color;

public:

	bool Draw(bool debug);

	char* GetText()const;
	void SetText(char* str);
	void SetFont(_TTF_Font* fonts);
	void SetColor(SDL_Color col);

	iPoint GetPixelSize()const;
	uint GetPixelWidth()const;
	uint GetPixelHeight()const;

	bool GenerateTexture();

	void FitBox();

};

#endif 
