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
}

EnemyGroundEntity::EnemyGroundEntity(const EnemyGroundEntity* copy):BaseEntity(copy)
{
}

EnemyGroundEntity::~EnemyGroundEntity()
{
}

bool EnemyGroundEntity::Update(float dt)
{
	
	UpdatePosition();

	return true;
}


bool EnemyGroundEntity::CleanUp()
{
	return true;
}

void EnemyGroundEntity::Shoot()
{
}

