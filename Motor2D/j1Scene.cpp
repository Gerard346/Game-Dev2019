#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Colliders.h"
#include "j1Player.h"
#include "j1FadeToBlack.h"
#include "EntityManager.h"
#include "j1Pathfinding.h"
#include "j1Gui.h"
#include "GUIElement.h"
#include "j1Fonts.h"
#include "GUI_Button.h"
#include "GUI_Image.h"
#include "GUI_Window.h"
#include "MainMenu.h"
#include "GUI_Scroll.h"
#include "GUI_String.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(const pugi::xml_node& node)
{
	if (node != nullptr) {
		music_path = node.child_value("music");
	}
	LOG("Loading Scene");
	bool ret = true;
	
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->fade->FadeToColor(NULL, NULL, 1.5f);
	p2SString title("Last Soldier");

	App->win->SetTitle(title.GetString());

	int id_tex_pause_atlas = App->gui->AddTexture(App->tex->Load("gui/menu_pause.png"));

	//Creating Scene GUI
	scene_gui = App->gui->GenerateElemGUI(TypeGUI::UNDEFINED);
	scene_gui->SetLocalRect({ 0,0,App->render->camera.w / (int)App->win->GetScale(),App->render->camera.h / (int)App->win->GetScale() });
	scene_gui->SetElemInteractive(false);
	scene_gui->SetElemActive(true);

	//Game GUI

	gui_game = (GUI_Image*)App->gui->GenerateElemGUI(TypeGUI::IMAGE);
	gui_game->SetIdTexture(id_tex_pause_atlas);
	gui_game->SetRectTexture({ 226,100,194, 70 });
	gui_game->SetLocalRect({ (App->render->camera.w - 194 * 2) / (int)App->win->GetScale(),(App->render->camera.h - 70 * 2) / (int)App->win->GetScale(),222,299 });
	gui_game->SetElemInteractive(false);

	scene_gui->AddChild(gui_game);

	//Timer
	clock_ui = (GUI_Image*)App->gui->GenerateElemGUI(TypeGUI::IMAGE);
	clock_ui->SetIdTexture(id_tex_pause_atlas);
	clock_ui->SetRectTexture({ 226,179,194, 70 });
	clock_ui->SetLocalRect({ 0,(App->render->camera.h - 70 * 2) / (int)App->win->GetScale(),222,299 });
	clock_ui->SetElemInteractive(false);

	scene_gui->AddChild(clock_ui);

	//Timer String
	time_playing = (GUI_String*)App->gui->GenerateElemGUI(TypeGUI::TEXT);
	time_playing->SetFont(App->font->Load("fonts/open_sans/OpenSans-Semibold.ttf", 20));
	time_playing->SetColor(App->gui->WHITE);
	time_playing->SetText("");
	time_playing->SetLocalPos({ 0, 280 });
	time_playing->FitBox();
	time_playing->SetElemInteractive(false);

	clock_ui->AddChild(time_playing);
	///Hearts

	heart_1 = (GUI_Image*)App->gui->GenerateElemGUI(TypeGUI::IMAGE);
	heart_1->SetIdTexture(id_tex_pause_atlas);
	heart_1->SetRectTexture({ 426,100,16, 16 });
	heart_1->SetLocalRect({ 119,27,16,16 });
	heart_1->SetElemInteractive(false);

	gui_game->AddChild(heart_1);

	heart_2 = (GUI_Image*)App->gui->GenerateElemGUI(TypeGUI::IMAGE);
	heart_2->SetIdTexture(id_tex_pause_atlas);
	heart_2->SetRectTexture({ 426,100,16, 16 });
	heart_2->SetLocalRect({ 141,27,16,16 });
	heart_2->SetElemInteractive(false);

	gui_game->AddChild(heart_2);

	heart_3 = (GUI_Image*)App->gui->GenerateElemGUI(TypeGUI::IMAGE);
	heart_3->SetIdTexture(id_tex_pause_atlas);
	heart_3->SetRectTexture({ 426,100,16, 16 });
	heart_3->SetLocalRect({ 163,27,16,16 });
	heart_3->SetElemInteractive(false);

	gui_game->AddChild(heart_3);

	//Menu Pause 

	window_pause = (GUI_Window*)App->gui->GenerateElemGUI(TypeGUI::WINDOW);
	window_pause->SetLocalRect({ 145,41,222,299 });
	window_pause->SetElemInteractive(false);

	scene_gui->AddChild(window_pause);

	window_pause_image = (GUI_Image*)App->gui->GenerateElemGUI(TypeGUI::IMAGE);
	window_pause_image->SetIdTexture(id_tex_pause_atlas);
	window_pause_image->SetRectTexture({ 2,1,222,299 });
	window_pause_image->SetLocalRect({ 0,0,222,299 });
	window_pause_image->SetElemInteractive(false);

	window_pause->AddChild(window_pause_image);

	///Parent Buttons pause

	menu_pause_buttons = App->gui->GenerateElemGUI(TypeGUI::UNDEFINED);
	menu_pause_buttons->SetLocalRect({ 0,0,222,299 });
	menu_pause_buttons->SetElemInteractive(false);

	window_pause->AddChild(menu_pause_buttons);

	///Resume Button

	bt_resume = (GUI_Button*)App->gui->GenerateElemGUI(TypeGUI::BUTTON);
	bt_resume->SetButtonOff({ 224,1,140,39 }, id_tex_pause_atlas);
	bt_resume->SetButtonOn({ 366,1,140,39 }, id_tex_pause_atlas);
	bt_resume->SetButtonHover({ 508,1,140,39 }, id_tex_pause_atlas);
	bt_resume->SetLocalRect({ 42,42,140,39 });
	bt_resume->SetInputTarget(this);

	window_pause->AddChild(bt_resume);

	///Slider Volume Music

	////Text Vol Music

	str_volume_music_pause = (GUI_String*)App->gui->GenerateElemGUI(TypeGUI::TEXT);
	str_volume_music_pause->SetColor(App->gui->WHITE);
	str_volume_music_pause->SetText("Adjust Volume Music");
	str_volume_music_pause->SetLocalPos({ 50, 0 });
	str_volume_music_pause->SetElemInteractive(false);
	str_volume_music_pause->FitBox();

	window_pause->AddChild(str_volume_music_pause);

	///
	slider_vol_music_pause = (GUI_Scroll*)App->gui->GenerateElemGUI(TypeGUI::SLIDER_UI);

	slider_vol_music_pause->SetTypeScroll(HORIZONTAL);
	slider_vol_music_pause->SetScrollBackground({ 228,275,146,16 }, id_tex_pause_atlas);
	slider_vol_music_pause->SetScrollImage({ 431, 264,11,27 }, id_tex_pause_atlas);
	slider_vol_music_pause->SetLocalPos({ 38, 110 });
	slider_vol_music_pause->SetMaxValue(128.0f);
	slider_vol_music_pause->SetElemInteractive(true);
	slider_vol_music_pause->SetInputTarget(this);

	window_pause->AddChild(slider_vol_music_pause);

	///Slider Volume FX
	////Text Vol FX

	str_volume_fx_pause = (GUI_String*)App->gui->GenerateElemGUI(TypeGUI::TEXT);
	str_volume_fx_pause->SetColor(App->gui->WHITE);
	str_volume_fx_pause->SetText("Adjust Volume FX");
	str_volume_fx_pause->SetLocalPos({ 200, 180 });
	str_volume_fx_pause->SetElemInteractive(false);

	window_pause->AddChild(str_volume_fx_pause);

	slider_vol_fx_pause = (GUI_Scroll*)App->gui->GenerateElemGUI(TypeGUI::SLIDER_UI);

	slider_vol_fx_pause->SetTypeScroll(HORIZONTAL);
	slider_vol_fx_pause->SetScrollBackground({ 228,275,146,16 }, id_tex_pause_atlas);
	slider_vol_fx_pause->SetScrollImage({ 431, 264,11,27 }, id_tex_pause_atlas);
	slider_vol_fx_pause->SetLocalPos({ 38, 180 });
	slider_vol_fx_pause->SetMaxValue(128.0f);
	slider_vol_fx_pause->SetElemInteractive(true);
	slider_vol_fx_pause->SetInputTarget(this);

	window_pause->AddChild(slider_vol_fx_pause);

	///Go Back To menu

	go_back_to_main_menu = (GUI_Button*)App->gui->GenerateElemGUI(TypeGUI::BUTTON);
	go_back_to_main_menu->SetButtonOff({ 224,44,140,39 }, id_tex_pause_atlas);
	go_back_to_main_menu->SetButtonOn({ 366,44,140,39 }, id_tex_pause_atlas);
	go_back_to_main_menu->SetButtonHover({ 508,44,140,39 }, id_tex_pause_atlas);
	go_back_to_main_menu->SetLocalRect({ 42,243,140,39 });
	go_back_to_main_menu->SetInputTarget(this);

	window_pause->AddChild(go_back_to_main_menu);

	App->gui->SetSceneGUI(scene_gui);

	actual_timer.Start();

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	//Set time
	_itoa_s((int)actual_timer.ReadSec(), actual_time, 10, 10);
	time_playing->SetText(actual_time);

	LOG("%f",actual_timer.ReadSec());
	//Set Ammo
	_itoa_s(App->player->GetAmmo(), actual_ammo, 3, 10);
	//ammo_string->SetText(actual_ammo);

	//Set lifes
	SetHeartsGUI();

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN) {
		App->audio->SetVolume(App->audio->GetVolume() + 10);
	}
	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN) {
		App->audio->SetVolume(App->audio->GetVolume() - 10);
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		App->player->StartFromLvl1();
	}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		App->player->StartFromLvl2();
	}
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		App->player->StatFromCurrentLvl();

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
		App->WantToSave();
	}
	current_time_load += App->Getdt();

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		if (App->IsLoading() == false)
		{
			if (current_time_load > last_loaded) {
				current_time_load = 0;
				App->WantToLoad();
			}
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {
		App->gui->SetDebug();
	}
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
		App->colliders->debug = !App->colliders->debug;
		App->path->debug = !App->path->debug;
	}

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		App->player->GodMode();
	}

	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {
		App->SetCappedFrames();
	}

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		is_paused = !is_paused;
	}
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		if (is_paused) {
			window_pause->SetElemActive(false);
			window_pause->SetElemsInteractiveForChilds(false);
			is_paused = false;
		}
		else {
			window_pause->SetElemActive(true);
			window_pause->SetElemsInteractiveForChilds(true);
			is_paused = true;
		}
	}
	App->map->Draw();

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;


	return ret;
}

