#ifndef __J1FADETOBLACK_H__
#define __J1FADETOBLACK_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"


class j1FadeToBlack : public j1Module
{


public:

	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	};

	j1FadeToBlack();
	~j1FadeToBlack();

	bool Start();
	bool Update(float dt);
	bool FadeToBlack(j1Module* module_off, j1Module* module_on, float time = 2.0f);
	bool FadeToColor(j1Module* module_off, j1Module* module_on, float time = 2.0f);

	bool isFading() const;

	j1Module* module_off = nullptr;
	j1Module* module_on = nullptr;

	fade_step current_step = none;
private:
	Uint32 start_time = NULL;
	Uint32 total_time = NULL;

	SDL_Rect screen = { 0,0,0,0 };
};

#endif //__MODULEFADETOBLACK_H__
