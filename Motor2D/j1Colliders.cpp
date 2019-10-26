#include "j1App.h"
#include "j1Input.h"
#include "j1Colliders.h"
#include "j1Render.h"

#include "p2Log.h"



j1Colliders::j1Colliders()
{
	name = ("colliders");
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALL][COLLIDER_FINISH] = false;
	matrix[COLLIDER_WALL][COLLIDER_DEAD] = false;

	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_DEAD] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_FINISH] = true;


	matrix[COLLIDER_DEAD][COLLIDER_WALL] = false;
	matrix[COLLIDER_DEAD][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_DEAD][COLLIDER_FINISH] = false;
	matrix[COLLIDER_DEAD][COLLIDER_DEAD] = false;

	matrix[COLLIDER_FINISH][COLLIDER_WALL] = false;
	matrix[COLLIDER_FINISH][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_FINISH][COLLIDER_FINISH] = false;
	matrix[COLLIDER_FINISH][COLLIDER_DEAD] = false;
}

j1Colliders::~j1Colliders()
{
}


bool j1Colliders::Update(float)
{
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];
		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);

			}
		}
	}

	DebugMode();

	return true;

}


bool j1Colliders::PostUpdate()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == NULL)
		{
			continue;
		}
		if (colliders[i]->to_delete == true) {
			RELEASE(colliders[i]);
			int b, c;
			for (b = i, c = i + 1; c < MAX_COLLIDERS; c++, b++) {
				colliders[b] = colliders[c];
			}
		}
	}
	return true;
}
bool j1Colliders::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}
bool j1Colliders::EraseCollider(Collider * collider)
{
	if (collider != nullptr)
	{
		// we still search for it in case we received a dangling pointer
		for (uint i = 0; i < MAX_COLLIDERS; ++i)
		{
			if (colliders[i] == collider)
			{
				collider->to_delete = true;

				break;
			}
		}
	}


	return false;
}
bool j1Colliders::EraseAllColliders() {

	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		if (colliders[i] == NULL) {
			continue;
		}
		if (colliders[i]->type == COLLIDER_PLAYER) {
			continue;
		}
		else {
			RELEASE(colliders[i]);
		}
	}

	return true;
}
bool Collider::CheckCollision(const SDL_Rect & r) const
{
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}
bool Collider::Update()
{
	return true;
}
Collider* j1Colliders::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module * callback)
{

	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}

COLLIDER_TYPE j1Colliders::TileIDToColliderTile(int tile_id) const
{
	switch (tile_id) {

	case 1:

		return COLLIDER_WALL;

	case 2:

		return COLLIDER_DEAD;

	case 3:

		return COLLIDER_START;

	case 4:

		return COLLIDER_FINISH;

	}

}

void j1Colliders::Desactivate()
{
	CleanUp();
	active = false;
}

void j1Colliders::Activate()
{
	active = true;
}


void j1Colliders::DebugMode()
{
	//Change Key for delivery
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN) {
		debug = !debug;
	}

	if (debug == false)
		return;

	Uint8 alpha = 255;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_START: // red
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 255, alpha);
			break;
		case COLLIDER_DEAD: // red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_WALL: // Green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_PLAYER: // blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;

		case COLLIDER_FINISH: // magenta
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 255, alpha);
			break;

		}
	}

}

bool j1Colliders::Load(pugi::xml_node&)
{
	return true;
}

bool j1Colliders::Save(pugi::xml_node&) const
{
	return true;
}

