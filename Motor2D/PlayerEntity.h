#ifndef PLAYER_ENTITY
#define PLAYER_ENTITY 

#include "BaseEntity.h"
#include "j1Input.h"

class PlayerEntity : public BaseEntity {
public:

	PlayerEntity();
	PlayerEntity(const PlayerEntity* copy);

	~PlayerEntity();

	bool Update(float dt);

	bool CleanUp();

	void HandleInput(float dt);

public:
	bool double_jump = false;
	float actual_gravity = gravity;

protected:
	void Shoot();
	void Die(entitySide);
};
#endif