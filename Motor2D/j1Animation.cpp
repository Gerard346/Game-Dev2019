#include "j1Animation.h"
#include "j1App.h"
#include "j1Textures.h"
/*
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
	App->tex->UnLoad(texture);
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

void Animation::SetId(ANIMATION_TYPE id)
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
	if (current_frame == -1)return &sprites[sprites.Count() - 1];

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
			return &sprites[sprites.Count() - 1];
		}
	}
	return &sprites[(int)current_frame];
}

ANIMATION_TYPE Animation::GetId() const
{
	return animation_type;
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
	sprites.PushBack(Sprite(rect, point));
}

j1Animation::j1Animation()
{
	name = "animation";
}

bool j1Animation::Awake(const pugi::xml_node& node)
{


	return false;
}

Animation* j1Animation::GetAnimation(ENTITY_TYPE entity_type, ANIMATION_TYPE animation_type)
{
	Animation* target_animation = nullptr;

	for (int i = 0; i < animations.Count(); i++)
	{
		if (animations[i].entity_type == entity_type)
		{
			for (int k = 0; k < animations[i].animations.Count(); k++)
			{
				if (animations[i].animations[k].GetId() == animation_type)
				{
					target_animation = &animations[i].animations[k];
					break;

				}
			}
			break;
		}
	}

	return target_animation;
}
*/