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
	PLAYER_TYPE
};

enum entityState {
	NONE_STATE,
	ENTITY_IDLE_LEFT,
	ENTITY_WALK_LEFT,
	ENTITY_JUMP_LEFT,
	ENTITY_IDLE_RIGHT,
	ENTITY_WALK_RIGHT,
	ENTITY_JUMP_RIGHT,
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
	
public:

	BaseEntity(entityType, char*);
	virtual ~BaseEntity();

	virtual bool Awake(pugi::xml_node&);

	virtual bool Start();

	virtual bool PreUpdate();

	virtual bool Update();

	virtual bool PostUpdate();

	virtual bool CleanUp();
	virtual bool Draw();

	virtual void IsDead();

public:

	entityState state_entity = entityState::NONE_STATE;
	Animation* current_animation = nullptr;

	p2SString name = "";

	fPoint entity_pos = { 0.0f,0.0f };
	fPoint entity_vel = { 0.0f,0.0f };
	fPoint entity_current_vel = { 0.0f, 0.0f };

	float gravity = 0.0f;
	bool entity_dead = false;

	Collider* entity_collider = nullptr;
	iPoint entity_size_collider = { 0, 0 };

	bool entity_floor = false;


private:

	SDL_Texture* entity_tex = nullptr;

	void SetEntityState(entityState new_state);


};
#endif 