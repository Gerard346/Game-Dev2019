#ifndef _j1PLAYER_H__
#define _j1PLAYER_H__

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "p2Point.h"
//#include "j1Animation.h"
struct SDL_Texture;

enum Levels {
	Lvl_1,
	Lvl_2
};

enum playerState {
	IDLE_LEFT,
	WALK_LEFT,
	JUMP_LEFT,
	IDLE_RIGHT,
	WALK_RIGHT,
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

	bool Awake(const pugi::xml_node& node) override;

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	bool Load(const pugi::xml_node&);

	bool Save(pugi::xml_node&);

	void OnCollision(Collider*, Collider*) override;

	void ChangeLvl();

	void StartFromLvl1();

	void StartFromLvl2();

	void StatFromCurrentLvl();

	void GodMode();

	bool IsGod();

	void PlayerDies();

	void CamFollowPlayer();

	void PlayerInput();

	void SpawnPlayer();


public:

	playerState p_current_state = IDLE_RIGHT;
	playerDirection p_current_direction = STAND;

	Levels p_current_lvl = Lvl_1;
	fPoint p_vel = { 0.0f,0.0f };
	fPoint p_current_vel = { 0.0f, 0.0f };
	fPoint p_pos = { 0.0f,0.0f };

	float gravity = 0.0f;
	bool p_dead = false;

	Collider* p_collider = nullptr;
	iPoint p_size_collider = { 0, 0};
private:

	SDL_Texture* p_tex = nullptr;
	SDL_Rect p_spawn = { 0,0,0,0 };

	bool p_god = false;
	bool p_floor = false;
	bool double_jump = false;

	unsigned int g_is_over_fx = NULL;
	unsigned int jump_fx = NULL;

};
#endif