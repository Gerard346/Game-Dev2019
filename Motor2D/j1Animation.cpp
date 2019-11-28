#include "j1Animation.h"
#include "j1App.h"
#include "p2Log.h"
#include "EntityManager.h"
Sprite::Sprite()
{
}

Sprite::Sprite(const SDL_Rect& frame, const iPoint& pivot) : frame(frame), pivot(pivot)
{
	
}

Sprite::~Sprite()
{
}

//Functionality =============
const SDL_Rect* Sprite::GetFrame() const
{
	return &frame;
}
int Sprite::GetXpivot() const
{
	return pivot.x;
}
int Sprite::GetYpivot() const
{
	return pivot.y;
}

Animation::Animation()
{
}

Animation::~Animation()
{
	sprites.Clear();
	texture = nullptr;
}

void Animation::ReleaseTexture()
{
	//App->tex->UnLoad(texture);
}

void Animation::SetTexture(const SDL_Texture* tex)
{
	texture = (SDL_Texture*)tex;
}

void Animation::SetLoop(bool loop_state)
{
	loop = loop_state;
}

void Animation::SetCurrentFrame(uint curr)
{
	if (curr < sprites.Count())	current_frame = curr;
	else current_frame = sprites.Count() - 1;
}

void Animation::SetSpeed(uint new_speed)
{
	speed = new_speed;
}

void Animation::SetId(entityState id)
{
	animation_type = id;
}

SDL_Texture* Animation::GetTexture() const
{
	return texture;
}

bool Animation::GetLoop() const
{
	return loop;
}

uint Animation::GetCurrentFrame() const
{
	return current_frame;
}

uint Animation::GetSpeed() const
{
	return speed;
}

const Sprite* Animation::GetCurrentSprite()
{
	if (current_frame == -1)return sprites[sprites.Count() - 1];

	//if (App->paused)return &sprites[(int)current_frame];

	current_frame = (float)floor(timer.Read() / speed);
	if (current_frame >= sprites.Count())
	{
		if (loop)
		{
			loops++;
			current_frame = 0;
			timer.Start();
		}
		else
		{
			loops = 0;
			current_frame = -1;
			return sprites[sprites.Count() - 1];
		}
	}
	return sprites[(int)current_frame];
}

entityState Animation::GetId() const
{
	return animation_type;
}

uint Animation::GetFrameNum() const
{
	return sprites.Count();
}

bool Animation::IsEnd()
{
	return (current_frame == -1 && !loop);
}

void Animation::Reset()
{
	timer.Start();
	current_frame = 0;
}

void Animation::AddSprite(const SDL_Rect & rect, const iPoint & point)
{
	sprites.PushBack(new Sprite(rect, point));
}

j1Animation::j1Animation()
{
	name = "animation";
}

bool j1Animation::Awake(const pugi::xml_node& node)
{
	if (node != nullptr) {
		animation_path = node.child("xml_source").child_value();

		pugi::xml_parse_result result = animation_doc.load_file(animation_path.GetString());

		if (result == false) {
			LOG(result.description());
		}
	}

	else {
		LOG("Error in Awake %s", name);
	}
	LOG("Loading Animations");

	return true;
}

bool j1Animation::Start()
{
	pugi::xml_node main_node = animation_doc.first_child();
	if (main_node == NULL) {
		LOG("main node from animation not found");
	}

	pugi::xml_node animation_block_node = main_node.first_child();

	while (animation_block_node != NULL)
	{
		pugi::xml_node block_info = animation_block_node.first_child();
		entityType entityType = App->entity->StrToEntityType(block_info.attribute("type").as_string());
		SDL_Texture* animation_tex = App->tex->Load((const char*)block_info.attribute("image_path").as_string());

		EntityAnimations* new_entity_animation = new EntityAnimations();
		new_entity_animation->entityType = entityType;

		pugi::xml_node animation = block_info.next_sibling();

		while (animation != NULL)
		{
			entityState animation_type = App->entity->StringToEntityState(animation.name());
			Animation* new_animation = new Animation();
			new_animation->SetTexture(animation_tex);
			new_animation->SetCurrentFrame(0);
			new_animation->SetSpeed(150);
			new_animation->SetLoop(animation.attribute("loop").as_bool(true));
			new_animation->SetId(animation_type);
			pugi::xml_node sprite = animation.first_child();

			while (sprite != NULL)
			{
				SDL_Rect sprite_rect;
				sprite_rect.x = sprite.attribute("x").as_int();
				sprite_rect.y = sprite.attribute("y").as_int();
				sprite_rect.w = sprite.attribute("w").as_int();
				sprite_rect.h = sprite.attribute("h").as_int();

				new_animation->AddSprite(sprite_rect, { 0,0 });

				sprite = sprite.next_sibling();
			}

			new_entity_animation->animations.PushBack(new_animation);

			animation = animation.next_sibling();
		}

		animations.PushBack(new_entity_animation);

		animation_block_node = animation_block_node.next_sibling();
	}
	return true;
}

bool j1Animation::CleanUp()
{
	return true;
}

Animation* j1Animation::GetAnimation(entityType entityType, entityState animation_type)
{
	Animation* target_animation = nullptr;

	for (int i = 0; i < animations.Count(); i++)
	{
		if (animations[i]->entityType == entityType)
		{
			for (int k = 0; k < animations[i]->animations.Count(); k++)
			{
				if (animations[i]->animations[k]->GetId() == animation_type)
				{
					target_animation = animations[i]->animations[k];
					break;
				}
			}
			break;
		}
	}

	return target_animation;
}
