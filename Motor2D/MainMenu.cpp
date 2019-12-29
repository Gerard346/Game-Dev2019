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
#include "GUI_Scroll.h"
#include "j1Map.h"

MainMenu::MainMenu() : j1Module()
{
	name.create("main_menu");
}

MainMenu::~MainMenu()
{}






bool MainMenu::Awake(const pugi::xml_node & node)
{
	return true;
}

bool MainMenu::Start()
{

	//Loading Textures
	int id_tex_bck = App->gui->AddTexture(App->tex->Load("gui/main_menu.png"));
	int id_tex_atlas = App->gui->AddTexture(App->tex->Load("gui/menu_icons.png"));

	//Creating Main Menu
	menu_scene = App->gui->GenerateElemGUI(TypeGUI::UNDEFINED);
	menu_scene->SetLocalRect({ 0,0,App->render->camera.w / (int)App->win->GetScale(),App->render->camera.h / (int)App->win->GetScale() });
	menu_scene->SetElemInteractive(false);

	App->audio->PlayMusic(MusicType::MAIN_MENU_MUSIC);
	
	//Background Menu
	bck_menu = (GUI_Image*)App->gui->GenerateElemGUI(TypeGUI::IMAGE);
	bck_menu->SetIdTexture(id_tex_bck);
	bck_menu->SetRectTexture({ 0,0, 512,384 });
	bck_menu->SetLocalRect({ 0,0, App->render->camera.w / (int)App->win->GetScale(), App->render->camera.h / (int)App->win->GetScale() });
	bck_menu->SetElemInteractive(true);
	
	menu_scene->AddChild(bck_menu);

	//Window Menu

	window_menu = (GUI_Window*)App->gui->GenerateElemGUI(TypeGUI::WINDOW);
	window_menu->SetLocalRect({ 145,41,222,299 });
	window_menu->SetElemInteractive(false);

	menu_scene->AddChild(window_menu);

	window_img = (GUI_Image*)App->gui->GenerateElemGUI(TypeGUI::IMAGE);
	window_img->SetIdTexture(id_tex_atlas);
	window_img->SetRectTexture({ 2,1,222,299 });
	window_img->SetLocalRect({ 0,0,222,299 });
	window_img->SetElemInteractive(false);

	window_menu->AddChild(window_img);

	//Parent Buttons

	menu_buttons = App->gui->GenerateElemGUI(TypeGUI::UNDEFINED);
	menu_buttons->SetLocalRect({ 0,0,222,299 });
	menu_buttons->SetElemInteractive(false);

	window_menu->AddChild(menu_buttons);

	//Play Button

	bt_play = (GUI_Button*)App->gui->GenerateElemGUI(TypeGUI::BUTTON);
	bt_play->SetButtonOff({ 224,1,140,39 }, id_tex_atlas);
	bt_play->SetButtonOn({ 366,1,140,39 }, id_tex_atlas);
	bt_play->SetButtonHover({ 508,1,140,39 }, id_tex_atlas);
	bt_play->SetLocalRect({ 42,42,140,39 });
	bt_play->SetInputTarget(this);
	menu_buttons->AddChild(bt_play);

	//Continue

	bt_continue = (GUI_Button*)App->gui->GenerateElemGUI(TypeGUI::BUTTON);
	bt_continue->SetButtonOff({ 224,44,140,39 }, id_tex_atlas);
	bt_continue->SetButtonOn({ 366,44,140,39 }, id_tex_atlas);
	bt_continue->SetButtonHover({ 508,44,140,39 }, id_tex_atlas);
	bt_continue->SetLocalRect({ 42,90,140,39 });
	bt_continue->SetInputTarget(this);
	menu_buttons->AddChild(bt_continue);

	//Settings

	bt_settings = (GUI_Button*)App->gui->GenerateElemGUI(TypeGUI::BUTTON);
	bt_settings->SetButtonOff({ 224,89,140,39 }, id_tex_atlas);
	bt_settings->SetButtonOn({ 366,89,140,39 }, id_tex_atlas);
	bt_settings->SetButtonHover({ 508,89,140,39 }, id_tex_atlas);
	bt_settings->SetLocalRect({ 42,141,140,39 });
	bt_settings->SetInputTarget(this);
	menu_buttons->AddChild(bt_settings);

	//Credits

	bt_credits = (GUI_Button*)App->gui->GenerateElemGUI(TypeGUI::BUTTON);
	bt_credits->SetButtonOff({ 224,132,140,39 }, id_tex_atlas);
	bt_credits->SetButtonOn({ 366,132,140,39 }, id_tex_atlas);
	bt_credits->SetButtonHover({ 508,132,140,39 }, id_tex_atlas);
	bt_credits->SetLocalRect({ 42,193,140,39 });
	bt_credits->SetInputTarget(this);
	menu_buttons->AddChild(bt_credits);

	//Page

	bt_web = (GUI_Button*)App->gui->GenerateElemGUI(TypeGUI::BUTTON);
	bt_web->SetButtonOn({ 576,263,36,39 }, id_tex_atlas);
	bt_web->SetButtonHover({ 497,263,36,39 }, id_tex_atlas);
	bt_web->SetButtonOff({ 537,263,36,39 }, id_tex_atlas);
	bt_web->SetLocalRect({ 10,0,41,41 });
	bt_web->SetInputTarget(this);
	menu_buttons->AddChild(bt_web);

	website_str = (GUI_String*)App->gui->GenerateElemGUI(TypeGUI::TEXT);
	website_str->SetColor(App->gui->WHITE);
	website_str->SetText("<- Click To Go The Website");
	website_str->SetLocalPos({ 195, 50 });
	website_str->SetElemInteractive(false);

	menu_buttons->AddChild(website_str);
	//Exit Button

	bt_exit = (GUI_Button*)App->gui->GenerateElemGUI(TypeGUI::BUTTON);
	bt_exit->SetButtonOff({ 224,180,140,39 }, id_tex_atlas);
	bt_exit->SetButtonOn({ 366,180,140,39 }, id_tex_atlas);
	bt_exit->SetButtonHover({ 508,180,140,39 }, id_tex_atlas);
	bt_exit->SetLocalRect({ 42,243,140,39 });
	bt_exit->SetInputTarget(this);
	menu_buttons->AddChild(bt_exit);

	//Sub-Menus

	//Settings-Menus

	///parent

	menu_settings = App->gui->GenerateElemGUI(TypeGUI::UNDEFINED);
	menu_settings->SetLocalRect({ 0,0,222,299 });
	menu_settings->SetElemActive(false);
	menu_settings->SetElemInteractive(false);
	window_menu->AddChild(menu_settings);

	///Slider Volume Music

	////Text Vol Music

	str_volume_music = (GUI_String*)App->gui->GenerateElemGUI(TypeGUI::TEXT);
	str_volume_music->SetColor(App->gui->WHITE);
	str_volume_music->SetText("Adjust Volume Music");
	str_volume_music->SetLocalPos({ 200, 110 });
	str_volume_music->SetElemInteractive(false);

	menu_settings->AddChild(str_volume_music);

	///
	slider_vol_music = (GUI_Scroll*)App->gui->GenerateElemGUI(TypeGUI::SLIDER_UI);

	slider_vol_music->SetTypeScroll(HORIZONTAL);
	slider_vol_music->SetScrollBackground({ 228,275,146,16 }, id_tex_atlas);
	slider_vol_music->SetScrollImage({ 431, 264,11,27 }, id_tex_atlas);
	slider_vol_music->SetLocalPos({ 38, 100 });
	slider_vol_music->SetMaxValue(128.0f);
	slider_vol_music->SetElemInteractive(true);
	slider_vol_music->SetInputTarget(this);

	menu_settings->AddChild(slider_vol_music);

	///Slider Volume FX
	////Text Vol FX

	str_volume_fx = (GUI_String*)App->gui->GenerateElemGUI(TypeGUI::TEXT);
	str_volume_fx->SetColor(App->gui->WHITE);
	str_volume_fx->SetText("Adjust Volume FX");
	str_volume_fx->SetLocalPos({ 200, 180});
	str_volume_fx->SetElemInteractive(false);

	menu_settings->AddChild(str_volume_fx);

	slider_vol_fx = (GUI_Scroll*)App->gui->GenerateElemGUI(TypeGUI::SLIDER_UI);

	slider_vol_fx->SetTypeScroll(HORIZONTAL);
	slider_vol_fx->SetScrollBackground({ 228,275,146,16 }, id_tex_atlas);
	slider_vol_fx->SetScrollImage({ 431, 264,11,27 }, id_tex_atlas);
	slider_vol_fx->SetLocalPos({ 38, 170 });
	slider_vol_fx->SetMaxValue(128.0f);
	slider_vol_fx->SetElemInteractive(true);
	slider_vol_fx->SetInputTarget(this);

	menu_settings->AddChild(slider_vol_fx);

	///Go Back To menu
	
	back_to_menu = (GUI_Button*)App->gui->GenerateElemGUI(TypeGUI::BUTTON);
	back_to_menu->SetButtonOff({ 224,223,140,39 }, id_tex_atlas);
	back_to_menu->SetButtonOn({ 366,223,140,39 }, id_tex_atlas);
	back_to_menu->SetButtonHover({ 508,223,140,39 }, id_tex_atlas);
	back_to_menu->SetLocalRect({ 42,200,140,39 });
	back_to_menu->SetInputTarget(this);
	menu_settings->AddChild(back_to_menu);


	//Credits-Menus

	///parent

	menu_credits = App->gui->GenerateElemGUI(TypeGUI::UNDEFINED);
	menu_credits->SetLocalRect({ 0,0,222,299 });
	menu_credits->SetElemActive(false);
	menu_credits->SetElemInteractive(false);
	window_menu->AddChild(menu_credits);


	///Text License

	str_volume_music = (GUI_String*)App->gui->GenerateElemGUI(TypeGUI::TEXT);
	str_volume_music->SetColor(App->gui->WHITE);
	str_volume_music->SetLocalPos({ 200, 110 });
	str_volume_music->SetElemInteractive(false);

	menu_credits->AddChild(str_volume_music);

	///Go Back To menu

	back_to_menu_credits = (GUI_Button*)App->gui->GenerateElemGUI(TypeGUI::BUTTON);
	back_to_menu_credits->SetButtonOff({ 224,223,140,39 }, id_tex_atlas);
	back_to_menu_credits->SetButtonOn({ 366,223,140,39 }, id_tex_atlas);
	back_to_menu_credits->SetButtonHover({ 508,223,140,39 }, id_tex_atlas);
	back_to_menu_credits->SetLocalRect({ 42,0,140,39 });
	back_to_menu_credits->SetInputTarget(this);
	menu_credits->AddChild(back_to_menu_credits);

	//SetScene
	//App->gui->SetSceneGUI(menu_scene);


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
	button_fx = App->audio->LoadFx("audio/fx/Click_Fx.wav");
	active = true;
	App->gui->SetSceneGUI(menu_scene);
	menu_buttons->SetElemsInteractiveForChilds(true);
	slider_vol_music->SetCurrentValue(App->audio->GetVolume());
}

