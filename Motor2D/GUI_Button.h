#ifndef __GUI_BUTTON_H__
#define __GUI_BUTTON_H__
#include "GUIElement.h"
#include "GUI_Image.h"

enum State_Button {
	BUTTON_OFF,
	BUTTON_ON,
	BUTTON_HOVER
};

class GUI_Button :public GUIElement
{
public:
	GUI_Button();
	~GUI_Button();
	bool Start();
	bool Draw(bool debug);
	void HandleInput() override;

private:

	float button_on_delay = 0.0f;
	float timer = 0.0f;
	GUI_Image button_on;
	GUI_Image button_off;
	GUI_Image button_hover;

	State_Button button_state = BUTTON_OFF;

public:
	void SetButtonOn(SDL_Rect rect_button, int texture_button = -1);
	void SetButtonOff(SDL_Rect rect_button, int texture_button = -1);
	void SetButtonHover(SDL_Rect rect_button, int texture_button = -1);
	void SetBlocked();
	void SetBoxElem(SDL_Rect elem);
	void SetPosElem(iPoint position);
	void SetDelayButton(float delay);
};
#endif