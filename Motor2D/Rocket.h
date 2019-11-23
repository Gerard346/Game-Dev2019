#include "BaseEntity.h"

class Rocket : public BaseEntity {
public:
	Rocket();
	Rocket(const Rocket* copy);

	~Rocket();

	bool Update(float dt);

	bool CleanUp();

	void HandleInput(float dt);

private:

};
