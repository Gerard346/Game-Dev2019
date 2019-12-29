#include "EntityManager.h"
#include "Brofiler/Brofiler.h"
#include "j1App.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "PlayerEntity.h"
#include "EnemyGroundEntity.h"
#include "EnemyRPGEntity.h"
#include "EnemyDrone.h"
#include "Bullet.h"
#include "Rocket.h"
#include "Ammo.h"
#include "j1Module.h"
#include "j1Player.h"
#include "j1Animation.h"
#include "p2Log.h"
#include "SDL/include/SDL_rect.h"
#include "BaseEntity.h"
#include "j1FadeToBlack.h"

EntityManager::EntityManager()
{
	name = ("entityManager");
}

EntityManager::~EntityManager()
{

}

bool EntityManager::Awake(const pugi::xml_node& node)
{
	BROFILER_CATEGORY("AwakeEntityManager", Profiler::Color::LightGoldenRodYellow);

	if (node != nullptr)
	{
		pugi::xml_node entity_node = node.first_child();


		while (entity_node != NULL)
		{
			BaseEntity* entity = nullptr;
			entityType type = StrToEntityType(entity_node.child("type").child_value());

			switch (type)
			{
			case PLAYER_TYPE:
			{
				entity = new PlayerEntity();
				entity->gravity = entity_node.child("gravity").attribute("g").as_float();
			}
			break;

			case ENEMY_GROUND_TYPE:
			{
				entity = new EnemyGroundEntity();

				entity->gravity = entity_node.child("gravity").attribute("g").as_float();
			}

			break;

			case DRONE_TYPE:
			{
				entity = new EnemyDrone();

				entity->gravity = entity_node.child("gravity").attribute("g").as_float();
			}

			break;
			case ENEMY_RPG_TYPE:
			{
				entity = new EnemyRPGEntity();

				entity->gravity = entity_node.child("gravity").attribute("g").as_float();
			}

			break;

			case BULLET_TYPE:
			{
				entity = new Bullet();

			}
			break;

			case AMMO_TYPE:
			{
				entity = new Ammo();

			}
			break;

			case ROCKET_TYPE:
			{
				entity = new Rocket();

				entity->gravity = entity_node.child("gravity").attribute("g").as_float();
			}

			break;
			}
			if (entity != nullptr)
			{
				entity->entity_type = type;
				entity->entity_vel = fPoint(entity_node.child("max_speed").attribute("x").as_float(), entity_node.child("max_speed").attribute("y").as_float());
				entity->collider_size = iPoint(entity_node.child("collider_size").attribute("x").as_int(), entity_node.child("collider_size").attribute("y").as_int());
				entity->entity_draw_dead = entity_node.child("draw_dead").attribute("d").as_bool();
				values_entities.PushBack(entity);
			}

			entity_node = entity_node.next_sibling();
		}
		jump_fx_path = (char*)node.child("fx_jump").child_value();
		shoot_fx_path = (char*)node.child("fx_shoot").child_value();
	}

	else {
		LOG("Error in Awake %s", name);
	}

	return true;
}

bool EntityManager::Start()
{
	if (jump_fx == -1) {
		jump_fx = App->audio->LoadFx(jump_fx_path);
	}
	if (shoot_fx == -1) {
		shoot_fx = App->audio->LoadFx(shoot_fx_path);
	}
	return true;
}

bool EntityManager::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdatEntityManager", Profiler::Color::LightGoldenRodYellow);

	while (new_entities.count() > 0)
	{
		for (int i = 0; i < new_entities.count(); i++) {
			BaseEntity* new_entity = new_entities.At(i)->data;
			new_entity->Start();
			entities_list.add(new_entity);
		}
		new_entities.clear();
	}

	if (!App->scene->GetPause()) {
		for (int i = 0; i < entities_list.count(); i++) {
			entities_list.At(i)->data->Update(App->Getdt());
		}
	}

	return true;
}

bool EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdateEntityManager", Profiler::Color::LightGoldenRodYellow);

	for (int i = 0; i < entities_list.count(); i++) {
		entities_list.At(i)->data->Draw();
	}
	for (int i = 0; i < dead_entities.count(); i++) {
		dead_entities.At(i)->data->Draw();
	}
	return true;
}

