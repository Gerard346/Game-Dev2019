#include "j1Render.h"
#include "j1App.h"
#include "j1Gui.h"
#include "GUI_Image.h"
#include "j1Window.h"
#include"p2Log.h"

GUI_Image::GUI_Image() :GUIElement(IMAGE) {

}

GUI_Image::~GUI_Image()
{
}

bool GUI_Image::Draw(bool debug)
{
	if (debug)
	{
		DebugDraw(App->gui->LIGHT_GREEN);
	}
	else
	{
		SDL_Rect screen_rect = is_button_image ? parent->parent->GetScreenRect() : parent->GetScreenRect();

		screen_rect.x *= (int)App->win->GetScale();
		screen_rect.y *= (int)App->win->GetScale();
		screen_rect.w *= (int)App->win->GetScale();
		screen_rect.h *= (int)App->win->GetScale();

		SDL_RenderSetViewport(App->render->renderer, &screen_rect);

		if (id_texture == -1 || rect_texture.w == 0 || rect_texture.h == 0)
		{
			App->render->Blit(App->gui->GetAtlas(), box_elem.x - App->render->camera.x / App->win->GetScale(), box_elem.y - App->render->camera.y / App->win->GetScale(), &rect_texture);
		}
		else
		{
			App->render->Blit(App->gui->GetAtlas(), (box_elem.x - App->render->camera.x / App->win->GetScale()), box_elem.y - App->render->camera.y / App->win->GetScale(), &rect_texture);
		}

		SDL_RenderSetViewport(App->render->renderer, NULL);
	}

	DrawChilds(debug);
	return true;
}

SDL_Rect GUI_Image::GetRectTexture() const
{
	return rect_texture;
}

void GUI_Image::SetRectTexture(SDL_Rect new_rect_texture)
{
	rect_texture = new_rect_texture;
}

int GUI_Image::GetIdTexture() const
{
	return id_texture;
}

void GUI_Image::SetIdTexture(int new_id)
{
	id_texture = new_id;
}

void GUI_Image::SetAsButtonImage()
{
	is_button_image = true;
}

bool GUI_Image::IsButtonImage() const
{
	return is_button_image;
}
