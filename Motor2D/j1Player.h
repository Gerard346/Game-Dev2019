#ifndef _j1PLAYER_H__
#define _j1PLAYER_H__

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "Animation.h"

#

struct SDL_Texture;

enum Levels {
	Lvl_1,
	Lvl_2
};

enum playerState {
	IDLE,
	WALK_LEFT,
	WALK_RIGHT,
	RUN_LEFT,
	RUN_RIGHT,
	JUMP_LEFT,
	JUMP_RIGHT,
	DEAD
};

enum playerDirection {
	STAND,
	UP,
	LEFT,
	DOWN,
	RIGHT
};

class j1Player : public j1Module {

public:

	j1Player();

	virtual ~j1Player();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	bool Save(pugi::xml_node&)const;

	bool Load(pugi::xml_node&);

	void ChangeLvl();

	void StartFromLvl1();

	void StartFromLvl2();

	void StatFromCurrentLvl();

	void GodMode();

	bool IsGod();

	void PlayerDies();

	void CamFollowPlayer();

	void PlayerInput();

	void PlayerPosition(fPoint);

	void SpawnPlayer();
public:

	playerState p_current_state = IDLE;
	playerDirection p_current_direction = STAND;

	Levels p_current_lvl;
	fPoint p_vel = { 0.0f,0.0f };
	fPoint p_current_vel = { 0.0f, 0.0f };
	fPoint p_pos = { 0.0f,0.0f };

	float gravity = 0.0f;

	Collider* p_collider = nullptr;
	iPoint p_size_collider = { 16, 32};
private:

	SDL_Texture* p_tex = nullptr;
	SDL_Rect p_spawn = { 0,0,0,0 };

	bool p_dead = false;
	bool p_god = false;
	bool p_floor = false;
	bool double_jump = false;

	unsigned int g_is_over_fx = NULL;
	unsigned int jump_fx = NULL;

};
#endif