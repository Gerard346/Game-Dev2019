#include "EntityManager.h"
#include "Brofiler/Brofiler.h"

EntityManager::EntityManager()
{
	name = ("entityManager");
}

EntityManager::~EntityManager()
{

}

bool EntityManager::Awake(pugi::xml_node&)
{
	BROFILER_CATEGORY("AwakeEntityManager", Profiler::Color::LightGoldenRodYellow);
	return true;
}

bool EntityManager::Start()
{
	return true;
}

bool EntityManager::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdatEntityManager", Profiler::Color::LightGoldenRodYellow);
	return true;
}

bool EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("UpdatEntityManager", Profiler::Color::LightGoldenRodYellow);
	return true;
}

bool EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdateEntityManager", Profiler::Color::LightGoldenRodYellow);
	return true;
}

bool EntityManager::CleanUp()
{
	return true;
}

void EntityManager::OnCollision(Collider* c1, Collider* c2)
{

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
	return nullptr;
}

BaseEntity* EntityManager::CreateEntity(entityType)
{
	return nullptr;
}

bool EntityManager::KillEntity(BaseEntity*)
{
	return true;
}
