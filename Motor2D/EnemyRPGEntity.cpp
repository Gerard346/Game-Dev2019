#include "j1App.h"
#include"p2Log.h"
#include"p2Defs.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Animation.h"
#include "j1Map.h"
#include "j1Module.h"
#include "EntityManager.h"
#include "EnemyRPGEntity.h"
#include "Rocket.h"
#include "j1Player.h"
EnemyRPGEntity::EnemyRPGEntity()
{
	shoot_timer.Start();
}

EnemyRPGEntity::EnemyRPGEntity(const EnemyRPGEntity* copy) :BaseEntity(copy)
{
	shoot_timer.Start();
}

EnemyRPGEntity::~EnemyRPGEntity()
{
}

bool EnemyRPGEntity::Update(float dt)
{
	if (entity_collider == nullptr) {
		return true;
	}
	if (shoot_timer.ReadSec() > shoot_rate)
	{
		shoot_timer.Start();

		Shoot();
	}

	UpdatePosition();

	return true;
}

bool EnemyRPGEntity::CleanUp()
{
	return true;
}

void EnemyRPGEntity::HandleInput(float dt)
{
}

void EnemyRPGEntity::Shoot()
{
	Rocket* rocket = (Rocket*)App->entity->CreateEntity(ROCKET_TYPE);
	rocket->player_bullet = false;

	switch (current_state_entity)
	{
	case ENTITY_IDLE_LEFT:
	case ENTITY_WALK_LEFT:
	case ENTITY_JUMP_LEFT:
		rocket->entity_pos = { entity_pos.x + collider_size.x * 0.8f, entity_pos.y };

		break;
	case ENTITY_IDLE_RIGHT:
	case ENTITY_WALK_RIGHT:
	case ENTITY_JUMP_RIGHT:
		rocket->entity_pos = { entity_pos.x - collider_size.x * 0.8f, entity_pos.y };

		break;
	}
	App->audio->PlayFx(App->entity->shoot_fx);
}