#include "EntityManager.h"
#include "Brofiler/Brofiler.h"
#include "j1App.h"
#include "PlayerEntity.h"
#include "j1Module.h"
#include "j1Player.h"
#include "j1Animation.h"
#include "p2Log.h"
#include "SDL/include/SDL_rect.h"
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
				entity = new PlayerEntity();
				
				entity->entity_type = type;
				
				entity->gravity = entity_node.child("gravity").attribute("g").as_float();
				
				entity->entity_vel = fPoint(entity_node.child("max_speed").attribute("x").as_float(), entity_node.child("max_speed").attribute("y").as_float());

				entity->collider_size = iPoint(entity_node.child("collider_size").attribute("x").as_int(), entity_node.child("collider_size").attribute("y").as_int());

				break;
			}
			
			values_entities.PushBack(entity);

			entity_node = entity_node.next_sibling();
		}
	}

	else {
		LOG("Error in Awake %s", name);
	}

	return true;
}

bool EntityManager::Start()
{
	return true;
}

bool EntityManager::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdatEntityManager", Profiler::Color::LightGoldenRodYellow);
	
	while (new_entities.count() > 0) {
		for (int i = 0; i < new_entities.count(); i++) {
			BaseEntity* new_entity = new_entities.At(i)->data;
			entities_list.add(new_entity);
			new_entity->Start();
		}
		new_entities.clear();
	}

	for (int i = 0; i < entities_list.count(); i++) {
		entities_list.At(i)->data->PreUpdate();
	}

	return true;
}

bool EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("UpdatEntityManager", Profiler::Color::LightGoldenRodYellow);
	
	for (int i = 0; i < entities_list.count(); i++) {
		entities_list.At(i)->data->Update(dt);
	}

	return true;
}

bool EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdateEntityManager", Profiler::Color::LightGoldenRodYellow);

	for (int i = 0; i < entities_list.count(); i++) {
		entities_list.At(i)->data->PostUpdate();
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

	entities_list.clear();

	return true;
}

void EntityManager::OnCollision(Collider* coll, Collider* coll2)
{
	if (coll->type == COLLIDER_PLAYER && coll2->type == COLLIDER_WALL) {
		PlayerEntity* player_entity = (PlayerEntity*)FindEntity(coll);
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


	if (coll->type == COLLIDER_PLAYER && coll2->type == COLLIDER_DEAD) {
		
		PlayerEntity* player_entity = (PlayerEntity*)FindEntity(coll);
		
		if (player_entity->current_state_entity != entityState::ENTITY_DEAD && App->player->IsGod() == false) 
		{
			player_entity->entity_current_vel.x = 0.0f;
			player_entity->entity_current_vel.y = 0.0f;
			App->player->PlayerDies();
		}
	}
}

bool EntityManager::Load(pugi::xml_node&)
{
	return true;
}

bool EntityManager::Save(pugi::xml_node&) const
{
	return true;
}

BaseEntity* EntityManager::FindEntity(const Collider* col) const
{
	for (int i = 0; i < entities_list.count(); i++) {
		if (entities_list.At(i)->data->entity_collider == col) {
			return entities_list.At(i)->data;
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
		PlayerEntity* new_player = new PlayerEntity((const PlayerEntity*)new_entity);
		new_entity = new_player;
		break;
	}

	new_entity->entity_collider = App->colliders->AddCollider({ 0,0,new_entity->collider_size.x, new_entity->collider_size.y }, new_entity->collider_type, this);

	entities_list.add(new_entity);

	SetEntityState(ENTITY_IDLE_RIGHT, new_entity->entity_collider);

	return new_entity;
}

bool EntityManager::KillEntity(BaseEntity*)
{
	return true;
}

void EntityManager::SetEntityState(entityState new_state, Collider* coll)
{
	BaseEntity* entity = (BaseEntity*)FindEntity(coll);

	if (entity->current_state_entity == new_state)return;

	entity->current_state_entity = new_state;
	entity->current_animation = App->animation->GetAnimation(entity->entity_type, entity->current_state_entity);

	if (entity->current_state_entity == ENTITY_JUMP_LEFT || entity->current_state_entity == ENTITY_JUMP_RIGHT)
	{
		entity->current_animation->SetLoop(false);
	}

	entity->current_animation->Reset();
}

entityType EntityManager::StrToEntityType(const char* str) const
{
	if (strcmp(str, "player") == 0) return entityType::PLAYER_TYPE;
	return entityType();
}

entityState EntityManager::StringToEntityState(const char* str) const
{
	if (strcmp(str, "jump_right") == 0) return entityState::ENTITY_JUMP_RIGHT;
	if (strcmp(str, "run_right") == 0) return entityState::ENTITY_WALK_RIGHT;
	if (strcmp(str, "stand_right") == 0) return entityState::ENTITY_IDLE_RIGHT;

	if (strcmp(str, "jump_left") == 0) return entityState::ENTITY_JUMP_LEFT;
	if (strcmp(str, "run_left") == 0) return entityState::ENTITY_WALK_LEFT;
	if (strcmp(str, "stand_left") == 0) return entityState::ENTITY_IDLE_LEFT;

	return entityState::NONE_STATE;
}

bool EntityManager::SpawnEntities(p2DynArray<std::pair<entityType, iPoint>>& list)
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