bool EntityManager::CleanUp()
{
	for (int i = 0; i < entities_list.count(); i++) {
		entities_list.At(i)->data->CleanUp();
		App->colliders->EraseCollider(entities_list.At(i)->data->entity_collider);
		RELEASE(entities_list.At(i)->data);
	}
	for (int i = 0; i < dead_entities_not_visible.count(); i++) {
		dead_entities_not_visible.At(i)->data->CleanUp();
		App->colliders->EraseCollider(dead_entities_not_visible.At(i)->data->entity_collider);
		RELEASE(dead_entities_not_visible.At(i)->data);
	}

	entities_list.clear();
	dead_entities_not_visible.clear();
	return true;
}

void EntityManager::OnCollision(Collider* coll, Collider* coll2)
{
	if (coll->type == COLLIDER_PLAYER && coll2->type == COLLIDER_WALL) {
		PlayerEntity* player_entity = (PlayerEntity*)FindEntity(coll);
		if (player_entity == nullptr)
		{
			return;
		}

		//TOP
		if (coll2->rect.y - (coll->rect.y + coll->rect.h) < 0 && coll2->rect.y - (coll->rect.y + coll->rect.h) > -16) {

			coll->rect.y = coll2->rect.y - coll->rect.h;
			player_entity->entity_pos.y = coll2->rect.y - coll->rect.h;

			if (player_entity->entity_current_vel.y > 0.0f) {
				player_entity->entity_current_vel.y = 0.0f;
			}

			player_entity->entity_floor = true;
			player_entity->double_jump = false;

			if (player_entity->current_state_entity == ENTITY_JUMP_LEFT)
			{
				SetEntityState(ENTITY_IDLE_LEFT, player_entity->entity_collider);
			}

			else if (player_entity->current_state_entity == ENTITY_JUMP_RIGHT)
			{
				SetEntityState(ENTITY_IDLE_RIGHT, player_entity->entity_collider);
			}
		}
		//LEFT
		else if (coll2->rect.x - (coll->rect.x + coll->rect.w) < 0 && coll2->rect.x - (coll->rect.x + coll->rect.w) > -16) {
			coll->rect.x = coll2->rect.x - coll->rect.w;
			player_entity->entity_pos.x = coll2->rect.x - coll->rect.w;
			player_entity->entity_current_vel.x = 0.0f;
		}
		//RIGHT
		else if (coll->rect.x - (coll2->rect.x + coll2->rect.w) < 0 && coll->rect.x - (coll2->rect.x + coll2->rect.w) > -16) {
			coll->rect.x = coll2->rect.x + coll2->rect.w;
			player_entity->entity_pos.x = coll2->rect.x + coll2->rect.w;
			player_entity->entity_current_vel.x = 0.0f;
		}
	}
	//STICKINESS
	if (coll->type == COLLIDER_PLAYER && coll2->type == COLLIDER_STICKINESS) {
		PlayerEntity* player_entity = (PlayerEntity*)FindEntity(coll);
		if (player_entity == nullptr)
		{
			return;
		}

		//TOP
		if (coll2->rect.y - (coll->rect.y + coll->rect.h) < 0 && coll2->rect.y - (coll->rect.y + coll->rect.h) > -16) {
			player_entity->actual_gravity = player_entity->gravity - 2000;
			coll->rect.y = coll2->rect.y - coll->rect.h;
			player_entity->entity_pos.y = coll2->rect.y - coll->rect.h;

			if (player_entity->entity_current_vel.y > 0.0f) {
				player_entity->entity_current_vel.y = 0.0f;
			}

			player_entity->entity_floor = true;
			player_entity->double_jump = false;

			if (player_entity->current_state_entity == ENTITY_JUMP_LEFT)
			{
				SetEntityState(ENTITY_IDLE_LEFT, player_entity->entity_collider);
			}

			else if (player_entity->current_state_entity == ENTITY_JUMP_RIGHT)
			{
				SetEntityState(ENTITY_IDLE_RIGHT, player_entity->entity_collider);
			}
		}
		//LEFT
		else if (coll2->rect.x - (coll->rect.x + coll->rect.w) < 0 && coll2->rect.x - (coll->rect.x + coll->rect.w) > -16) {
			coll->rect.x = coll2->rect.x - coll->rect.w;
			player_entity->entity_pos.x = coll2->rect.x - coll->rect.w;
			player_entity->entity_current_vel.x = 0.0f;
		}
		//RIGHT
		else if (coll->rect.x - (coll2->rect.x + coll2->rect.w) < 0 && coll->rect.x - (coll2->rect.x + coll2->rect.w) > -16) {
			coll->rect.x = coll2->rect.x + coll2->rect.w;
			player_entity->entity_pos.x = coll2->rect.x + coll2->rect.w;
			player_entity->entity_current_vel.x = 0.0f;
		}
	}

	//if (App->player->IsChangingLVL())return;

	if (coll->type == COLLIDER_PLAYER && coll2->type == COLLIDER_START) {
		App->WantToSaveCheckpoints();
	}
	if (coll->type == COLLIDER_PLAYER && coll2->type == COLLIDER_AMMO) {
		Ammo* ammo_entity = (Ammo*)FindEntity(coll2);
		App->entity->KillEntity(ammo_entity);
		App->player->SetAmmo(10);
	}
	if (coll->type == COLLIDER_PLAYER && coll2->type == COLLIDER_DEAD) {

		if (App->fade->isFading())return;
		if (App->player->IsDead()) return;
		if (App->player->IsGod())return;

		PlayerEntity* player_entity = (PlayerEntity*)FindEntity(coll);
		if (player_entity == nullptr)
		{
			return;
		}

		player_entity->entity_current_vel.x = 0.0f;
		player_entity->entity_current_vel.y = 0.0f;
		App->entity->KillEntity(player_entity);
	}
	if (coll->type == COLLIDER_PLAYER && coll2->type == COLLIDER_ENEMY) {

		if (App->fade->isFading())return;
		if (App->player->IsDead()) return;
		if (App->player->IsGod())return;

		PlayerEntity* player_entity = (PlayerEntity*)FindEntity(coll);
		if (player_entity == nullptr)
		{
			return;
		}

		player_entity->entity_current_vel.x = 0.0f;
		player_entity->entity_current_vel.y = 0.0f;
		App->entity->KillEntity(player_entity);
	}
	//ENEMIES COLLISION
	if (coll->type == COLLIDER_ENEMY && coll2->type == COLLIDER_WALL) {
		EnemyGroundEntity* enemy_entity = (EnemyGroundEntity*)FindEntity(coll);
		if (enemy_entity == nullptr)
		{
			return;
		}

		//TOP
		if (coll2->rect.y - (coll->rect.y + coll->rect.h) < 0 && coll2->rect.y - (coll->rect.y + coll->rect.h) > -16) {

			coll->rect.y = coll2->rect.y - coll->rect.h;
			enemy_entity->entity_pos.y = coll2->rect.y - coll->rect.h;

			if (enemy_entity->entity_current_vel.y > 0.0f) {
				enemy_entity->entity_current_vel.y = 0.0f;
			}

			enemy_entity->entity_floor = true;

			if (enemy_entity->current_state_entity == ENTITY_JUMP_LEFT)
			{
				SetEntityState(ENTITY_IDLE_LEFT, enemy_entity->entity_collider);
			}

			else if (enemy_entity->current_state_entity == ENTITY_JUMP_RIGHT)
			{
				SetEntityState(ENTITY_IDLE_RIGHT, enemy_entity->entity_collider);
			}
		}
		//LEFT
		else if (coll2->rect.x - (coll->rect.x + coll->rect.w) < 0 && coll2->rect.x - (coll->rect.x + coll->rect.w) > -16) {
			coll->rect.x = coll2->rect.x - coll->rect.w;
			enemy_entity->entity_pos.x = coll2->rect.x - coll->rect.w;
			enemy_entity->entity_current_vel.x = 0.0f;
		}
		//RIGHT
		else if (coll->rect.x - (coll2->rect.x + coll2->rect.w) < 0 && coll->rect.x - (coll2->rect.x + coll2->rect.w) > -16) {
			coll->rect.x = coll2->rect.x + coll2->rect.w;
			enemy_entity->entity_pos.x = coll2->rect.x + coll2->rect.w;
			enemy_entity->entity_current_vel.x = 0.0f;
		}
	}

	if (coll->type == COLLIDER_ENEMY && coll2->type == COLLIDER_DEAD) {

		EnemyGroundEntity* enemy_entity = (EnemyGroundEntity*)FindEntity(coll);
		if (enemy_entity == nullptr)
		{
			return;
		}

		if (enemy_entity->current_state_entity != entityState::ENTITY_DEAD_LEFT || enemy_entity->current_state_entity != entityState::ENTITY_DEAD_RIGHT)
		{
			enemy_entity->entity_current_vel.x = 0.0f;
			enemy_entity->entity_current_vel.y = 0.0f;
			App->entity->KillEntity(enemy_entity);
		}
	}
	//Rocket && Bullet
	if (coll->type == COLLIDER_BULLET)
	{
		Bullet* bullet = (Bullet*)FindEntity(coll);
		if (bullet == nullptr)
		{
			return;
		}

		if (coll2->type == COLLIDER_WALL)
		{
			App->entity->KillEntity(bullet);
		}
		if (coll2->type == COLLIDER_STICKINESS)
		{
			App->entity->KillEntity(bullet);
		}

		if (bullet->player_bullet == true)
		{
			if (coll2->type == COLLIDER_ENEMY)
			{
				BaseEntity* enemy = FindEntity(coll2);
				if (enemy == nullptr)
				{
					return;
				}

				App->entity->KillEntity(bullet);
				App->entity->KillEntity(enemy);
			}
		}
		else
		{
			if (coll2->type == COLLIDER_PLAYER)
			{
				if (App->fade->isFading())return;
				if (App->player->IsDead()) return;
				if (App->player->IsGod())return;

				BaseEntity* player = FindEntity(coll2);
				if (player == nullptr)
				{
					return;
				}

				App->entity->KillEntity(bullet);
				App->entity->KillEntity(player);
			}
		}

		if (coll2->type == COLLIDER_BULLET)
		{
			Bullet* bullet2 = (Bullet*)FindEntity(coll2);
			if (bullet2 == nullptr)
			{
				return;
			}

			if (bullet->player_bullet != bullet2->player_bullet)
			{
				App->entity->KillEntity(bullet);
				App->entity->KillEntity(bullet2);
			}
		}
	}
}

