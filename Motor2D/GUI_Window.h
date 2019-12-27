#ifndef  _GUI_WINDOW_H_
#define _GUI_WINDOW_H_

#include "GUIElement.h"

class GUI_Window : public GUIElement 
{
public: 
	GUI_Window();
	void HandleInput() override;
};
#endif // ! _GUI_WINDOW_H_

