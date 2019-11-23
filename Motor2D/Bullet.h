#include "BaseEntity.h"

class Bullet : public BaseEntity {
public:
	Bullet();
	Bullet(const Bullet* copy);

	~Bullet();

	bool Update(float dt);

	bool CleanUp();

	void HandleInput(float dt);

private:
	bool is_player_seen = false;
};
