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

Rocket::Rocket(const Rocket* copy) :Bullet(copy)
{
}

Rocket::~Rocket()
{
}

bool Rocket::Update(float dt)
{
	BROFILER_CATEGORY("UpdateRocket", Profiler::Color::BlueViolet);
	
	BaseEntity* player_entity = (BaseEntity*)App->entity->GetPlayer();

	if (player_entity != nullptr)
	{
		fPoint dir_vector = player_entity->entity_pos - entity_pos;
		float vec_magnitude = (abs(dir_vector.x) + abs(dir_vector.y));
		dir_vector.x = dir_vector.x / vec_magnitude;
		dir_vector.y = dir_vector.y / vec_magnitude;

		dir_vector.x *= entity_vel.x;
		dir_vector.y *= entity_vel.x;

		entity_current_vel.x += (dir_vector.x - entity_current_vel.x) * dt * acceleration;
		entity_current_vel.y += (dir_vector.y - entity_current_vel.y) * dt * acceleration;
	}

	UpdatePosition();

	return true;
}

bool Rocket::CleanUp()
{
	return true;
}
