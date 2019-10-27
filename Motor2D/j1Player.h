#ifndef _j1PLAYER_H__
#define _j1PLAYER_H__

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "j1Timer.h"

struct SDL_Texture;
class Animation;

enum Levels {
	Lvl_1,
	Lvl_2
};

enum playerState 
{
	P_NONE,
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

private:

	bool Awake(const pugi::xml_node& node) override;

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

public:

	bool Load(const pugi::xml_node&);

	bool Save(pugi::xml_node&);

	void OnCollision(Collider*, Collider*) override;

	void DrawPlayer(float dt);

	void ChangeLvl();

	void StartFromLvl1();

	void StartFromLvl2();

	void StatFromCurrentLvl();

	void GodMode();

	bool IsGod();

	void PlayerDies();

	void CamFollowPlayer();

	void PlayerInput(float dt);

	void SpawnPlayer();


public:

	playerState p_current_state = IDLE_RIGHT;
	Animation* current_animation = nullptr;

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

	int g_is_over_fx = -1;
	int jump_fx = -1;

	char* g_is_over_fx_path = nullptr;
	char* jump_fx_path = nullptr;

	float animation_index = 0;

	void SetPlayerState(playerState new_state);

};
#endif