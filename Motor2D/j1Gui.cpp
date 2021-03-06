#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "GUIElement.h"
#include "GUI_Image.h"
#include "GUI_Button.h"
#include "GUI_Window.h"
#include "GUI_String.h"
#include "GUI_Scroll.h"

#include "Brofiler/Brofiler.h"
j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(const pugi::xml_node& node)
{
	LOG("Loading GUI atlas");
	bool ret = true;
	if (node != nullptr) {
		atlas_file_name = node.child("atlas").attribute("file").as_string("");
	}
	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());
	textures.add(atlas);
	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	CalculateLayer();
	
	return true;
}

bool j1Gui::Update(float dt)
{
	BROFILER_CATEGORY("GUI Update", Profiler::Color::AliceBlue);

	gui_scene->Update(dt);
	gui_console->Update(dt);

	return true;
}
// Called after all Updates
bool j1Gui::PostUpdate()
{
	BROFILER_CATEGORY("GUI Update", Profiler::Color::AliceBlue);

	gui_scene->Draw(debug);
	gui_console->Draw(debug);

	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	
	gui_scene->CleanUp();

	return true;
}

GUIElement* j1Gui::GenerateElemGUI(TypeGUI type)
{
	GUIElement* ret = nullptr;

	switch (type)
	{
	case UNDEFINED:
		ret = new GUIElement();
		break;
	case IMAGE:
		ret = new GUI_Image();
		break;
	case BUTTON:
		ret = new GUI_Button();
		break;
	case WINDOW:
		ret = new GUI_Window();
		break;
	case TEXT:
		ret = new GUI_String();
		break;
	case SLIDER_UI:
		ret = new GUI_Scroll();
		break;
	}

	return ret;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}


int j1Gui::GetTopLayer()
{
	return top_layer;
}

int j1Gui::CalculateLayer()
{
	top_layer = 0;
	CalculateTopLayer(gui_scene, top_layer);
	CalculateTopLayer(gui_console, top_layer);

	return top_layer;
}

SDL_Texture* j1Gui::GetTexture(int id) const
{
	if (id >= textures.count())
	{
		LOG("Error texture Id out of range");
		return nullptr;
	}
	return textures.At(id)->data;
}


int j1Gui::AddTexture(SDL_Texture* new_tex)
{
	if (new_tex == nullptr)
	{
		LOG("Error texture null in AddTexture");
		return -1;
	}

	textures.add(new_tex);
	return textures.count() - 1;
}

void j1Gui::SetDebug()
{
	debug = !debug;
}

void j1Gui::SetConsoleGUI(GUIElement* elem)
{
	gui_console = elem;
}

int j1Gui::CalculateTopLayer(const GUIElement* element, int& layer)
{
	for (int i = 0; i < element->childs.count(); i++)
	{
		GUIElement* current_element = element->childs.At(i)->data;

		CalculateTopLayer(current_element, layer);
		if (current_element->GetLayer() > layer && current_element->GetElemActive() && current_element->GetElemInteractive() && current_element->MouseIn())
		{
			layer = current_element->GetLayer();
		}
	}
	
	return top_layer = layer;
}

void j1Gui::SetSceneGUI(GUIElement* elem)
{
	gui_scene = elem;
}
