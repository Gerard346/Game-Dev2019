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
struct _TTF_Font;

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
	GUIElement* menu_scene = nullptr;
	GUI_Image* bck_menu = nullptr;
	//Main Menu
	GUI_Window* window_menu = nullptr;
	GUI_Button* bt_play = nullptr;
	GUI_Button* bt_continue = nullptr;
	GUI_Button* bt_settings = nullptr;
	GUI_Button* bt_credits = nullptr;
	GUI_Button* bt_exit = nullptr;
	GUI_Button* bt_web = nullptr;
	GUI_Button* bt_back = nullptr;

	//Settings
	//Sliders FX & music
	//credits

};
#endif