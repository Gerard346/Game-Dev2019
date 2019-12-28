#include "p2List.h"
#include "j1Module.h"
#include "BaseEntity.h"
#include "p2Point.h"

class PlayerEntity;

class EntityManager : public j1Module
{
public:

	EntityManager();
	virtual ~EntityManager();

	bool Awake(const pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);

	bool Load(const pugi::xml_node&);
	bool Save(pugi::xml_node&);

	void Activate();
	void Desactivate();
public:

	//Handle Entities
	BaseEntity* FindEntity(const Collider* col) const;
	BaseEntity* FindDeadEntity(const Collider* col) const;

	BaseEntity* CreateEntity(entityType);
	bool KillEntity(BaseEntity*);
	void SetEntityState(entityState new_state, Collider* c1);
	
	entityType StrToEntityType(const char* input)const;
	char* EntityTypeToStr(entityType) const;
	entityState StringToEntityState(const char* str)const;
	entityType TileIdToEntityType(int) const;
	
	bool SpawnEntities(p2DynArray<std::pair<entityType, iPoint>>& list);
	bool DeleteAllEntities();

	bool EntitiesLoad();

	PlayerEntity* GetPlayer()const;
	entitySide GetEntitySideView(BaseEntity*);

public:

	int shoot_fx = -1;
	int jump_fx = -1;

private:

	char* shoot_fx_path = nullptr;
	char* jump_fx_path = nullptr;

	p2DynArray <BaseEntity*> values_entities;

	p2List <BaseEntity*> entities_list;
	p2List <BaseEntity*> new_entities;
	p2List <BaseEntity*> dead_entities;
	p2List <BaseEntity*> dead_entities_not_visible;
	p2List <EntityReference> pending_entities;

	bool load_pending = false;
	pugi::xml_node load_node;

};
