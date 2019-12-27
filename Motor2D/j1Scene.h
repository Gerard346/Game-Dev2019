#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "PugiXml/src/pugixml.hpp"

struct SDL_Texture;

struct GUI_Image;
struct GUI_Button;
struct GUI_Window;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(const pugi::xml_node& node) override;

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	void ChangeScene(const float) const;
	// Called before quitting
	bool CleanUp();

	bool GetPause();


private:

	p2SString music_path;
	SDL_Texture* img = nullptr;
	bool is_paused = false;

	GUI_Image* imgsa = nullptr;
	GUIElement* scene_gui = nullptr;
	GUI_Button* button = nullptr;
	GUI_Window* window = nullptr;
};

#endif // __j1SCENE_H__