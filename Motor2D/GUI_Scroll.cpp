#include "GUI_Scroll.h"
#include "j1Gui.h"
#include "j1Input.h"
#include "j1App.h"
#include "j1Render.h"
#include "p2Log.h"

GUI_Scroll::GUI_Scroll() :GUIElement(SLIDER_UI)
{
	scroll_image.SetParent(this);
	scroll_image.SetLayer(layer);

	scroll_background.SetParent(this);
	scroll_background.SetLayer(layer);

}

GUI_Scroll::~GUI_Scroll()
{
}

bool GUI_Scroll::Draw(bool debug)
{
	if (true)
	{
		scroll_background.DebugDraw(App->gui->UI_YELLOW);
		scroll_image.DebugDraw(App->gui->UI_BROWN);
	}
	else
	{
		scroll_background.Draw(false);
		scroll_image.Draw(false);
	}
	DrawChilds(debug);

	return true;
}

void GUI_Scroll::HandleInput()
{
	if (input_target == nullptr)
	{
		LOG("Error input target not set");
		return;
	}

	bool mouse_in = scroll_image.MouseIn();

	if (mouse_in && layer == App->gui->GetTopLayer())
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			scroll_selected = true;

			int x, y;
			App->input->GetMousePosition(x, y);

			drag_delta.x = scroll_image.GetScreenPos().x - x;
			drag_delta.y = scroll_image.GetScreenPos().y - y;
		}
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
	{
		scroll_selected = false;
	}

	if (scroll_selected)
	{
		int x, y;
		App->input->GetMousePosition(x, y);
		MoveScroll(x, y);
		input_target->HandleInput(this, TypeInput::SCROLL_MOVE);
	}
}

void GUI_Scroll::MoveScroll(int x_position, int y_position)
{
	if (scroll_image.MouseIn() || scroll_selected == true)
	{
		scroll_selected = true;
	}
	if (scroll_selected)
	{
		if (scroll_type == VERTICAL)
		{
			SDL_Rect new_rect = scroll_image.GetScreenRect();
			new_rect.y = y_position + drag_delta.y;

			SDL_Rect back_rect = scroll_background.GetScreenRect();

			if (scroll_image.RectCheck(new_rect, scroll_background.GetScreenRect(), false) == false)
			{
				if (new_rect.y < back_rect.y)
				{
					new_rect.y = back_rect.y;
				}
				else if (new_rect.y + new_rect.h > back_rect.y + back_rect.h)
				{
					new_rect.y = back_rect.y + back_rect.h - new_rect.h;
				}
			}

			scroll_image.AddPosition(0,  new_rect.y - scroll_image.GetScreenRect().y);

			//Update scroll value
			current_value = ((scroll_image.GetScreenRect().y - scroll_background.GetScreenRect().y) * max_value) / (float)(scroll_background.GetScreenRect().h - scroll_image.GetScreenRect().h);
		}
		else if (scroll_type == HORIZONTAL)
		{
			SDL_Rect new_rect = scroll_image.GetScreenRect();
			new_rect.x = x_position + drag_delta.x;

			SDL_Rect back_rect = scroll_background.GetScreenRect();

			if (scroll_image.RectCheck(new_rect, scroll_background.GetScreenRect(), true) == false)
			{
				if (new_rect.x < back_rect.x)
				{
					new_rect.x = back_rect.x;
				}
				else if (new_rect.x + new_rect.w > back_rect.x + back_rect.w)
				{
					new_rect.x = back_rect.x + back_rect.w - new_rect.w;
				}
			}

			scroll_image.AddPosition(new_rect.x - scroll_image.GetScreenRect().x, 0);

			//Update scroll value
			current_value = ((scroll_image.GetScreenRect().x - scroll_background.GetScreenRect().x) * max_value) / (float)(scroll_background.GetScreenRect().w - scroll_image.GetScreenRect().w);
		}
	}

}

void GUI_Scroll::SetTypeScroll(Scroll_TYPE type)
{
	scroll_type = type;
}

void GUI_Scroll::SetMaxValue(float value)
{
	max_value = value;
}

void GUI_Scroll::SetScrollImage(SDL_Rect rect_image, int texture_scroll_image)
{
	scroll_image.SetRectTexture(rect_image);
	scroll_image.SetIdTexture(texture_scroll_image);
	scroll_image.FitBox();

	if (scroll_type == HORIZONTAL)
	{
		scroll_image.SetLocalPos({ 0, (int)(-scroll_background.GetScreenRect().h * 0.5f) });
	}
	else
	{
		scroll_image.SetLocalPos({(int)(-scroll_background.GetScreenRect().w * 0.5f), 0 });
	}
}

void GUI_Scroll::SetScrollBackground(SDL_Rect rect_background, int texture_scroll_background)
{
	scroll_background.SetRectTexture(rect_background);
	scroll_background.SetIdTexture(texture_scroll_background);
	scroll_background.FitBox();
}

void GUI_Scroll::SetScrollSelected(bool selected)
{
	scroll_selected = selected;
}

GUI_Image GUI_Scroll::GetScrollBackground() const
{
	return scroll_background;
}

GUI_Image GUI_Scroll::GetScrollImage() const
{
	return scroll_image;
}

float GUI_Scroll::GetMaxValue() const
{
	return max_value;
}

float GUI_Scroll::GetCurrentValue() const
{
	return current_value;
}

void GUI_Scroll::SetCurrentValue(float value)
{
	if (scroll_type == VERTICAL)
	{

	}
	else
	{
		int x = (value / max_value) * (scroll_background.GetLocalRect().w + scroll_background.GetLocalPos().x - scroll_image.GetLocalRect().w);
		scroll_image.SetLocalPos({ x, scroll_image.GetLocalPos().y });
	}
	current_value = value;
}

