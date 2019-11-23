#include "j1App.h"
#include"p2Log.h"
#include"p2Defs.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Animation.h"
#include "j1Map.h"
#include "j1Module.h"
#include "EntityManager.h"
#include "Rocket.h"

Rocket::Rocket()
{
}

Rocket::Rocket(const Rocket* copy) :BaseEntity(copy)
{
}

Rocket::~Rocket()
{
}

bool Rocket::Update(float dt)
{
	BROFILER_CATEGORY("UpdateRocket", Profiler::Color::BlueViolet);

	if (current_state_entity == entityState::ENTITY_DEAD) {
		return true;
	}

	entity_pos.x += entity_current_vel.x * App->Getdt();
	entity_pos.y += entity_current_vel.y * App->Getdt();

	entity_collider->SetPos(entity_pos.x, entity_pos.y);

	return true;
}


bool Rocket::CleanUp()
{
	return true;
}

void Rocket::HandleInput(float dt)
{
}
