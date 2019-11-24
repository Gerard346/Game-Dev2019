#ifndef ROCKET_ENTITY
#define ROCKET_ENTITY

#include "Bullet.h"



class Rocket : public Bullet 
{
public:

	Rocket();
	Rocket(const Rocket* copy);

	~Rocket();

	bool Update(float dt);

	bool CleanUp();

private:

};

#endif