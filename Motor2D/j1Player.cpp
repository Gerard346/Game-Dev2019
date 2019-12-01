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
	if (p_dead == true) {
		p_dead = false;
		App->WantToLoadCheckpoints();
	}

	
	//next_lvl = false;
	
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
	int loaded_lvl = node.child("player").attribute("current_level").as_int();

	if (p_current_lvl != loaded_lvl) {
		if (loaded_lvl == 1) {
//			App->map->ChangeMap(App->map->GetPathFromLevel(lvl_1_map));
		}
		else {
			//App->map->ChangeMap(App->map->GetPathFromLevel(lvl_2_map));
		}
	}
	return true;
}

bool j1Player::Save(pugi::xml_node& node) 
{
	pugi::xml_node player = node.append_child("player");
	player.append_attribute("current_level").set_value(p_current_lvl+1);

	return true;
}

void j1Player::ChangeLvl()
{
	App->audio->PlayFx(next_lvl_fx);
	//next_lvl = true;
	if (p_current_lvl == Lvl_1) {
		StartFromLvl2();
	}

	else{
		StartFromLvl1();
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

bool j1Player::IsChangingLVL() const
{
	return next_lvl;
}

void j1Player::PlayerDies()
{
	if (p_dead == false)
	{
		LOG("DEAD");
		p_dead = true;
		App->audio->PlayFx(g_is_over_fx);
		App->fade->FadeToBlack(App->player, App->player);
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

	App->render->camera.y = window_h - max_height * App->win->GetScale();
}

