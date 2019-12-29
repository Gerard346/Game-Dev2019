#ifndef _MAINMENU_H__
#define _MAINMENU_H__

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "j1Timer.h"

struct SDL_Texture;

struct GUI_Image;
struct GUI_Button;
struct GUI_Window;
struct GUI_String;
struct _TTF_Font;
struct GUI_Scroll;

class MainMenu : public j1Module {

public:

	MainMenu();

	virtual ~MainMenu();

	void HandleInput(GUIElement* input, TypeInput type_input);

private:

	bool Awake(const pugi::xml_node& node) override;

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();



public:
	bool Load(const pugi::xml_node&);
	bool Save(pugi::xml_node&);

	void Activate()override;
	void Desactivate()override;

private:
	int button_fx = 0;

	GUIElement* menu_scene = nullptr;

	//Main Menu
	GUIElement* menu_buttons = nullptr;
	GUI_Image* bck_menu = nullptr;
	GUI_Window* window_menu = nullptr;
	GUI_Image* window_img = nullptr;
	GUI_Button* bt_play = nullptr;
	GUI_Button* bt_continue = nullptr;
	GUI_Button* bt_settings = nullptr;
	GUI_Button* bt_credits = nullptr;
	GUI_Button* bt_exit = nullptr;
	GUI_Button* bt_web = nullptr;
	GUI_Button* bt_back = nullptr;
	GUI_String* website_str = nullptr;
	//Settings
	GUIElement* menu_settings = nullptr;
	GUI_String* str_volume_music = nullptr;
	GUI_String* str_volume_fx = nullptr;
	GUI_Scroll* slider_vol_music = nullptr;
	GUI_Scroll* slider_vol_fx = nullptr;
	GUI_Button* back_to_menu = nullptr;

	//Credit Menu
	GUIElement* menu_credits = nullptr;
	GUI_String* license = nullptr;
	GUI_Button* back_to_menu_credits = nullptr;

	//Settings
	//Sliders FX & music
	//credits

};
#endif