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
		if (id_texture == -1)
			App->render->Blit(App->gui->GetAtlas(), box_elem.x - App->render->camera.x / App->win->GetScale(), box_elem.y - App->render->camera.y / App->win->GetScale(), &rect_texture);

		else
		{
			if (rect_texture.w == 0 || rect_texture.h == 0)
			{
				App->render->Blit(App->gui->GetAtlas(), ceil( box_elem.x - App->render->camera.x / App->win->GetScale()), box_elem.y - App->render->camera.y / App->win->GetScale());
			}
			else
			{
				App->render->Blit(App->gui->GetAtlas(), (box_elem.x - App->render->camera.x / App->win->GetScale()), box_elem.y - App->render->camera.y / App->win->GetScale(), &rect_texture);
			}
		}
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