void j1Scene::ChangeScene(const float value) const
{
	App->path->ResetPath();
	App->fade->FadeToBlack(App->map, App->map, value);
	App->map->WantToLoadMap();
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool j1Scene::GetPause()
{
	return is_paused;
}

void j1Scene::Activate()
{
	App->fade->FadeToBlack(App->main_menu, App->map, 2.0f);
	scene_gui->SetElemActive(true);
	button_fx = App->audio->LoadFx("audio/fx/Click_Fx.wav");

	active = true;
}

void j1Scene::Desactivate()
{
	App->player->Desactivate();
	App->map->Desactivate();
	App->entity->Desactivate();
	scene_gui->SetElemActive(false);

	active = false;
}

GUIElement* j1Scene::GetSceneGUI()
{
	return scene_gui;
}

void j1Scene::HandleInput(GUIElement* input, TypeInput type_input)
{
	if (App->fade->isFading() == false) {
		//Main Menu Buttons
		if (window_pause->GetElemActive()) {

			if (input == bt_resume) {
				if (type_input == MOUSE_RIGHT_DOWN) {
					App->audio->PlayFx(button_fx);
					bt_resume->SetElemInteractive(false);
					window_pause->SetElemActive(false);
					is_paused = false;
				}
				if (type_input == MOUSE_LEFT_DOWN) {
					App->audio->PlayFx(button_fx);
					bt_resume->SetElemInteractive(false);
					window_pause->SetElemActive(false);
					is_paused = false;

				}
			}

			else if (input == go_back_to_main_menu) {
				if (type_input == MOUSE_RIGHT_DOWN) {
					App->audio->PlayFx(button_fx);
					go_back_to_main_menu->SetElemInteractive(false);
					window_pause->SetElemActive(false);
					is_paused = false;
					App->ActivateMainMenu();
				}
				if (type_input == MOUSE_LEFT_DOWN) {
					App->audio->PlayFx(button_fx);
					go_back_to_main_menu->SetElemInteractive(false);
					window_pause->SetElemActive(false);
					is_paused = false;
					App->ActivateMainMenu();

				}
			}
		}
	}
}

bool j1Scene::Load(const pugi::xml_node& node)
{
	float loaded_time = node.child("scene").attribute("current_time").as_float();

	actual_timer.Start();
	actual_timer.AddTime(loaded_time * 1000);

	return true;
}

bool j1Scene::Save(pugi::xml_node& node)
{
	pugi::xml_node scene = node.append_child("scene");
	scene.append_attribute("current_time").set_value(actual_timer.ReadSec());
	return true;
}

void j1Scene::SetHeartsGUI()
{
}
