#include "p2List.h"
#include "j1Module.h"
#include "BaseEntity.h"

class EntityManager : public j1Module
{

public:

	EntityManager();
	virtual ~EntityManager();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	//Handle Entities
	BaseEntity* FindEntity(const Collider* col) const;
	BaseEntity* CreateEntity(entityType);
	bool KillEntity(BaseEntity*);

private:

};
