#include "MainMenu.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1FadeToBlack.h"
#include "j1Gui.h"
#include "GUIElement.h"
#include "j1Fonts.h"
#include "GUI_Button.h"
#include "GUI_Image.h"
#include "GUI_Window.h"
#include "GUI_String.h"

MainMenu::MainMenu() : j1Module()
{
	name.create("main_menu");
}

MainMenu::~MainMenu()
{}

void MainMenu::HandleInput(GUIElement * input, TypeInput type_input)
{
	if (App->fade->isFading() == false) {
		if (input == bt_play) {
			if (type_input == MOUSE_RIGHT_DOWN) {
				App->ActivateGame();
			}
			if (type_input == MOUSE_LEFT_DOWN) {
				App->ActivateGame();
			}
		}
		else if (input == bt_continue) {
			if (type_input == MOUSE_RIGHT_DOWN) {
				App->ActivateGame();
				App->WantToLoad();
			}
			if (type_input == MOUSE_LEFT_DOWN) {
				App->ActivateGame();
				App->WantToLoad();
			}
		}
		else if (input == bt_settings) {

		}
		else if (input == bt_credits) {

		}
		else if (input == bt_exit) {

		}
		else if (input == bt_web) {
			if (type_input == MOUSE_RIGHT_DOWN) {
				ShellExecuteA(NULL, "open", "google.es", NULL, NULL, SW_SHOWNORMAL);
			}
			if (type_input == MOUSE_LEFT_DOWN) {
				ShellExecuteA(NULL, "open", "google.es", NULL, NULL, SW_SHOWNORMAL);
			}
		}
		else if (input == bt_back) {

		}
	}
}



bool MainMenu::Awake(const pugi::xml_node & node)
{
	return true;
}

bool MainMenu::Start()
{
	App->audio->PlayMusic(MusicType::MAIN_MENU_MUSIC);

	menu_scene = App->gui->GenerateElemGUI(TypeGUI::UNDEFINED);
	menu_scene->SetLocalRect({ 0,0,App->render->camera.w / (int)App->win->GetScale(),App->render->camera.h / (int)App->win->GetScale() });

	int id_tex_bck = App->gui->AddTexture(App->tex->Load("gui/main_menu.png"));

	bck_menu = (GUI_Image*)App->gui->GenerateElemGUI(TypeGUI::IMAGE);
	bck_menu->SetIdTexture(id_tex_bck);
	bck_menu->SetRectTexture({ 0,0, 512,384 });
	bck_menu->SetLocalRect({ 0,0, App->render->camera.w / (int)App->win->GetScale(), App->render->camera.h / (int)App->win->GetScale() });
	
	menu_scene->AddChild(bck_menu);

	window_menu = (GUI_Window*)App->gui->GenerateElemGUI(TypeGUI::WINDOW);
	window_menu->SetLocalRect({ 145,41,222,299 });
	menu_scene->AddChild(window_menu);

	str_hi = (GUI_String*)App->gui->GenerateElemGUI(TypeGUI::TEXT);
	str_hi->SetColor(App->gui->White);
	str_hi->SetText("HOLA");
	str_hi->SetLocalPos({ 0, 0 });
	menu_scene->AddChild(str_hi);
	App->gui->SetSceneGUI(menu_scene);

	
	return true;
}

bool MainMenu::PreUpdate()
{
	return true;
}

bool MainMenu::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {
		App->gui->SetDebug();
	}
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) {
		App->ActivateGame();
	}
	return true;
}

bool MainMenu::PostUpdate()
{
	return true;
}

bool MainMenu::CleanUp()
{
	return true;
}

void MainMenu::Activate()
{
	active = true;
	App->gui->SetSceneGUI(menu_scene);
}

void MainMenu::Desactivate()
{
	active = false;
}

bool MainMenu::Load(const pugi::xml_node&)
{
	return true;
}

bool MainMenu::Save(pugi::xml_node&)
{
	return true;
}
