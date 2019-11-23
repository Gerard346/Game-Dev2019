#include "BaseEntity.h"
#include "EntityManager.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Colliders.h"
#include "j1Animation.h"
#include "j1Render.h"

BaseEntity::BaseEntity()
{
}

BaseEntity::BaseEntity(const BaseEntity* copy): entity_type(copy->entity_type), current_state_entity(copy->current_state_entity), collider_size(copy->collider_size), gravity(copy->gravity), entity_vel(copy->entity_vel)
{

}

BaseEntity::~BaseEntity()
{
	if (entity_collider != nullptr) {
		App->colliders->EraseCollider(entity_collider);
	}
}

bool BaseEntity::Update(float dt)
{
	BROFILER_CATEGORY("UpdateBaseEntity", Profiler::Color::Black);
	return true;
}

bool BaseEntity::CleanUp()
{
	return true;
}

bool BaseEntity::Draw()
{
	animation_index += 0.01f;// dt * current_animation->GetSpeed();
	current_animation->SetCurrentFrame((uint)animation_index);

	App->render->Blit(current_animation->GetTexture(), entity_pos.x, entity_pos.y, current_animation->GetCurrentSprite()->GetFrame());

	if (((uint)animation_index) > current_animation->GetFrameNum() && current_animation->GetLoop())
	{
		animation_index = 0.0f;
	}

	return true;
}

void BaseEntity::HandleInput(float dt)
{
}

void BaseEntity::IsDead()
{
	if (entity_collider != nullptr) {
		App->colliders->EraseCollider(entity_collider);
	}
}

