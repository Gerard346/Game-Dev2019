#include "BaseEntity.h"

class EnemyGroundEntity : public BaseEntity {
public:
	EnemyGroundEntity();
	EnemyGroundEntity(const EnemyGroundEntity* copy);
	
	~EnemyGroundEntity();

	bool Update(float dt);

	bool CleanUp();

	void HandleInput(float dt);

private:
	bool is_player_seen = false;
};
