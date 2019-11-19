#include "BaseEntity.h"
#include "EntityManager.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Colliders.h"

BaseEntity::BaseEntity(entityType, char*)
{
}

BaseEntity::~BaseEntity()
{
	App->colliders->EraseCollider(entity_collider);
}

bool BaseEntity::Awake(pugi::xml_node&)
{
	BROFILER_CATEGORY("AwakeBaseEntity", Profiler::Color::Black);
	return true;
}

bool BaseEntity::Start()
{
	BROFILER_CATEGORY("StartBaseEntity", Profiler::Color::Black);
	return true;
}

bool BaseEntity::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdateBaseEntity", Profiler::Color::Black);
	return true;
}

bool BaseEntity::Update()
{
	BROFILER_CATEGORY("UpdateBaseEntity", Profiler::Color::Black);
	return true;
}

bool BaseEntity::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdateBaseEntity", Profiler::Color::Black);
	return true;
}

bool BaseEntity::CleanUp()
{
	return true;
}

bool BaseEntity::Draw()
{
	return true;
}

void BaseEntity::IsDead()
{
	App->colliders->EraseCollider(entity_collider);
}

void BaseEntity::SetEntityState(entityState new_state)
{
}
