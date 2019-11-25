#ifndef ENEMYGROUND_ENTITY
#define ENEMYGROUND_ENTITY

#include "BaseEntity.h"

class EnemyGroundEntity : public BaseEntity {
public:
	EnemyGroundEntity();
	EnemyGroundEntity(const EnemyGroundEntity* copy);
	
	~EnemyGroundEntity();

	bool Update(float dt);

	bool CleanUp();

private:

	j1Timer recharge_timer;
	j1Timer shoot_timer;
	float shoot_rate = 2.0f;
	float rechage_rate = 8.0f;
	int ammo = 4;

	fPoint view_distance = { 2250.0f,0.0f };
	p2DynArray<iPoint>	path;
	iPoint				path_next_pos = { 0.0f,0.0f };

protected:
	void Shoot();
};
#endif