bool EntityManager::Load(const pugi::xml_node& node)
{
	pugi::xml_node entity = node.child("Alive_Entities").first_child();

	while (entity != NULL)
	{
		pending_entities.add(EntityReference(StrToEntityType((char*)entity.attribute("Type").as_string()), entity.attribute("X").as_float(), entity.attribute("Y").as_float(), false));

		entity = entity.next_sibling();
	}

	pugi::xml_node dead_entity = node.child("Dead_Entities").first_child();

	while (dead_entity != NULL)
	{
		pending_entities.add(EntityReference(StrToEntityType((char*)entity.attribute("Type").as_string()), entity.attribute("X").as_float(), entity.attribute("Y").as_float(), true));

		entity = entity.next_sibling();
	}


	load_pending = true;

	return true;
}

bool EntityManager::EntitiesLoad()
{
	load_pending = false;

	for (int i = 0; i < pending_entities.count(); i++)
	{
		BaseEntity* new_entity = CreateEntity(pending_entities[i].type_entity);

		new_entity->entity_pos = pending_entities[i].pos_entity;

		if (pending_entities[i].is_dead)
		{
			App->entity->KillEntity(new_entity);
		}
	}

	pending_entities.clear();

	return true;
}

bool EntityManager::Save(pugi::xml_node& node)
{
	bool ret = true;
	node.append_child("Alive_Entities");
	node.append_child("Dead_Entities");
	for (int i = 0; i < entities_list.count(); i++)
	{
		if (entities_list[i]->entity_type == entityType::ROCKET_TYPE || entities_list[i]->entity_type == entityType::BULLET_TYPE) continue;
		pugi::xml_node entities_node = node.first_child().append_child("Entity_Alive");
		{
			entities_node.append_attribute("Type") = EntityTypeToStr(entities_list[i]->entity_type);
			entities_node.append_attribute("X") = entities_list[i]->entity_pos.x;
			entities_node.append_attribute("Y") = entities_list[i]->entity_pos.y;
			if (entities_list[i]->entity_type == entityType::BULLET_TYPE || entities_list[i]->entity_type == entityType::ROCKET_TYPE) {
				entities_node.append_attribute("Vel_X") = entities_list[i]->entity_current_vel.x;
				entities_node.append_attribute("Vel_Y") = entities_list[i]->entity_current_vel.y;
			}
		}

	}
	for (int i = 0; i < dead_entities.count(); i++)
	{
		if (dead_entities[i]->entity_type == entityType::ROCKET_TYPE || dead_entities[i]->entity_type == entityType::BULLET_TYPE) continue;

		pugi::xml_node dead_entities_node = node.child("Dead_Entities").append_child("Entity_Dead");
		{
			dead_entities_node.append_attribute("Type") = EntityTypeToStr(dead_entities[i]->entity_type);
			dead_entities_node.append_attribute("X") = dead_entities[i]->entity_pos.x;
			dead_entities_node.append_attribute("Y") = dead_entities[i]->entity_pos.y;
		}
	}
	return true;
}

