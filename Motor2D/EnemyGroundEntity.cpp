#include "j1App.h"
#include"p2Log.h"
#include"p2Defs.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Animation.h"
#include "j1Map.h"
#include "j1Module.h"
#include "EntityManager.h"
#include "EnemyGroundEntity.h"
#include "Rocket.h"

EnemyGroundEntity::EnemyGroundEntity()
{
	shoot_timer.Start();
}

EnemyGroundEntity::EnemyGroundEntity(const EnemyGroundEntity* copy):BaseEntity(copy)
{
	shoot_timer.Start();
}

EnemyGroundEntity::~EnemyGroundEntity()
{
}

bool EnemyGroundEntity::Update(float dt)
{
	if (shoot_timer.ReadSec() > shoot_rate)
	{
		shoot_timer.Start();

		Shoot();
	}

	UpdatePosition();

	return true;
}


bool EnemyGroundEntity::CleanUp()
{
	return true;
}

void EnemyGroundEntity::Shoot()
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
}

