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


protected:
	
	void Shoot();
};
#endif