void EntityManager::Activate()
{
	CleanUp();
	active = true;
}

void EntityManager::Desactivate()
{
	CleanUp();
	active = false;
}

BaseEntity* EntityManager::FindEntity(const Collider* col) const
{
	for (int i = 0; i < new_entities.count(); i++)
	{
		if (new_entities.At(i)->data->entity_collider == col)
		{
			return new_entities.At(i)->data;
		}
	}

	for (int i = 0; i < entities_list.count(); i++)
	{
		if (entities_list.At(i)->data->entity_collider == col)
		{
			return entities_list.At(i)->data;
		}
	}

	return nullptr;
}
BaseEntity* EntityManager::FindDeadEntity(const Collider* col) const
{
	for (int i = 0; i < dead_entities.count(); i++)
	{
		if (dead_entities.At(i)->data->entity_collider == col)
		{
			return dead_entities.At(i)->data;
		}
	}

	for (int i = 0; i < dead_entities_not_visible.count(); i++)
	{
		if (dead_entities_not_visible.At(i)->data->entity_collider == col)
		{
			return dead_entities_not_visible.At(i)->data;
		}
	}

	return nullptr;
}
BaseEntity* EntityManager::CreateEntity(entityType entity_type)
{
	BROFILER_CATEGORY("CreateEntity", Profiler::Color::LightGoldenRodYellow);

	BaseEntity* new_entity = nullptr;

	for (int i = 0; i < values_entities.Count(); i++)
	{
		if (values_entities[i]->entity_type == entity_type)
		{
			new_entity = values_entities[i];
		}
	}

	if (new_entity == nullptr)
	{
		return nullptr;
	}

	switch (entity_type)
	{
	case PLAYER_TYPE:
	{
		PlayerEntity* new_player = new PlayerEntity((const PlayerEntity*)new_entity);
		new_player->collider_type = COLLIDER_PLAYER;
		new_entity = new_player;
	}
	break;

	case ENEMY_GROUND_TYPE:
	{
		EnemyGroundEntity* new_enemy_ground = new EnemyGroundEntity((const EnemyGroundEntity*)new_entity);
		new_enemy_ground->collider_type = COLLIDER_ENEMY;
		new_entity = new_enemy_ground;
	}
	break;
	case DRONE_TYPE:
	{
		EnemyDrone* new_enemy_drone = new EnemyDrone((const EnemyDrone*)new_entity);
		new_enemy_drone->collider_type = COLLIDER_ENEMY;
		new_entity = new_enemy_drone;
	}
	break;
	case ENEMY_RPG_TYPE:
	{
		EnemyRPGEntity* new_enemy_rpg = new EnemyRPGEntity((const EnemyRPGEntity*)new_entity);
		new_enemy_rpg->collider_type = COLLIDER_ENEMY;
		new_entity = new_enemy_rpg;
	}

	break;

	case BULLET_TYPE:
	{
		Bullet* bullet = new Bullet((const Bullet*)new_entity);
		bullet->collider_type = COLLIDER_BULLET;
		new_entity = bullet;
	}

	break;

	case ROCKET_TYPE:
	{
		Rocket* rocket = new Rocket((const Rocket*)new_entity);
		rocket->collider_type = COLLIDER_BULLET;
		new_entity = rocket;
	}

	break;

	case AMMO_TYPE:
	{
		Ammo* ammo = new Ammo((const Ammo*)new_entity);
		ammo->collider_type = COLLIDER_AMMO;
		new_entity = ammo;
	}

	break;

	}

	new_entities.add(new_entity);

	new_entity->entity_collider = App->colliders->AddCollider({ 0,0,new_entity->collider_size.x, new_entity->collider_size.y }, new_entity->collider_type, this);



	return new_entity;
}

