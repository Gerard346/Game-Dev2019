#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1FadeToBlack.h"
#include "j1Player.h"
#include <math.h>
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "j1Input.h"

j1FadeToBlack::j1FadeToBlack() : j1Module()
{
	name = ("fadetoblack");
}

j1FadeToBlack::~j1FadeToBlack()
{}

bool j1FadeToBlack::Start()
{
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);

	uint width =0;
	uint height =0;
	uint scale= 0;

	App->win->GetWindowSize(width, height);
	scale = App->win->GetScale();

	screen = { 0, 0, (int)(width * scale), (int)(height * scale) };

	return true;
}

bool j1FadeToBlack::PostUpdate()
{
	if (current_step == none)
	{
		return true;
	}

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_to_black:
	{
		LOG("%f", App->Getdt());

		if (now >= total_time)
		{
			if (module_off != NULL) {
				module_off->CleanUp();
				module_off->Desactivate();
			}
			if (module_on != NULL) {
				module_on->Start();
				module_on->Activate();
			}

			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = fade_from_black;
			App->input->active = true;
		}
	}break;

	case fade_from_black:
	{
		normalized = 1.0f - normalized;

		if (now >= total_time)
		{
			current_step = none;
		}

	}break;
	}

	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return true;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool j1FadeToBlack::FadeToBlack(j1Module* module_off, j1Module* module_on, float time)
{
	bool ret = false;

	if (current_step == none)
	{
		App->input->active = false;
		current_step = fade_to_black;
		this->module_off = module_off;
		this->module_on = module_on;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		ret = true;
	}

	return ret;
}

bool j1FadeToBlack::FadeToColor(j1Module* module_off, j1Module* module_on, float time)
{
	bool ret = false;

	if (current_step == none)
	{
		current_step = fade_from_black;
		this->module_off = module_off;
		this->module_on = module_on;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		ret = true;
	}

	return ret;
}

bool j1FadeToBlack::isFading() const
{
	return current_step != none;
}
