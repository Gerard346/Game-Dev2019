#include "j1App.h"
#include"p2Log.h"
#include"p2Defs.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Animation.h"
#include "j1Map.h"
#include "j1Module.h"
#include "EntityManager.h"
#include "EnemyGroundEntity.h"
#include "Rocket.h"
#include "j1Pathfinding.h"

EnemyGroundEntity::EnemyGroundEntity()
{
	shoot_timer.Start();
}

EnemyGroundEntity::EnemyGroundEntity(const EnemyGroundEntity* copy):BaseEntity(copy)
{
	shoot_timer.Start();
}

EnemyGroundEntity::~EnemyGroundEntity()
{
}

bool EnemyGroundEntity::Update(float dt)
{
	BaseEntity* entity_player = (BaseEntity*)App->entity->GetPlayer();
	//Is seen
	if (entity_player != nullptr) {

		fPoint dir_vector = entity_player->entity_pos - entity_pos;
		float vec_magnitude = (abs(dir_vector.x) + abs(dir_vector.y));

		if (vec_magnitude < view_distance.x) {
			//Shooting
			if (shoot_timer.ReadSec() > shoot_rate && ammo > 0)
			{
				shoot_timer.Start();
				Shoot();
				ammo -= 1;
				if (ammo == 0) {
					recharge_timer.Start();
				}
			}
			//Recharge
			if (ammo == 0 && recharge_timer.ReadSec() > rechage_rate)
			{
				ammo = 4;
				shoot_timer.Start();
			}

			//Path
			
			if (path.Count() == 0 && path_next_pos.y == 0)
			{
				path = App->path->PropagateASTARf(entity_pos, entity_player->entity_pos);
				if (path.Count() == 0)
				{
					//PATROL
				}
				else
				{
					path.Pop(path_next_pos);
					path_next_pos = App->map->MapToWorld(path_next_pos.x, path_next_pos.y);
				}
			}
			else
			{
				fPoint f_path_point = { (float)path_next_pos.x , (float)path_next_pos.y };
				fPoint dir_vector = f_path_point - entity_pos;
				float vec_magnitude = (abs(dir_vector.x) + abs(dir_vector.y));
				dir_vector = { dir_vector.x / vec_magnitude , dir_vector.y / vec_magnitude };

				if (vec_magnitude < 10.0f)
				{
					if (path.Count() == 0)
					{
						path_next_pos.x = path_next_pos.y = 0;
					}
					else
					{
						path.Pop(path_next_pos);
						path_next_pos = App->map->MapToWorld(path_next_pos.x, path_next_pos.y);
					}
				}
				else
				{
					entity_current_vel += { dir_vector.x* entity_vel.x* App->Getdt(), dir_vector.y* entity_vel.y* App->Getdt()};
				}
			}
		}
	}
	
	UpdatePosition();

	return true;
}


bool EnemyGroundEntity::CleanUp()
{
	return true;
}

void EnemyGroundEntity::Shoot()
{
	Bullet* bullet = (Bullet*)App->entity->CreateEntity(BULLET_TYPE);
	bullet->player_bullet = false;

	switch (current_state_entity)
	{
	case ENTITY_IDLE_LEFT:
	case ENTITY_WALK_LEFT:
	case ENTITY_JUMP_LEFT:
		bullet->entity_pos = { entity_pos.x + collider_size.x * 0.8f, entity_pos.y };
		bullet->entity_current_vel = { bullet->entity_vel.x,0.0f };

		break;
	case ENTITY_IDLE_RIGHT:
	case ENTITY_WALK_RIGHT:
	case ENTITY_JUMP_RIGHT:
		bullet->entity_pos = { entity_pos.x - collider_size.x * 0.8f, entity_pos.y };
		bullet->entity_current_vel = { -bullet->entity_vel.x,0.0f };
		break;
	}
}