bool EntityManager::KillEntity(BaseEntity* entity)
{
	bool ret = false;
	for (int i = 0; i < entities_list.count(); i++)
	{
		if (entities_list.At(i)->data == entity)
		{
			entities_list.del(entities_list.At(i));
			ret = true;
			break;
		}
	}

	if (ret)
	{
		if (entity->entity_draw_dead) {
			dead_entities.add(entity);
		}
		else {
			dead_entities_not_visible.add(entity);
		}

		App->colliders->EraseCollider(entity->entity_collider);

		entity->Die(GetEntitySideView(entity));
	}

	return ret;
}

void EntityManager::SetEntityState(entityState new_state, Collider* coll)
{
	BaseEntity* entity = nullptr;

	if (new_state == entityState::ENTITY_DEAD_LEFT || new_state == entityState::ENTITY_DEAD_RIGHT) {
		entity = (BaseEntity*)FindDeadEntity(coll);
	}
	else
	{
		entity = (BaseEntity*)FindEntity(coll);
	}
	if (entity == nullptr) {
		return;
	}
	if (entity->current_state_entity == new_state)
	{
		return;
	}

	if (entity->current_animation != nullptr)
	{
		//RELEASE(entity->current_animation);
	}

	Animation* new_animation = App->animation->GetAnimation(entity->entity_type, new_state);

	if (new_animation != nullptr)
	{
		entity->current_state_entity = new_state;
		entity->current_animation = new_animation;

		if (entity->current_state_entity == ENTITY_JUMP_LEFT || entity->current_state_entity == ENTITY_JUMP_RIGHT)
		{
			entity->current_animation->SetLoop(false);
		}

		entity->current_animation->Reset();
	}
}