void MainMenu::Desactivate()
{
	active = false;
}

void MainMenu::HandleInput(GUIElement* input, TypeInput type_input)
{
	if (App->fade->isFading() == false) {
		//Main Menu Buttons
		if (menu_buttons->GetElemActive()) {

			if (input == bt_play) {
				if (type_input == MOUSE_RIGHT_DOWN) {
					App->ActivateGame();
					App->audio->PlayFx(button_fx);
					bt_play->SetElemInteractive(false);
				}
				if (type_input == MOUSE_LEFT_DOWN) {
					App->ActivateGame();
					App->audio->PlayFx(button_fx);
					bt_play->SetElemInteractive(false);

				}
			}

			else if (input == bt_continue) {
				if (type_input == MOUSE_RIGHT_DOWN) {
					App->audio->PlayFx(button_fx);

					App->ActivateGame();
					App->WantToLoad();
				}
				if (type_input == MOUSE_LEFT_DOWN) {
					App->audio->PlayFx(button_fx);

					App->ActivateGame();
					App->WantToLoad();
				}
			}

			else if (input == bt_settings) {
				if (type_input == MOUSE_RIGHT_DOWN) {
					App->audio->PlayFx(button_fx);

					menu_buttons->SetElemActive(false);
					menu_settings->SetElemActive(true);

					slider_vol_music->SetCurrentValue(App->audio->GetVolume());
					slider_vol_fx->SetCurrentValue(App->audio->GetVolumeFX());
				}
				if (type_input == MOUSE_LEFT_DOWN) {
					App->audio->PlayFx(button_fx);

					menu_buttons->SetElemActive(false);
					menu_settings->SetElemActive(true);

					slider_vol_music->SetCurrentValue(App->audio->GetVolume());
					slider_vol_fx->SetCurrentValue(App->audio->GetVolumeFX());
				}
			}

			else if (input == bt_credits) {
				if (type_input == MOUSE_RIGHT_DOWN) {
					App->audio->PlayFx(button_fx);

					menu_buttons->SetElemActive(false);
					menu_credits->SetElemActive(true);
				}
				if (type_input == MOUSE_LEFT_DOWN) {
					App->audio->PlayFx(button_fx);

					menu_buttons->SetElemActive(false);
					menu_credits->SetElemActive(true);
				}
			}

			else if (input == bt_exit) {
				if (type_input == MOUSE_RIGHT_DOWN) {
					App->audio->PlayFx(button_fx);

					bt_exit->SetElemInteractive(false);
					App->audio->PlayFx(button_fx);
					App->WantToQuit();
				}
				if (type_input == MOUSE_LEFT_DOWN) {
					App->audio->PlayFx(button_fx);

					bt_exit->SetElemInteractive(false);
					App->audio->PlayFx(button_fx);
					App->WantToQuit();
				}
			}

			else if (input == bt_web) {
				if (type_input == MOUSE_RIGHT_DOWN) {
					App->audio->PlayFx(button_fx);

					ShellExecuteA(NULL, "open", "http://google.es", NULL, NULL, SW_SHOWNORMAL);
				}
				if (type_input == MOUSE_LEFT_DOWN) {
					App->audio->PlayFx(button_fx);

					ShellExecuteA(NULL, "open", "http://google.es", NULL, NULL, SW_SHOWNORMAL);
				}
			}
		}
		//Settings Menu Buttons
		else if (menu_settings->GetElemActive()) {
			if (input == back_to_menu) {
				if (type_input == MOUSE_RIGHT_DOWN) {
					App->audio->PlayFx(button_fx);

					menu_buttons->SetElemActive(true);
					menu_settings->SetElemActive(false);
				}

				if (type_input == MOUSE_LEFT_DOWN) {
					App->audio->PlayFx(button_fx);

					menu_buttons->SetElemActive(true);
					menu_settings->SetElemActive(false);
				}
			}
			
			else if (input == slider_vol_music)
			{
				App->audio->SetVolume(slider_vol_music->GetCurrentValue());
			}

			else if (input == slider_vol_fx)
			{
				App->audio->SetVolumeFX(slider_vol_fx->GetCurrentValue());
			}
		}
	
		else if (menu_credits->GetElemActive()) {
			if (input == back_to_menu_credits) {
				if (type_input == MOUSE_RIGHT_DOWN) {
					App->audio->PlayFx(button_fx);

					menu_buttons->SetElemActive(true);
					menu_credits->SetElemActive(false);
				}

				if (type_input == MOUSE_LEFT_DOWN) {
					App->audio->PlayFx(button_fx);

					menu_buttons->SetElemActive(true);
					menu_credits->SetElemActive(false);
				}
			}
		}
	}
}
bool MainMenu::Load(const pugi::xml_node&)
{
	return true;
}

bool MainMenu::Save(pugi::xml_node&)
{
	return true;
}
