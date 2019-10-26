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

bool j1Player::Save(pugi::xml_node& node) const
{
	pugi::xml_node player = node.append_child("Player");

	player.attribute("Position x") = p_pos.x;
	player.attribute("Position y") = p_pos.y;
	player.attribute("Current level") = p_current_lvl;

	return true;
}

bool j1Player::Load(pugi::xml_node& node)
{
	int loaded_lvl = node.child("Player").attribute("Current level").as_int();

	if (p_current_lvl != loaded_lvl) {
		if (loaded_lvl == 1) {
			App->map->ChangeMap("Level2.tmx");
		}
		else {
			App->map->ChangeMap("Level1.tmx");
		}
		p_pos.x = node.child("Player").attribute("Position x").as_float();
		p_pos.y = node.child("Player").attribute("Position y").as_float();
	}
	else {
		p_pos.x = node.child("Player").attribute("Position x").as_float();
		p_pos.y = node.child("Player").attribute("Position y").as_float();
	}

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
	}

	if (c2->type == COLLIDER_DEAD) {
		PlayerDies();
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
	//if (p_floor == true && IsGod() == false) {
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

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			App->audio->PlayFx(jump_fx);
			p_floor = false;
			p_current_vel.y = -p_vel.y;
		}
	//}
}
/*
	if (p_floor == false && IsGod() == false) {
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			if (double_jump == false) {
				double_jump = true;
			}

		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			p_current_vel.x = p_vel.x * -1;

		}

		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
			p_current_vel.x = 0.0f;
		}

		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {

		}

		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {

		}
	}
	if (IsGod()) {
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {

		}

		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {

		}

		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {

		}

		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {

		}

		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {

		}

		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP) {

		}

		else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {

		}

		else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP) {

		}
	}
}
*/


void j1Player::SpawnPlayer()
{
	p_spawn = { App->map->map_info.p_spaw_point.x, App->map->map_info.p_spaw_point.y, p_size_collider.x, p_size_collider.y };
	p_pos = fPoint(p_spawn.x, p_spawn.y - p_size_collider.y *0.5f);
	p_collider = App->colliders->AddCollider(p_spawn, COLLIDER_PLAYER);
	p_collider->callback = this;

}

void j1Player::StartFromLvl1()
{
	if (p_current_lvl != Lvl_1) {
		App->map->ChangeMap("Level1.tmx");
	}
	StatFromCurrentLvl();
}

void j1Player::StartFromLvl2()
{
	if (p_current_lvl != Lvl_2) {
		App->map->ChangeMap("Level2.tmx");
	}
	StatFromCurrentLvl();
}

void j1Player::StatFromCurrentLvl()
{
	App->render->camera.x = 0;
	App->render->camera.y = 0;
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

}

