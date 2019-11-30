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

	fPoint view_distance = { 250.0f,0.0f };
	fPoint is_viewed = { 1000.0f,0.0f };
	p2DynArray<iPoint>	path;
	iPoint				path_next_pos = { 0.0f,0.0f };

	float recalculate_path = 2.0f;
	j1Timer timer_path;
protected:

	void Shoot();
};
#endif