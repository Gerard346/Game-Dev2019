#include "j1App.h"
#include"p2Log.h"
#include"p2Defs.h"
#include "j1Audio.h"
#include "PlayerEntity.h"
#include "Bullet.h"
#include "j1Player.h"
#include "j1Render.h"
#include "j1Animation.h"
#include "j1Map.h"
#include "j1Module.h"
#include "EntityManager.h"

PlayerEntity::PlayerEntity()
{
}

PlayerEntity::PlayerEntity(const PlayerEntity* copy): BaseEntity(copy)
{

}

PlayerEntity::~PlayerEntity()
{
}

bool PlayerEntity::Update(float dt)
{
	BROFILER_CATEGORY("UpdatePlayerEntity", Profiler::Color::DarkOliveGreen);
	if (entity_collider == nullptr) {
		return true;
	}

	HandleInput(App->Getdt());

	if (App->player->IsGod() == false) 
	{
		entity_current_vel.y -= actual_gravity * App->Getdt();
	}

	entity_pos.x += entity_current_vel.x * App->Getdt();
	entity_pos.y += entity_current_vel.y * App->Getdt();

	entity_collider->SetPos(entity_pos.x, entity_pos.y);

	if (actual_gravity != gravity)
	{
		actual_gravity = gravity;
	}

	return true;
}

bool PlayerEntity::CleanUp()
{
	return true;
}

void PlayerEntity::HandleInput(float dt)
{
	
	
	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) {
		Shoot();
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		entity_current_vel.x = entity_vel.x * -1 ;
		if (entity_floor)
		{
			App->entity->SetEntityState(ENTITY_WALK_RIGHT, entity_collider);
		}
		else
		{
			App->entity->SetEntityState(ENTITY_JUMP_RIGHT, entity_collider);

		}
	}

	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
		entity_current_vel.x = 0.0f;
		if (entity_floor)
		{
			App->entity->SetEntityState(ENTITY_IDLE_RIGHT, entity_collider);
		}
		else
		{
			App->entity->SetEntityState(ENTITY_JUMP_RIGHT, entity_collider);
		}
	}

	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		entity_current_vel.x = entity_vel.x ;
		if (entity_floor)
		{
			App->entity->SetEntityState(ENTITY_WALK_LEFT, entity_collider);
		}
		else
		{
			App->entity->SetEntityState(ENTITY_JUMP_LEFT, entity_collider);
		}
	}

	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
		entity_current_vel.x = 0.0f;
		if (entity_floor)
		{
			App->entity->SetEntityState(ENTITY_IDLE_LEFT, entity_collider);
		}
		else
		{
			App->entity->SetEntityState(ENTITY_JUMP_LEFT, entity_collider);
		}

	}
	if (entity_floor == true) {
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			actual_gravity = gravity;
			App->audio->PlayFx(App->entity->jump_fx);
			entity_floor = false;
			entity_current_vel.y = -entity_vel.y;

			if (current_state_entity == ENTITY_IDLE_RIGHT || current_state_entity == ENTITY_WALK_RIGHT)
			{
				App->entity->SetEntityState(ENTITY_JUMP_RIGHT, entity_collider);
			}
			else if (current_state_entity == ENTITY_IDLE_LEFT || current_state_entity == ENTITY_WALK_LEFT)
			{
				App->entity->SetEntityState(ENTITY_JUMP_LEFT, entity_collider);
			}
		}
	}
	else if (double_jump == false && entity_floor == false && App->player->IsGod() == false) {

		if (entity_current_vel.y > -1) {
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
				App->audio->PlayFx(App->entity->jump_fx);
				double_jump = true;
				entity_current_vel.y = -entity_vel.y;

				if (current_state_entity == ENTITY_IDLE_RIGHT || current_state_entity == ENTITY_WALK_RIGHT)
				{
					App->entity->SetEntityState(ENTITY_JUMP_RIGHT, entity_collider);
				}
				else if (current_state_entity == ENTITY_IDLE_LEFT || current_state_entity == ENTITY_WALK_LEFT)
				{
					App->entity->SetEntityState(ENTITY_JUMP_LEFT, entity_collider);
				}
			}
		}
	}
	if (App->player->IsGod()) {
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			entity_current_vel.y = -entity_vel.y;
		}
		else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP) {
			entity_current_vel.y = 0.0f;
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			entity_current_vel.y = entity_vel.y;
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP) {
			entity_current_vel.y = 0.0f;
		}
	}
}

void PlayerEntity::Shoot()
{
	Bullet* bullet = (Bullet*) App->entity->CreateEntity(BULLET_TYPE);
	bullet->player_bullet = true;

	switch (current_state_entity)
	{
	case ENTITY_IDLE_LEFT:
	case ENTITY_WALK_LEFT:
	case ENTITY_JUMP_LEFT:
		bullet->entity_pos = { entity_pos.x + collider_size.x * 0.8f, entity_pos.y};
		bullet->entity_current_vel = { bullet->entity_vel.x,0.0f };

		break;
	case ENTITY_IDLE_RIGHT:
	case ENTITY_WALK_RIGHT:
	case ENTITY_JUMP_RIGHT:
		bullet->entity_pos = { entity_pos.x - collider_size.x * 0.8f, entity_pos.y};
		bullet->entity_current_vel = { -bullet->entity_vel.x,0.0f };
		break;
	}
	App->audio->PlayFx(App->entity->shoot_fx);
}

void PlayerEntity::Die(entitySide side)
{
	if (side == entitySide::LEFT) {
		App->entity->SetEntityState(ENTITY_DEAD_LEFT, entity_collider);
	}
	else {
		App->entity->SetEntityState(ENTITY_DEAD_RIGHT, entity_collider);
	}
	App->player->PlayerDies();
}

