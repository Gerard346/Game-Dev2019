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

EnemyRPGEntity::EnemyRPGEntity()
{
}

EnemyRPGEntity::EnemyRPGEntity(const EnemyRPGEntity* copy) :BaseEntity(copy)
{
}

EnemyRPGEntity::~EnemyRPGEntity()
{
}

bool EnemyRPGEntity::Update(float dt)
{
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
