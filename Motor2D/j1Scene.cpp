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
	if (App->player->IsChangingLVL()) {
		App->path->ResetPath();
		App->fade->FadeToBlack(App->map, App->map, value);
		App->map->WantToLoadMap();
		App->player->setChangingLVL();
	}
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