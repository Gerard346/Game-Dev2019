#include "j1App.h"
#include"p2Log.h"
#include"p2Defs.h"
#include "j1Audio.h"
#include "PlayerEntity.h"
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

bool PlayerEntity::Awake(pugi::xml_node& node)
{
	BROFILER_CATEGORY("AwakePlayerEntity", Profiler::Color::DarkOliveGreen);
	
	return true;
}

bool PlayerEntity::Start()
{
	BROFILER_CATEGORY("StartPlayerEntity", Profiler::Color::DarkOliveGreen);

	return true;
}

bool PlayerEntity::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdatePlayerEntity", Profiler::Color::DarkOliveGreen);
	if (current_state_entity == entityState::ENTITY_DEAD)
		return true;

	HandleInput(App->Getdt());

	entity_pos.x += entity_current_vel.x * App->Getdt();
	entity_pos.y += entity_current_vel.y * App->Getdt();

	if (App->player->IsGod() == false) {
		entity_current_vel.y -= gravity * App->Getdt();
	}

	entity_collider->SetPos(entity_pos.x, entity_pos.y);

	return true;
}

bool PlayerEntity::Update(float dt)
{
	BROFILER_CATEGORY("UpdatePlayerEntity", Profiler::Color::DarkOliveGreen);

	return true;
}

bool PlayerEntity::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdatePlayerEntity", Profiler::Color::DarkOliveGreen);
	Draw();
	return true;
}

bool PlayerEntity::CleanUp()
{
	return true;
}



void PlayerEntity::HandleInput(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		entity_current_vel.x = entity_vel.x * -1 * dt;
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
		entity_current_vel.x = entity_vel.x * dt;
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
			App->audio->PlayFx(jump_fx);
			entity_floor = false;
			entity_current_vel.y = -entity_vel.y * dt;

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
				App->audio->PlayFx(jump_fx);
				double_jump = true;
				entity_current_vel.y = -entity_vel.y * dt;

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
			entity_current_vel.y = -entity_vel.x * dt;
		}
		else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP) {
			entity_current_vel.y = 0.0f;
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			entity_current_vel.y = entity_vel.x * dt;
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP) {
			entity_current_vel.y = 0.0f;
		}
	}
}
