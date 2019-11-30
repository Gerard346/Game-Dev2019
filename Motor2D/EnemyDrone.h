#ifndef ENEMYDRONE_ENTITY
#define ENEMYDRONE_ENTITY

#include "BaseEntity.h"

class EnemyDrone : public BaseEntity {
public:
	EnemyDrone();
	EnemyDrone(const EnemyDrone* copy);

	~EnemyDrone();

	bool Update(float dt);

	bool CleanUp();

private:

	j1Timer recharge_timer;
	j1Timer shoot_timer;
	float shoot_rate = 1.0f;
	float rechage_rate = 2.0f;
	int ammo = 4;
	fPoint is_viewed = { 1000.0f,0.0f };
	fPoint view_distance = { 600.0f,0.0f };
	p2DynArray<iPoint>	path;
	iPoint				path_next_pos = { 0.0f,0.0f };

	float recalculate_path = 2.0f;
	j1Timer timer_path;
	float acceleration = 2.0f;

protected:
	void Shoot();
};
#endif