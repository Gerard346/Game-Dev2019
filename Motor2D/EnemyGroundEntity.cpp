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

EnemyGroundEntity::EnemyGroundEntity()
{
}

EnemyGroundEntity::EnemyGroundEntity(const EnemyGroundEntity* copy):BaseEntity(copy)
{
}

EnemyGroundEntity::~EnemyGroundEntity()
{
}

bool EnemyGroundEntity::Update(float dt)
{
	if (current_state_entity == entityState::ENTITY_DEAD) {
		return true;
	}

	entity_pos.x += entity_current_vel.x * App->Getdt();
	entity_pos.y += entity_current_vel.y * App->Getdt();

	entity_current_vel.y -= gravity * App->Getdt();

	entity_collider->SetPos(entity_pos.x, entity_pos.y);

	return true;
}


bool EnemyGroundEntity::CleanUp()
{
	return true;
}

void EnemyGroundEntity::HandleInput(float dt)
{
}
