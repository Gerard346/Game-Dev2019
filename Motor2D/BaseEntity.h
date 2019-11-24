#ifndef _BASEENTITY_H__
#define _BASEENTITY_H__

#include "p2Point.h"
#include "p2SString.h"
#include "p2DynArray.h"
#include "PugiXml\src\pugixml.hpp"
#include "SDL/include/SDL.h"

#include "j1Module.h"
#include"j1Colliders.h"

struct SDL_Texture;
class Animation;

enum entityType {
	UNKNOWN,
	PLAYER_TYPE,
	ENEMY_GROUND_TYPE,
	ENEMY_RPG_TYPE,
	BULLET_TYPE,
	ROCKET_TYPE,
	RPG_TYPE
};

enum entityState {
	NONE_STATE,
	ENTITY_IDLE_LEFT,
	ENTITY_WALK_LEFT,
	ENTITY_JUMP_LEFT,
	ENTITY_IDLE_RIGHT,
	ENTITY_WALK_RIGHT,
	ENTITY_JUMP_RIGHT,
	ENTITY_SHOOTING,
	ENTITY_DEAD
};

enum entityDirection {
	ENTITY_STAND,
	ENTITY_UP,
	ENTITY_LEFT,
	ENTITY_DOWN,
	ENTITY_RIGHT
};

class BaseEntity {
	
	friend class EntityManager;

public:

	BaseEntity();
	BaseEntity(const BaseEntity* copy);

	virtual ~BaseEntity();

	virtual void IsDead();

public:

	entityType entity_type = entityType::UNKNOWN;
	entityState current_state_entity = entityState::NONE_STATE;
	Animation* current_animation = nullptr;
	float animation_index = 0.0f;

	p2SString name = "";

	fPoint entity_pos = { 0.0f,0.0f };
	fPoint entity_vel = { 0.0f,0.0f };
	fPoint entity_current_vel = { 0.0f, 0.0f };
	
	float gravity = 0.0f;
	
	Collider* entity_collider = nullptr;
	COLLIDER_TYPE collider_type = COLLIDER_TYPE::COLLIDER_NONE;
	iPoint collider_size = { 0,0 };

	bool entity_floor = false;

protected:

	virtual bool Update(float);
	virtual bool CleanUp();

	virtual bool Draw();

	void UpdatePosition();
	void Die();
};
#endif 