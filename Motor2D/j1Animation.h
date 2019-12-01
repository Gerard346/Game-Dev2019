#ifndef __j1ANIMATION_H__
#define __j1ANIMATION_H__
#include "PugiXml/src/pugixml.hpp"
#include "p2SString.h"
#include "j1Module.h"
#include "p2Point.h"
#include "SDL/include/SDL_rect.h"
#include "p2DynArray.h"
#include "j1Timer.h"
#include "j1Textures.h"
#include "BaseEntity.h"
class j1App;

class Sprite
{
public:
	Sprite();
	Sprite(const SDL_Rect& frame, const iPoint& pivot);
	~Sprite();

private:

	SDL_Rect		frame = { 0,0,0,0 };
	iPoint			pivot = { 0,0 };

public:

	//Functionality
	//Get Methods
	const SDL_Rect*		GetFrame()const;
	int					GetXpivot()const;
	int					GetYpivot()const;

};

class Animation
{
public:

	Animation();
	Animation(const Animation* cpy);
	~Animation();

protected:

	//Vector that storage the frames rect & pivot
	p2DynArray<Sprite*>		sprites;
	//Pointer to the animation texture
	SDL_Texture* texture = nullptr;
	//Id of the animation enum type
	entityState			animation_type = entityState::ENTITY_IDLE_RIGHT;
	//Current frame calculated by the timer
	float					current_frame = 0.0f;
	bool					loop = true;
	int						loops = 0;
	//Animation update rate
	uint					speed = 2500;
	j1Timer					timer;

public:

	//Functionality -------------------
	//Called before clean ---
	void	ReleaseTexture();

	//Set Methods -----------
	void	SetTexture(const SDL_Texture* tex);
	void	SetLoop(bool loop_state);
	void	SetCurrentFrame(uint curr);
	void	SetSpeed(uint new_speed);
	void	SetId(entityState id);

	//Get Methods -----------
	SDL_Texture*					GetTexture()const;
	bool							GetLoop()const;
	uint							GetCurrentFrame()const;
	uint							GetSpeed()const;
	const Sprite*					GetCurrentSprite();
	entityState					GetId()const;
	uint							GetFrameNum()const;

	//Check if the animation is end
	bool IsEnd();
	//Reset the animation frame
	void Reset();

	//Add New sprite
	void AddSprite(const SDL_Rect& rect, const iPoint& point);

};

struct EntityAnimations
{
	entityType entityType = UNKNOWN;
	p2DynArray<Animation*> animations;
};

class j1Animation:public j1Module
{
public:

	j1Animation();

	void Init()
	{
		active = true;
	}

	// Called before render is available
	bool Awake(const pugi::xml_node& node)override;
	
	// Called before the first frame
	bool Start()override;
	
	// Called before quitting
	bool CleanUp()override;
	
private:

	p2SString animation_path = "";

	p2DynArray<EntityAnimations*> animations;

	pugi::xml_document animation_doc;

public:

	Animation* GetAnimation(entityType, entityState);

};

#endif // __j1ANIMATION_H__