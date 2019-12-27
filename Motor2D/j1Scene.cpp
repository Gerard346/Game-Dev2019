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
	App->fade->FadeToColor(NULL,NULL, 1.5f);
	//App->map->ChangeMap(App->map->GetPathFromLevel(lvl_1_map));
	App->audio->PlayMusic(music_path.GetString());
	p2SString title("Last Soldier");

	App->win->SetTitle(title.GetString());

	scene_gui = App->gui->GenerateElemGUI(TypeGUI::UNDEFINED);
	scene_gui->SetLocalRect({ 0,0,App->render->camera.w / (int)App->win->GetScale(),App->render->camera.h / (int)App->win->GetScale()});
	/*// TODO 3: Create the banner (rect {485, 829, 328, 103}) as a UI element
	imgsa = (GUI_Image*)App->gui->GenerateElemGUI(TypeGUI::IMAGE);
	imgsa->SetRectTexture({ 0, 0, 1500	, 1503 });
	imgsa->SetBoxElem({ 0,0,0,0 });
	scene_gui->AddChild(imgsa);

	SDL_Color color = { 255, 255, 255, 255 };
	_TTF_Font* font = App->font->Load("fonts/open_sans/OpenSans-Regular.ttf", 60);

	SDL_Texture* font_img = App->font->Print("Hello World", App->gui->YELLOW, font);
	*/
	window = (GUI_Window*)App->gui->GenerateElemGUI(TypeGUI::WINDOW);
	window->SetLocalRect({ 100,100,300,300 });
	scene_gui->AddChild(window);



	button = (GUI_Button*)App->gui->GenerateElemGUI(TypeGUI::BUTTON);
	button->SetButtonOff({ 0,113,229,69 }, 0);
	button->SetButtonOn({ 642,169,229,69 }, 0);
	button->SetButtonHover({ 411,169,229,69 }, 0);
	button->SetLocalRect({-50,0,229,69});
	button->SetInputTarget(this);
	window->AddChild(button);

	button2 = (GUI_Button*)App->gui->GenerateElemGUI(TypeGUI::BUTTON);
	button2->SetButtonOff({ 0,113,229,69 }, 0);
	button2->SetButtonOn({ 642,169,229,69 }, 0);
	button2->SetButtonHover({ 411,169,229,69 }, 0);
	button2->SetLocalRect({ 0,30,229,69 });
	button2->SetInputTarget(this);
	window->AddChild(button2, button->GetLayer() + 1);

	App->gui->SetSceneGUI(scene_gui);

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN) {
		App->audio->SetVolume(20);
	}
	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN) {
		App->audio->SetVolume(-20);
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

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		if (App->IsLoading() == false)
		{
			App->WantToLoad();
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

	App->map->Draw();

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

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