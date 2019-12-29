#include "GUI_String.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "j1Textures.h"
#include "j1Window.h"
GUI_String::GUI_String() : GUIElement(TEXT)
{

}

GUI_String::~GUI_String()
{
}

bool GUI_String::Draw(bool debug)
{
	if (active_elem == false)return true;

	if (debug)
		DebugDraw(App->gui->RED);
	else
	{
		SDL_Rect screen_rect = parent->GetScreenRect();

		screen_rect.x *= (int)App->win->GetScale();
		screen_rect.y *= (int)App->win->GetScale();
		screen_rect.w *= (int)App->win->GetScale();
		screen_rect.h *= (int)App->win->GetScale();

		SDL_RenderSetViewport(App->render->renderer, &screen_rect);

		App->render->Blit(texture_text, box_elem.x - App->render->camera.x / App->win->GetScale(), box_elem.y - App->render->camera.y / App->win->GetScale());

		SDL_RenderSetViewport(App->render->renderer, NULL);
	}

	DrawChilds(debug);
	return true;
}

char* GUI_String::GetText() const
{
	return (char*)text.GetString();
}

void GUI_String::SetText(char* str)
{
	text = str;
	if (str != nullptr)
	{
		App->tex->UnLoad(texture_text);
		texture_text = App->font->Print(text.GetString(), color, font);
	}
}

void GUI_String::SetFont(_TTF_Font* fonts)
{
	font = fonts;
}

void GUI_String::SetColor(SDL_Color col)
{
	color = col;
}

iPoint GUI_String::GetPixelSize() const
{
	int w = 0, h = 0;
	App->font->CalcSize(text.GetString(), w, h, font);
	return iPoint(w, h);
}

uint GUI_String::GetPixelWidth() const
{
	int w = 0, h = 0;
	App->font->CalcSize(text.GetString(), w, h, font);
	return uint(w);
}

uint GUI_String::GetPixelHeight() const
{
	int w = 0, h = 0;
	App->font->CalcSize(text.GetString(), w, h, font);
	return uint(h);
}

bool GUI_String::GenerateTexture()
{
	if (strlen(text.GetString()) > 0)
	{
		texture_text = App->font->Print(text.GetString(), color, font);
		return true;
	}
	return false;
}

void GUI_String::FitBox()
{
	iPoint size = GetPixelSize();
	box_elem.w = size.x;
	box_elem.h = size.y;
}
