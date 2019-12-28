#ifndef __GUI_SCROLL_H__
#define __GUI_SCROLL_H__

#include "GUIElement.h"
#include "GUI_Image.h"

enum Scroll_TYPE
{
	VERTICAL = 0,
	HORIZONTAL
};

class GUI_Scroll :public GUIElement
{
public:

	GUI_Scroll();
	~GUI_Scroll();

	bool Draw(bool debug);
	void HandleInput() override;

private:

	uint			scroll_position = 0;
	float			max_value = 0.0f;
	float			current_value = 0.0f;
	Scroll_TYPE		scroll_type;
	GUI_Image		scroll_image;
	GUI_Image		scroll_background;
	bool			scroll_selected = false;
	iPoint			drag_delta = { 0,0 };

public:

	void MoveScroll(int x_motion, int y_motion);

	void SetTypeScroll(Scroll_TYPE type);
	void SetMaxValue(float value);
	void SetScrollImage(SDL_Rect rect_image, int texture_scroll_image = -1);
	void SetScrollBackground(SDL_Rect rect_background, int texture_scroll_background = -1);
	void SetScrollSelected(bool selected);

	GUI_Image GetScrollBackground()const;
	GUI_Image GetScrollImage()const;
	float GetMaxValue()const;
	float GetCurrentValue() const;
	void SetCurrentValue(float value);
};

#endif 