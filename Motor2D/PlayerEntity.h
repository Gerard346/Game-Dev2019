#include "BaseEntity.h"
#include "j1Input.h"

class PlayerEntity : public BaseEntity {
public:

	PlayerEntity();
	PlayerEntity(const PlayerEntity* copy);

	~PlayerEntity();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void HandleInput(float dt);

public:
	bool double_jump = false;

private:
	int g_is_over_fx = -1;
	int jump_fx = -1;

	char* g_is_over_fx_path = nullptr;
	char* jump_fx_path = nullptr;


};