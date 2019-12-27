#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "GUIElement.h"
#include "GUI_Image.h"

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

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	for (int i = 0; i < gui_scenes.count(); i++) {
		gui_scenes[i]->Draw(debug);
	}
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	for (int i = 0; i < gui_scenes.count(); i++) {
		gui_scenes[i]->CleanUp();
	}
	gui_scenes.clear();
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
	}
	return ret;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}


void j1Gui::AddSceneGUI(GUIElement* elem)
{
	gui_scenes.add(elem);
}
