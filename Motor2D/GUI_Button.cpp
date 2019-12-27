#include "j1Render.h"
#include "j1App.h"
#include "j1Audio.h"
#include "j1Gui.h"
#include "j1Input.h"
#include "p2Log.h"
#include "GUI_Button.h"

GUI_Button::GUI_Button() :GUIElement(BUTTON)
{
}

GUI_Button::~GUI_Button()
{
}

bool GUI_Button::Start()
{

	return true;
}

bool GUI_Button::Draw(bool debug)
{
	if (debug) {
		DebugDraw(App->gui->BROWN);
	}
	else {
		switch (button_state) {
		case BUTTON_ON:
			button_on.Draw(debug);
			break;
		case BUTTON_OFF:
			button_off.Draw(debug);
			break;
		case BUTTON_HOVER:
			button_hover.Draw(debug);
			break;
		}
	}
	DrawChilds(debug);
	return true;

}

void GUI_Button::HandleInput()
{
	if (input_target == nullptr)
	{
		LOG("Error input target not set");
		return;
	}

	if (button_state == BUTTON_ON) {
		timer += App->Getdt();

		if (timer > button_on_delay) {
			timer = 0.0f;
			button_state = BUTTON_HOVER;
		}
		return;
	}

	bool mouse_in = MouseIn();

	if (mouse_in)
	{
		input_target->HandleInput(this, MOUSE_IN);
		button_state = BUTTON_HOVER;
	}
	else
	{
		input_target->HandleInput(this, MOUSE_OUT);
		button_state = BUTTON_OFF;
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (mouse_in && layer == App->gui->GetTopLayer())
		{
			input_target->HandleInput(this, MOUSE_LEFT_DOWN);
			button_state = BUTTON_ON;
		}
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
	{
		if (mouse_in && layer == App->gui->GetTopLayer())
		{
			input_target->HandleInput(this, MOUSE_LEFT_UP);
			button_state = BUTTON_OFF;
		}
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		if (mouse_in && layer == App->gui->GetTopLayer())
		{
			input_target->HandleInput(this, MOUSE_RIGHT_DOWN);
			button_state = BUTTON_ON;
		}
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
	{
		if (mouse_in && layer == App->gui->GetTopLayer())
		{
			input_target->HandleInput(this, MOUSE_RIGHT_UP);
			button_state = BUTTON_OFF;
		}
	}
}

void GUI_Button::SetButtonOn(SDL_Rect rect_button, int texture_button)
{
	button_on.SetRectTexture(rect_button);
	button_on.SetIdTexture(texture_button);
}

void GUI_Button::SetButtonOff(SDL_Rect rect_button, int texture_button)
{
	button_off.SetRectTexture(rect_button);
	button_off.SetIdTexture(texture_button);
}

void GUI_Button::SetButtonHover(SDL_Rect rect_button, int texture_button)
{
	button_hover.SetRectTexture(rect_button);
	button_hover.SetIdTexture(texture_button);
}

void GUI_Button::SetBlocked()
{
	interactive = false;
	button_state = BUTTON_ON;
}

void GUI_Button::SetBoxElem(SDL_Rect elem)
{
	box_elem = elem;
	button_on.SetBoxElem(elem);
	button_off.SetBoxElem(elem);
	button_hover.SetBoxElem(elem);
}

void GUI_Button::SetPosElem(iPoint position)
{
	box_elem.x = position.x;
	box_elem.y = position.y;
	button_on.SetPosElem(position);
	button_off.SetPosElem(position);
	button_hover.SetPosElem(position);
}

void GUI_Button::SetDelayButton(float delay)
{
	button_on_delay = delay;
}