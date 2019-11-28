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

BaseEntity::BaseEntity(const BaseEntity* copy): entity_type(copy->entity_type), current_state_entity(copy->current_state_entity), collider_size(copy->collider_size), gravity(copy->gravity), entity_vel(copy->entity_vel), entity_draw_dead(copy->entity_draw_dead)
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
	UpdatePosition();
	AnimationUpdate();
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

void BaseEntity::IsDead()
{
	if (entity_collider != nullptr) {
		App->colliders->EraseCollider(entity_collider);
	}
}

void BaseEntity::UpdatePosition()
{
	entity_current_vel.x = entity_current_vel.x > 0 ? entity_current_vel.x > entity_vel.x ? entity_vel.x : entity_current_vel.x : entity_current_vel.x < -entity_vel.x ? -entity_vel.x : entity_current_vel.x;
	entity_current_vel.y = entity_current_vel.y > 0 ? entity_current_vel.y > entity_vel.y ? entity_vel.y : entity_current_vel.y : entity_current_vel.y < -entity_vel.y ? -entity_vel.y : entity_current_vel.y;

	entity_pos.x += entity_current_vel.x * App->Getdt();
	entity_pos.y += entity_current_vel.y * App->Getdt();

	entity_current_vel.y -= gravity * App->Getdt();

	entity_collider->SetPos(entity_pos.x, entity_pos.y);
}

bool BaseEntity::AnimationUpdate()
{
	if (abs(entity_current_vel.y) > 20) {
		if (entity_current_vel.x > 0) {
			App->entity->SetEntityState(ENTITY_JUMP_LEFT, entity_collider);
		}
		else{
			App->entity->SetEntityState(ENTITY_JUMP_RIGHT, entity_collider);
		}
	}
	else {

		if (entity_current_vel.x >= 0) {
			App->entity->SetEntityState(ENTITY_WALK_LEFT, entity_collider);
		}
		else {
			App->entity->SetEntityState(ENTITY_WALK_RIGHT, entity_collider);
		}
	}
	return true;
}

void BaseEntity::Die(entitySide side)
{
	if (side == entitySide::LEFT) {
		App->entity->SetEntityState(ENTITY_DEAD_LEFT, entity_collider);
	}
	else {
		App->entity->SetEntityState(ENTITY_DEAD_RIGHT, entity_collider);
	}
}

