#ifndef ENEMYRPG_ENTITY
#define ENEMYRPG_ENTITY 

#include "BaseEntity.h"

class EnemyRPGEntity : public BaseEntity {
public:
	EnemyRPGEntity();
	EnemyRPGEntity(const EnemyRPGEntity* copy);

	~EnemyRPGEntity();

	bool Update(float dt);

	bool CleanUp();

	void HandleInput(float dt);

private:

	j1Timer shoot_timer;
	float shoot_rate = 2.0f;

protected:
	void Shoot();
private:
	bool is_player_seen = false;
};
#endif