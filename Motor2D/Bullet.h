#ifndef BULLET_ENTITY
#define BULLET_ENTITY

#include "BaseEntity.h"

class Bullet : public BaseEntity {
public:

	Bullet();
	Bullet(const Bullet* copy);

	~Bullet();

public:
	bool player_bullet = false;

};
#endif