entityType EntityManager::StrToEntityType(const char* str) const
{
	if (strcmp(str, "player") == 0) return entityType::PLAYER_TYPE;
	if (strcmp(str, "enemy") == 0) return entityType::ENEMY_GROUND_TYPE;
	if (strcmp(str, "rpg") == 0) return entityType::ENEMY_RPG_TYPE;
	if (strcmp(str, "bullet") == 0) return entityType::BULLET_TYPE;
	if (strcmp(str, "rocket") == 0) return entityType::ROCKET_TYPE;
	if (strcmp(str, "drone") == 0) return entityType::DRONE_TYPE;
	if (strcmp(str, "ammo") == 0) return entityType::AMMO_TYPE;

	return entityType();
}

char* EntityManager::EntityTypeToStr(entityType type) const
{
	switch (type)
	{
	case UNKNOWN:
		return nullptr;
		break;
	case PLAYER_TYPE:
		return("player");
		break;
	case ENEMY_GROUND_TYPE:
		return("enemy");
		break;
	case ENEMY_RPG_TYPE:
		return("rpg");
		break;
	case BULLET_TYPE:
		return("bullet");
		break;
	case ROCKET_TYPE:
		return("rocket");
		break;
	case DRONE_TYPE:
		return("drone");
		break;
	case AMMO_TYPE:
		return("ammo");
		break;
	default:
		return nullptr;
		break;
	}
}

