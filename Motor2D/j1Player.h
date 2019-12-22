#ifndef _j1PLAYER_H__
#define _j1PLAYER_H__

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "j1Timer.h"

struct SDL_Texture;

class Animation;

enum Levels {
	Lvl_undefined,
	Lvl_1,
	Lvl_2
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

	void ChangeLvl();

	void StartFromLvl1();

	void StartFromLvl2();

	void StatFromCurrentLvl();

	void GodMode();

	bool IsGod() const;

	bool IsDead() const;

	void setAlive();

	void PlayerDies();

	void CamFollowPlayer();

public:
	Levels p_current_lvl = Lvl_1;

private:
	bool p_god = false;
	bool p_floor = false;
	bool double_jump = false;
	bool p_dead = false;
	int g_is_over_fx = -1;
	int next_lvl_fx = -1;

	char* g_is_over_fx_path = nullptr;
	char* next_lvl_fx_path = nullptr;

	bool start_from_current_lvl = false;
};
#endif