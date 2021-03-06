#ifndef __GUI_IMAGE_H__
#define __GUI_IMAGE_H__
#include "GUIElement.h"

class GUI_Image : public GUIElement
{
public:

	GUI_Image();
	~GUI_Image();

private:

	SDL_Rect  rect_texture = { 0,0,0,0 };
	int		  id_texture = -1;
	Uint8 alpha = 255;
	int alpha_min = 200;
	bool alpha_off = true;
	bool is_button_image = false;

public:

	bool Draw(bool debug);

	SDL_Rect GetRectTexture()const;
	void SetRectTexture(SDL_Rect new_rect_texture);
	int GetIdTexture()const;
	void SetIdTexture(int new_id);

	void SetAsButtonImage();
	bool IsButtonImage()const;
	void FitBox();
};
#endif