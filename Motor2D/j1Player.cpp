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

j1Player::j1Player()
{
	name = ("player");
}

j1Player::~j1Player()
{
}

bool j1Player::Awake()
{
	return true;
}

bool j1Player::Start()
{
	
	return true;
}

bool j1Player::PreUpdate()
{
	if (App->fade->isFading() == true) {
		if (App->fade->current_step != 2) {
			return true;
		}
	}
	PlayerInput();

	p_pos.x += p_current_vel.x;

	p_pos.y += p_current_vel.y;

	//if (p_floor == false) {
	p_current_vel.y -= gravity;
	//}
	//else
	{
		//p_current_vel.y = 0.0f;
	}
	p_collider->SetPos(p_pos.x, p_pos.y);
	return true;
}

bool j1Player::Update(float dt)
{



	CamFollowPlayer();

	return true;
}

bool j1Player::PostUpdate()
{

	return true;
}

bool j1Player::CleanUp()
{
	LOG("Cleaning Player");
	return true;
}

bool j1Player::Load(const pugi::xml_node& node)
{
	int loaded_lvl = node.child("Player").attribute("Current level").as_int();

	if (p_current_lvl != loaded_lvl) {
		if (loaded_lvl == 1) {
			App->map->ChangeMap("Level2.tmx");
		}
		else {
			App->map->ChangeMap("Level1.tmx");
		}
	}
	float uu = node.child("player").attribute("position_x").as_float();
	p_pos.x = node.child("player").attribute("position_x").as_float();
	p_pos.y = node.child("player").attribute("position_y").as_float();

	return true;
}

bool j1Player::Save(pugi::xml_node& node) 
{
	pugi::xml_node player = node.append_child("player");

	player.append_attribute("position_x").set_value(p_pos.x);;

	player.append_attribute("position_y").set_value(p_pos.y);

	player.append_attribute("current_level").set_value(p_current_lvl);;

	return true;
}


void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_WALL) {
		//TOP
		if (c2->rect.y - (c1->rect.y + c1->rect.h) < 0 && c2->rect.y - (c1->rect.y + c1->rect.h) > -16) {
			c1->rect.y = c2->rect.y - c1->rect.h;
			p_pos.y = c2->rect.y - c1->rect.h;
			if (p_current_vel.x > 0.0f) {
				p_current_state = IDLE;
			}             
			if (p_current_vel.y > 0.0f) {
				p_current_vel.y = 0.0f;
				p_current_state = IDLE;
			}
			p_floor = true;
			double_jump = false;
		}
		//LEFT
		else if (c2->rect.x - (c1->rect.x + c1->rect.w) < 0 && c2->rect.x - (c1->rect.x + c1->rect.w) > -16) {
			c1->rect.x = c2->rect.x - c1->rect.w;
			p_pos.x = c2->rect.x - c1->rect.w;
			p_current_vel.x = 0.0f;
		}
		//RIGHT
		else if (c1->rect.x - (c2->rect.x + c2->rect.w) < 0 && c1->rect.x - (c2->rect.x + c2->rect.w) > -16) {
			c1->rect.x = c2->rect.x + c2->rect.w;
			p_pos.x = c2->rect.x + c2->rect.w;
			p_current_vel.x = 0.0f;
		}
		//BOTTOM
		/*if (c1->rect.y - (c2->rect.y + c2->rect.h) < 0 && c1->rect.y - (c2->rect.y + c2->rect.h) > -16)
		{
			c1->rect.y = c2->rect.y + c2->rect.h;
			p_pos.y = c2->rect.y + c2->rect.h;
			p_current_vel.y = 0;
			return;
		}*/
	}

	if (c2->type == COLLIDER_FINISH) {
		ChangeLvl();
		App->scene->ChangeScene(1.0f);
	}

	if (c2->type == COLLIDER_DEAD) {
		p_current_vel.y = 0.0f;
		p_current_vel.x = 0.0f;
		App->scene->ChangeScene(2.0f);
		if (App->fade->current_step == 2) {
			PlayerDies();
		}
	}
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

void j1Player::PlayerInput()
{
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			p_current_vel.x = p_vel.x * -1;
		}

		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
			p_current_vel.x = 0.0f;
		}

		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			p_current_vel.x = p_vel.x;

		}

		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
			p_current_vel.x = 0.0f;
		}
		if (p_floor == true) {
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
				App->audio->PlayFx(jump_fx);
				p_floor = false;
				p_current_vel.y = -p_vel.y;
			}
		}
		else if (double_jump == false && p_floor == false) {
			if (p_current_vel.y > -1) {
				if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
					double_jump = true;
					p_current_vel.y = -p_vel.y;
				}
			}
		}

}



void j1Player::SpawnPlayer()
{
	p_spawn = { App->map->map_info.p_spaw_point.x, App->map->map_info.p_spaw_point.y, p_size_collider.x, p_size_collider.y };
	p_pos = fPoint(p_spawn.x, p_spawn.y - p_size_collider.y * 0.5f);
	p_current_vel = { 0.0f,0.0f };
	p_floor = false;
	if (p_collider == nullptr) {
		p_collider = App->colliders->AddCollider(p_spawn, COLLIDER_PLAYER);
	}
	p_collider->callback = this;
}

void j1Player::StartFromLvl1()
{
	App->scene->ChangeScene(1.0f);
	if (p_current_lvl != Lvl_1) {
		App->map->ChangeMap("Level1.tmx");
		p_current_lvl = Lvl_2;
	}
	StatFromCurrentLvl();
}

void j1Player::StartFromLvl2()
{
	App->scene->ChangeScene(1.0f);
	if (p_current_lvl != Lvl_2) {
		App->map->ChangeMap("Level2.tmx");
		p_current_lvl = Lvl_1;
	}

	StatFromCurrentLvl();
}

void j1Player::StatFromCurrentLvl()
{
	if (App->fade->isFading() == false) {
		App->scene->ChangeScene(1.0f);
	}

	SpawnPlayer();
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
	p_dead = true;
	App->audio->PlayFx(g_is_over_fx);
	StatFromCurrentLvl();

}

void j1Player::CamFollowPlayer()
{
	uint window_w, window_h;
	App->win->GetWindowSize(window_w, window_h);

	int max_width = App->map->map_info.width * App->map->map_info.tilewidth;
	int max_height = App->map->map_info.height * App->map->map_info.tileheight;
	
	int min_camera_x = 0;
	int max_camera_x = -max_width * App->win->GetScale() + window_w;
	int target_cam_x = (-p_pos.x * App->win->GetScale()) + (window_w * 0.5f);


	App->render->camera.x = target_cam_x > min_camera_x ? min_camera_x : target_cam_x < max_camera_x ? max_camera_x : target_cam_x;
	
	App->render->camera.y = window_h - max_height * App->win->GetScale();

	//App->render->camera
}

