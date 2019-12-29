#ifndef _MAINMENU_H__
#define _MAINMENU_H__

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "j1Timer.h"
#include "p2List.h"

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
	GUI_Scroll* scroll_credits = nullptr;
	GUIElement* menu_credits = nullptr;
	GUI_Image*  img_credits = nullptr;
	GUI_Window* text_box = nullptr;
	GUIElement* text_store = nullptr;
	GUI_String* license_name = nullptr;

	GUI_String* copyright = nullptr;
	GUI_String* copyright2 = nullptr;

	GUI_String* parag1_line1 = nullptr;
	GUI_String* parag1_line2 = nullptr;
	GUI_String* parag1_line3 = nullptr;
	GUI_String* parag1_line4 = nullptr;
	GUI_String* parag1_line5 = nullptr;
	GUI_String* parag1_line6 = nullptr;

	GUI_String* parag2_line1 = nullptr;
	GUI_String* parag2_line2 = nullptr;

	GUI_String* parag3_line1 = nullptr;
	GUI_String* parag3_line2 = nullptr;
	GUI_String* parag3_line3 = nullptr;
	GUI_String* parag3_line4 = nullptr;
	GUI_String* parag3_line5 = nullptr;
	GUI_String* parag3_line6 = nullptr;
	GUI_String* parag3_line7 = nullptr;
	GUI_Button* back_to_menu_credits = nullptr;

	p2List<iPoint> credits_positions;

	//Settings
	//Sliders FX & music
	//credits

};
#endif