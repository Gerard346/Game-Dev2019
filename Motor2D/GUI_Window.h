#ifndef  _GUI_WINDOW_H_
#define _GUI_WINDOW_H_

#include "GUIElement.h"

enum State_Window {
	WINDOW_ON_DRAG,
	WINDOW_IDLE
};

class GUI_Window : public GUIElement 
{
public: 
	GUI_Window();
	void HandleInput() override;

private:
	
	iPoint click_delta = { 0,0 };
	State_Window window_state = WINDOW_IDLE;
};
#endif // ! _GUI_WINDOW_H_