entityState EntityManager::StringToEntityState(const char* str) const
{
	if (strcmp(str, "jump_right") == 0) return entityState::ENTITY_JUMP_RIGHT;
	if (strcmp(str, "run_right") == 0) return entityState::ENTITY_WALK_RIGHT;
	if (strcmp(str, "stand_right") == 0) return entityState::ENTITY_IDLE_RIGHT;

	if (strcmp(str, "jump_left") == 0) return entityState::ENTITY_JUMP_LEFT;
	if (strcmp(str, "run_left") == 0) return entityState::ENTITY_WALK_LEFT;
	if (strcmp(str, "stand_left") == 0) return entityState::ENTITY_IDLE_LEFT;

	if (strcmp(str, "dead_left") == 0) return entityState::ENTITY_DEAD_LEFT;
	if (strcmp(str, "dead_right") == 0) return entityState::ENTITY_DEAD_RIGHT;

	if (strcmp(str, "shoot_right") == 0) return entityState::ENTITY_SHOOTING_RIGHT;
	if (strcmp(str, "shoot_left") == 0) return entityState::ENTITY_SHOOTING_LEFT;

	return entityState::NONE_STATE;
}

entityType EntityManager::TileIdToEntityType(int i) const
{
	switch (i) {
	case 13:
		return entityType::AMMO_TYPE;
	case 12:
		return entityType::PLAYER_TYPE;
	case 7:
		return entityType::ENEMY_GROUND_TYPE;
	case 10:
		return entityType::ENEMY_RPG_TYPE;
	case 11:
		return entityType::DRONE_TYPE;
	}
	return entityType::UNKNOWN;
}

bool EntityManager::SpawnEntities(p2DynArray<std::pair<entityType, iPoint>>& list)
{
	DeleteAllEntities();

	if (load_pending)
	{
		EntitiesLoad();
	}
	else
	{
		for (int i = 0; i < list.Count(); i++)
		{
			BaseEntity* new_entity = CreateEntity(list[i].first);

			if (new_entity == nullptr)
			{
				return false;
			}

			new_entity->entity_pos = fPoint(list[i].second.x, list[i].second.y);
		}
	}
	return true;
}

bool EntityManager::DeleteAllEntities()
{
	for (int i = 0; i < new_entities.count(); i++) {
		RELEASE(new_entities[i]);
	}
	for (int i = 0; i < entities_list.count(); i++) {
		RELEASE(entities_list[i]);
	}
	for (int i = 0; i < dead_entities.count(); i++) {
		RELEASE(dead_entities[i]);
	}
	for (int i = 0; i < dead_entities_not_visible.count(); i++) {
		RELEASE(dead_entities_not_visible[i]);
	}
	new_entities.clear();
	entities_list.clear();
	dead_entities.clear();
	dead_entities_not_visible.clear();
	return true;
}


PlayerEntity* EntityManager::GetPlayer() const
{
	for (int i = 0; i < entities_list.count(); i++)
	{
		if (entities_list.At(i)->data->entity_type == PLAYER_TYPE)
		{
			return (PlayerEntity*)entities_list.At(i)->data;
		}
	}
	return nullptr;
}

entitySide EntityManager::GetEntitySideView(BaseEntity* entity)
{
	switch (entity->current_state_entity)
	{
	case entityState::ENTITY_IDLE_LEFT:
		return entitySide::LEFT;
	case entityState::ENTITY_WALK_LEFT:
		return entitySide::LEFT;
	case entityState::ENTITY_JUMP_LEFT:
		return entitySide::LEFT;
	case entityState::ENTITY_SHOOTING_LEFT:
		return entitySide::LEFT;

	case entityState::ENTITY_IDLE_RIGHT:
		return entitySide::RIGHT;
	case entityState::ENTITY_WALK_RIGHT:
		return entitySide::RIGHT;
	case entityState::ENTITY_JUMP_RIGHT:
		return entitySide::RIGHT;
	case entityState::ENTITY_SHOOTING_RIGHT:
		return entitySide::RIGHT;

	default:
		return entitySide::RIGHT;
	}

	return entitySide::RIGHT;
}
