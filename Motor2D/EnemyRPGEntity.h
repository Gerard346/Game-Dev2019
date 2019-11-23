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
	bool is_player_seen = false;
};
