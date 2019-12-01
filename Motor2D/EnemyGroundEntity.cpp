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
	timer_path.Start();
}

EnemyGroundEntity::EnemyGroundEntity(const EnemyGroundEntity* copy):BaseEntity(copy)
{
	shoot_timer.Start();
	timer_path.Start();
}

EnemyGroundEntity::~EnemyGroundEntity()
{
	
}

void EnemyGroundEntity::Start()
{
	App->entity->SetEntityState(ENTITY_IDLE_RIGHT, this->entity_collider);
	App->path->TypePathfinding(WALK);
	App->map->GetPatrolPoints(entity_pos.x, entity_pos.y, a_point, b_point);
}

bool EnemyGroundEntity::Update(float dt)
{
	BaseEntity* entity_player = (BaseEntity*)App->entity->GetPlayer();
	//Is seen
	if (entity_player != nullptr) {
		fPoint dir_vector = entity_player->entity_pos - entity_pos;
		float vec_magnitude = (abs(dir_vector.x) + abs(dir_vector.y));
		if (vec_magnitude < is_viewed.x) {
			if (vec_magnitude < view_distance.x) {
				//Shooting
				if (shoot_timer.ReadSec() > shoot_rate&& ammo > 0)
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
			}
			//Path
			App->path->TypePathfinding(typePathfinding::WALK);
			if (path.Count() == 0 && path_next_pos.y == 0 || timer_path.ReadSec() > recalculate_path)
			{
				timer_path.Start();
				if (App->path->PropagateASTARf(entity_pos, entity_player->entity_pos, path) == false)
				{
					//PATROL
					float a_distance = entity_pos.DistanceManhattan(a_point);
					float b_distance = entity_pos.DistanceManhattan(b_point);
					fPoint goal = a_distance < b_distance ? b_point : a_point;

					if (App->path->PropagateASTARf(entity_pos, goal, path) == true)
					{
						LOG("CRIDAt");
						path.Pop(path_next_pos);
						path_next_pos = App->map->MapToWorld(path_next_pos.x, path_next_pos.y);
						LOG("CRIDAdsat");
					}
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

				if (vec_magnitude < 16.0f)
				{
					if (path.Count() == 0)
					{
						path_next_pos.x = path_next_pos.y = 0;
						entity_current_vel = { 0.0f, 0.0f };
					}
					else
					{
						path.Pop(path_next_pos);
						path_next_pos = App->map->MapToWorld(path_next_pos.x, path_next_pos.y);
					}
				}
				else
				{
					if (App->Getdt() < 1.0f / App->GetCappedFrames())
					{
						entity_current_vel += { dir_vector.x* entity_vel.x* App->Getdt()* vec_magnitude, dir_vector.y* entity_vel.y* App->Getdt()* vec_magnitude * 0.0005f};
						//entity_current_vel += { dir_vector.x* entity_vel.x* App->Getdt()* 1000.0f, dir_vector.y* entity_vel.y* App->Getdt() * 1000.0f};
					}
				}
			}
		}
	}
	
	UpdatePosition();
	AnimationUpdate();
	
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
	App->audio->PlayFx(App->entity->shoot_fx);
}

