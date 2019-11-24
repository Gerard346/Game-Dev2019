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

	j1Timer shoot_timer;
	float shoot_rate = 2.0f;

protected:
	void Shoot();
};
#endif