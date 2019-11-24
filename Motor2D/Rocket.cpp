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

	UpdatePosition();

	return true;
}

bool Rocket::CleanUp()
{
	return true;
}
