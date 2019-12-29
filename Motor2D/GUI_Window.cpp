#include "GUI_Window.h"
#include "j1Gui.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "p2Log.h"

GUI_Window::GUI_Window() : GUIElement(WINDOW)
{
}

void GUI_Window::HandleInput()
{
	if (interactive == false) return;
	if (window_state == WINDOW_IDLE)
	{
		if (layer != App->gui->GetTopLayer())return;

		bool mouse_in = MouseIn();

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			if (mouse_in && layer == App->gui->GetTopLayer())
			{
				window_state = WINDOW_ON_DRAG;

				int x2, y2;
				App->input->GetMousePosition(x2, y2);
				
				click_delta.x = GetScreenPos().x - x2;
				click_delta.y = GetScreenPos().y - y2;
			}
		}
	}
	else
	{
		int x2, y2;
		App->input->GetMousePosition(x2, y2);
		SetLocalPos(iPoint(click_delta.x + x2, click_delta.y + y2));

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		{
			window_state = WINDOW_IDLE;
			click_delta = { 0,0 };
		}
	}
}
