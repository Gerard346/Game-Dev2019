#ifndef __j1COLLIDERS_H__
#define __j1COLLIDERS_H__
#include "j1Module.h"
#include "p2List.h"
#include "p2SString.h"
#include "SDL\include\SDL_rect.h"

#define MAX_COLLIDERS 1500

struct SDL_Rect;

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_WALL,
	COLLIDER_PLAYER,
	COLLIDER_ENEMY,
	COLLIDER_BULLET,
	COLLIDER_DEAD,
	COLLIDER_FINISH,
	COLLIDER_START,
	COLLIDER_STICKINESS,
	COLLIDER_PATROL,
	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect = { 0,0,0,0 };
	bool to_delete = false;
	COLLIDER_TYPE type;
	j1Module* callback = nullptr;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
	bool Update();
};

class j1Colliders : public j1Module
{

public:

	j1Colliders();
	~j1Colliders();

	// Called before render is available


	// Called before the first frame


	// Called each loop iteration
	bool Update(float);
	// Called each loop iteration
	bool PostUpdate();
	// Called before quitting
	bool CleanUp();
	bool EraseCollider(Collider* collider);
	bool EraseAllColliders();
	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback = nullptr);
	
	COLLIDER_TYPE TileIDToColliderTile(int tile_id) const;

	void Desactivate()override;
	void Activate()override;

	void DebugMode();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;



public:

	bool						debug = false;
private:

	Collider* colliders[MAX_COLLIDERS];
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
};

#endif 