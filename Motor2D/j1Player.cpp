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
		jump_fx_path = (char*)node.child("fx_jump").child_value();
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
	if (jump_fx == -1) {
		jump_fx = App->audio->LoadFx(jump_fx_path);
	}

	StatFromCurrentLvl();

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
			App->map->ChangeMap("Level2.tmx");
		}
		else {
			App->map->ChangeMap("Level1.tmx");
		}
	}
	float uu = node.child("player").attribute("position_x").as_float();
	//p_pos.x = node.child("player").attribute("position_x").as_float();
	//p_pos.y = node.child("player").attribute("position_y").as_float();

	return true;
}

bool j1Player::Save(pugi::xml_node& node) 
{
	pugi::xml_node player = node.append_child("player");

//	player.append_attribute("position_x").set_value(p_pos.x);;

	//player.append_attribute("position_y").set_value(p_pos.y);

	//player.append_attribute("current_level").set_value(p_current_lvl);;

	return true;
}






void j1Player::ChangeLvl()
{
	if (p_current_lvl == Lvl_1) {
		StartFromLvl2();
	}

	else{
		StartFromLvl1();
	}
}

void j1Player::PlayerInput(float dt)
{/*
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			p_current_vel.x = p_vel.x * -1*dt;
			if (p_floor)
			{
				SetPlayerState(WALK_RIGHT);
			}
			else
			{
				SetPlayerState(JUMP_RIGHT);

			}
		}

		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
			p_current_vel.x = 0.0f;
			if (p_floor)
			{
				SetPlayerState(IDLE_RIGHT);
			}
			else
			{
				SetPlayerState(JUMP_RIGHT);
			}
		}

		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			p_current_vel.x = p_vel.x*dt;
			if (p_floor)
			{
				SetPlayerState(WALK_LEFT);
			}
			else
			{
				SetPlayerState(JUMP_LEFT);
			}
		}

		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
			p_current_vel.x = 0.0f;
			if (p_floor)
			{
				SetPlayerState(IDLE_LEFT);
			}
			else
			{
				SetPlayerState(JUMP_LEFT);
			}

		}
		if (p_floor == true) {
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
				App->audio->PlayFx(jump_fx);
				p_floor = false;
				p_current_vel.y = -p_vel.y*dt;

				if (p_current_state == IDLE_RIGHT || p_current_state == WALK_RIGHT)
				{
					SetPlayerState(JUMP_RIGHT);
				}
				else if(p_current_state == IDLE_LEFT || p_current_state == WALK_LEFT)
				{
					SetPlayerState(JUMP_LEFT);
				}
			}
		}
		else if (double_jump == false && p_floor == false && IsGod() == false) {
			
			if (p_current_vel.y > -1) {
				if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
					App->audio->PlayFx(jump_fx);
					double_jump = true;
					p_current_vel.y = -p_vel.y*dt;

					if (p_current_state == IDLE_RIGHT || p_current_state == WALK_RIGHT)
					{
						SetPlayerState(JUMP_RIGHT);
					}
					else if (p_current_state == IDLE_LEFT || p_current_state == WALK_LEFT)
					{
						SetPlayerState(JUMP_LEFT);
					}
				}
			}
		}
		if (IsGod()) {
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
				p_current_vel.y = -p_vel.x*dt;
			}
			else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP) {
				p_current_vel.y = 0.0f;
			}
			else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
				p_current_vel.y = p_vel.x*dt;
			}
			else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP) {
				p_current_vel.y = 0.0f;
			}
		}*/
}



void j1Player::SpawnPlayer()
{
	PlayerEntity* player = App->entity->GetPlayer();

	//player

	App->entity->CreateEntity(entityType::PLAYER_TYPE);
	/*
	p_dead = false;
	p_spawn = { App->map->map_info.p_spaw_point.x, App->map->map_info.p_spaw_point.y, p_size_collider.x, p_size_collider.y };
	p_pos = fPoint(p_spawn.x, p_spawn.y - p_size_collider.y * 0.5f);
	p_current_vel = { 0.0f,0.0f };
	p_floor = false;
	
	if (p_collider == nullptr) {
		p_collider = App->colliders->AddCollider(p_spawn, COLLIDER_PLAYER);
	}

	p_collider->callback = this;

	current_animation = App->animation->GetAnimation(PLAYER, (ANIMATION_TYPE)p_current_state);
*/}

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
	//SpawnPlayer();
}

void j1Player::GodMode()
{
	p_god = !p_god;
}

bool j1Player::IsGod()
{
	return p_god;
}

void j1Player::PlayerDies()
{
	App->audio->PlayFx(g_is_over_fx);
	App->fade->FadeToBlack(App->player, App->player);
}

void j1Player::CamFollowPlayer()
{
	uint window_w, window_h;
	App->win->GetWindowSize(window_w, window_h);

	int max_width = App->map->map_info.width * App->map->map_info.tilewidth;
	int max_height = App->map->map_info.height * App->map->map_info.tileheight;
	
	int min_camera_x = 0;
	int max_camera_x = -max_width * App->win->GetScale() + window_w;

	PlayerEntity* entity_player = nullptr;
	int target_cam_x = 0;
	entity_player = App->entity->GetPlayer();
	if (entity_player != nullptr) {
		target_cam_x = (-entity_player->entity_pos.x * App->win->GetScale()) + (window_w * 0.5f);
	}

	App->render->camera.x = target_cam_x > min_camera_x ? min_camera_x : target_cam_x < max_camera_x ? max_camera_x : target_cam_x;
	
	App->render->camera.y = window_h - max_height * App->win->GetScale();

	//App->render->camera
}

