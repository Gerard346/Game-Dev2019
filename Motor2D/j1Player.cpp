#include "p2Log.h"
#include "j1Window.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Colliders.h"
#include "j1FadeToBlack.h"
#include "j1Animation.h"
#include "EntityManager.h"
#include "PlayerEntity.h"
j1Player::j1Player()
{
	name = ("player");
}

j1Player::~j1Player()
{
}

bool j1Player::Awake(const pugi::xml_node& node)
{
	BROFILER_CATEGORY("Awake Player", Profiler::Color::Yellow);

	if (node != nullptr) {
		g_is_over_fx_path = (char*)node.child("fx_dead").child_value();
		next_lvl_fx_path = (char*)node.child("fx_next_lvl").child_value();
	}

	else {
		LOG("Error in Awake %s", name);
	}

	LOG("Loading Player");
	return true;
}

bool j1Player::Start()
{
	if (g_is_over_fx == -1) {
		g_is_over_fx = App->audio->LoadFx(g_is_over_fx_path);
	}
	
	if (next_lvl_fx == -1) {
		next_lvl_fx = App->audio->LoadFx(next_lvl_fx_path);
	}
	
	return true;
}

bool j1Player::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdate Player", Profiler::Color::Yellow);

	return true;
}

bool j1Player::Update(float dt)
{
	BROFILER_CATEGORY("Update Player", Profiler::Color::Yellow);
	CamFollowPlayer();

	if (end_game)
	{
		end_game_timer += App->Getdt();
		if (end_game_timer >= end_game_delay)
		{
			end_game = false;
			end_game_timer = 0.0f;
			App->ActivateMainMenu();
		}
	}
	return true;
}

bool j1Player::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate Player", Profiler::Color::Yellow);
	
	return true;
}

bool j1Player::CleanUp()
{
	LOG("Cleaning Player");
	return true;
}

bool j1Player::Load(const pugi::xml_node& node)
{
	Levels loaded_lvl = (Levels)node.child("player").attribute("current_level").as_int();
	
	if (loaded_lvl == Lvl_1)
	{
		StartFromLvl1();
		//App->map->LoadMaplvl1();
	}
	else 
	{
		StartFromLvl2();
		//App->map->LoadMaplvl2();
	}

	return true;
}

bool j1Player::Save(pugi::xml_node& node) 
{
	pugi::xml_node player = node.append_child("player");
	player.append_attribute("current_level").set_value(p_current_lvl);

	return true;
}

void j1Player::ChangeLvl()
{
	App->audio->PlayFx(next_lvl_fx);
	
	if (p_current_lvl == Lvl_1) {
		StartFromLvl2();
	}

	else{
		SetEndGame(true);
		App->scene->SetActiveWinImg();
	}
}

void j1Player::StartFromLvl1()
{
	p_current_lvl = Lvl_1;
	App->scene->ChangeScene(1.0f);
}

void j1Player::StartFromLvl2()
{
	p_current_lvl = Lvl_2;
	App->scene->ChangeScene(1.0f);
}

void j1Player::StatFromCurrentLvl()
{
	App->scene->ChangeScene(1.0f);
}

void j1Player::GodMode()
{
	p_god = !p_god;
}

bool j1Player::IsGod() const
{
	return p_god;
}

bool j1Player::IsDead() const
{
	return p_dead;
}

void j1Player::setAlive()
{
	p_dead = false;
}

void j1Player::PlayerDies()
{
	if (p_dead == false)
	{
		SetLife(-1);
		LOG("DEAD");
		p_dead = true;
		App->audio->PlayFx(g_is_over_fx);
		if (lifes < 0) {
			LOG("LOSE");
			end_game = true;
			App->scene->SetActiveLoseImg();
			return;
		}
		App->WantToLoadCheckpoints();
	}
}	

void j1Player::CamFollowPlayer()
{
	PlayerEntity* entity_player = App->entity->GetPlayer();
	if (entity_player == nullptr)return;

	uint window_w, window_h;
	App->win->GetWindowSize(window_w, window_h);

	int max_width = App->map->map_info.width * App->map->map_info.tilewidth;
	int max_height = App->map->map_info.height * App->map->map_info.tileheight;

	int min_camera_x = 0;
	int max_camera_x = -max_width * App->win->GetScale() + window_w;


	int target_cam_x = 0;
	target_cam_x = (-entity_player->entity_pos.x * App->win->GetScale()) + (window_w * 0.5f);

	App->render->camera.x = target_cam_x > min_camera_x ? min_camera_x : target_cam_x < max_camera_x ? max_camera_x : target_cam_x;

	//App->render->camera.y = window_h - max_height * App->win->GetScale();
}

void j1Player::Activate()
{
	active = true;
}

void j1Player::Desactivate()
{
	active = false;
}

int j1Player::GetLife() const
{
	return lifes;
}

void j1Player::SetLife(int life)
{
	lifes += life;
}

int j1Player::GetAmmo() const
{
	return ammo;
}

void j1Player::SetAmmo(int new_ammo)
{
	ammo += new_ammo;
}

void j1Player::SetEndGame(bool ret)
{
	end_game = true;
}

