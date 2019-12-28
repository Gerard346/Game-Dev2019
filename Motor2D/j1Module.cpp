#include "j1Module.h"

void j1Module::Activate()
{
	active = true;
}

void j1Module::Desactivate()
{
	active = false;
}

bool j1Module::GetActive() const
{
	return active;
}

