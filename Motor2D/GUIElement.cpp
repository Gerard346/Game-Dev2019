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
	if (active_elem == false)return true;

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
	if (active_elem == false)return true;

	if (debug)
		DebugDraw(App->gui->LIGHT_GREEN);
		
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

SDL_Rect GUIElement::GetScreenRect()const
{
	iPoint parent_screen_pos = parent != nullptr ? parent->GetScreenPos() : iPoint(0,0);
	
	return { parent_screen_pos.x + box_elem.x, parent_screen_pos.y + box_elem.y, box_elem.w, box_elem.h };
}

SDL_Rect GUIElement::GetLocalRect()const
{
	return box_elem;
}

iPoint GUIElement::GetScreenPos()const
{
	iPoint parent_screen_pos = parent != nullptr ? parent->GetScreenPos() : iPoint(0, 0);

	return iPoint(box_elem.x + parent_screen_pos.x, box_elem.y + parent_screen_pos.y);
}

iPoint GUIElement::GetLocalPos()const
{
	return iPoint(box_elem.x, box_elem.y);
}

void GUIElement::AddChild(GUIElement* elem, int child_layer)
{
	if (child_layer == 0)
	{
		elem->SetLayer(layer + 1);
	}
	else elem->SetLayer(child_layer);

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

void GUIElement::AddPosition(int x, int y)
{
	box_elem.x += x;
	box_elem.y += y;

}

void GUIElement::SetInputTarget(j1Module* module)
{
	input_target = module;
}

bool GUIElement::RectIsIn(const SDL_Rect& target, int x_vel, int y_vel, bool x_axis) const
{
	bool ret = false;

	SDL_Rect screen_rect = GetScreenRect();

	if (x_axis == false)
	{

		ret = (target.y <= screen_rect.y + y_vel && (target.y + target.h) >= (screen_rect.y + screen_rect.h + y_vel));

	}
	else
	{
		ret = (target.x <= screen_rect.x + x_vel && (target.x + target.w) >= (screen_rect.x + screen_rect.w + x_vel));
	}
	return ret;
}

bool GUIElement::RectCheck(const SDL_Rect& in_target, const SDL_Rect& out_target, bool x_axis) const
{
	bool ret = false;

	if (x_axis == false)
	{

		ret = (in_target.y <= out_target.y && (in_target.y + in_target.h) >= (out_target.y + out_target.h));

	}
	else
	{
		ret = (in_target.x <= out_target.x && (in_target.x + in_target.w) >= (out_target.x + out_target.w));
	}

	return ret;
}

void GUIElement::SetElemsInteractiveForChilds(bool ret)
{
	int get_childs = childs.count();
	for (int i = 0; i < get_childs; i++) {
		if (childs[i]->GetElemActive() == true) {
			childs[i]->SetElemInteractive(ret);
		}
	}
}