#include "j1Render.h"
#include "j1Input.h"
#include "j1App.h"
#include "p2Log.h"
#include "j1Gui.h"
#include "GUIElement.h"
#include "j1Window.h"

GUIElement::GUIElement(TypeGUI type_gui) : type_gui(type_gui)
{
}

GUIElement::~GUIElement()
{
}

bool GUIElement::Update(float dt)
{
	HandleInput();

	UpdateChilds(dt);
	return true;
}

bool GUIElement::UpdateChilds(float dt)
{
	for (int i = 0; i < childs.count(); i++) {
		childs[i]->Update(dt);
	}
	return true;
}

void GUIElement::HandleInput()
{
}

bool GUIElement::CleanUp()
{
	CleanUpChilds();
	return true;
}

bool GUIElement::CleanUpChilds()
{
	int get_childs = childs.count();
	for (int i = 0; i < get_childs; i++) {
		childs[i]->CleanUp();
	}

	for (int i = 0; i < get_childs; i++) {
		RELEASE(childs[i]);
	}
	childs.clear();

	return true;
}

bool GUIElement::Draw(bool debug)
{
	if (debug)
		DebugDraw(App->gui->YELLOW);
		
	DrawChilds(debug);
	return true;
}

void GUIElement::DebugDraw(SDL_Color color)
{
	SDL_Rect screen_rect = GetScreenRect();

	if (interactive)
	{
		App->render->DrawQuad({ screen_rect.x - App->render->camera.x / (int)App->win->GetScale() , screen_rect.y - App->render->camera.y / (int)App->win->GetScale(), screen_rect.w, screen_rect.h }, color.r, color.g, color.b, color.a);

	}
	else
	{
		App->render->DrawQuad({ screen_rect.x - App->render->camera.x / (int)App->win->GetScale() , screen_rect.y - App->render->camera.y / (int)App->win->GetScale(), screen_rect.w, screen_rect.h }, 100, 100, 100, 100);

	}

}

bool GUIElement::DrawChilds(bool debug)
{
	int get_childs = childs.count();
	for (int i = 0; i < get_childs; i++) {
		if (childs[i]->GetElemActive() == true) {
			childs[i]->Draw(debug);
		}
	}
	return true;
}

void GUIElement::SetLocalPos(iPoint target_point)
{
	box_elem.x = target_point.x;
	box_elem.y = target_point.y;
}

void GUIElement::SetLocalRect(SDL_Rect elem)
{
	box_elem = elem;
}

SDL_Rect GUIElement::GetScreenRect()
{
	iPoint parent_screen_pos = parent != nullptr ? parent->GetScreenPos() : iPoint(0,0);
	
	return { parent_screen_pos.x + box_elem.x, parent_screen_pos.y + box_elem.y, box_elem.w, box_elem.h };
}

SDL_Rect GUIElement::GetLocalRect()
{
	return box_elem;
}

iPoint GUIElement::GetScreenPos()
{
	iPoint parent_screen_pos = parent != nullptr ? parent->GetScreenPos() : iPoint(0, 0);

	return iPoint(box_elem.x + parent_screen_pos.x, box_elem.y + parent_screen_pos.y);
}

iPoint GUIElement::GetLocalPos()
{
	return iPoint(box_elem.x, box_elem.y);
}

void GUIElement::AddChild(GUIElement* elem, int child_layer)
{
	if (child_layer == 0)
	{
		elem->SetLayer(layer + 1);
	}
	childs.add(elem);
	
	elem->SetParent(this);
}

void GUIElement::SetParent(const GUIElement* elem)
{
	parent = (GUIElement*)elem;
}

GUIElement* GUIElement::GetParent() const
{
	return parent;
}

void GUIElement::SetLayer(int lay)
{
	layer = lay;
}

int GUIElement::GetLayer() const
{
	return layer;
}

void GUIElement::SetElemActive(bool active)
{
	active_elem = active;
}

bool GUIElement::GetElemActive() const
{
	return active_elem;
}

bool GUIElement::GetElemInteractive() const
{
	return interactive;
}

void GUIElement::SetElemInteractive(bool inter)
{
	interactive = inter;
}

bool GUIElement::MouseIn()
{
	int x2, y2;
	App->input->GetMousePosition(x2, y2);

	SDL_Rect screen_rect = GetScreenRect();
	return (screen_rect.x  < x2 && (screen_rect.x + screen_rect.w) > x2 && (screen_rect.y) < y2 && (screen_rect.y + screen_rect.h) > y2);
}

void GUIElement::SetInputTarget(j1Module* module)
{
	input_target = module;
}
