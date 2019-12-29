#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "PugiXml/src/pugixml.hpp"

struct SDL_Texture;

struct GUI_Image;
struct GUI_Button;
struct GUI_Window;
struct GUI_String;
struct GUI_Scroll;

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

	void Activate();
	void Desactivate();

	GUIElement* GetSceneGUI();
	p2SString music_path;

	void HandleInput(GUIElement* input, TypeInput type_input);

private:
	int button_fx = 0;
	float last_loaded = 1.0f;
	float current_time_load = 0;

	SDL_Texture* img = nullptr;
	bool is_paused = false;

	GUI_Image* imgsa = nullptr;
	GUIElement* scene_gui = nullptr;
	GUI_Button* button = nullptr;
	GUI_Button* button2 = nullptr;

	//GUI in-game

	GUI_Image* gui_game = nullptr;
	GUI_Image* heart_1 = nullptr;
	GUI_Image* heart_2 = nullptr;
	GUI_Image* heart_3 = nullptr;

	//MenuPause
	
	GUI_Window* window_pause = nullptr;
	GUI_Image* window_pause_image = nullptr;
	GUIElement* menu_pause_buttons = nullptr;
	GUI_Button* bt_resume = nullptr;
	GUI_String* str_volume_music_pause = nullptr;
	GUI_Scroll* slider_vol_music_pause = nullptr;
	GUI_String* str_volume_fx_pause = nullptr;
	GUI_Scroll* slider_vol_fx_pause = nullptr;
	GUI_Button* go_back_to_main_menu = nullptr;

};

#endif // __j1SCENE_H__