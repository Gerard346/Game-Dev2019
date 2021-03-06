// ----------------------------------------------------
// j1Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __j1MODULE_H__
#define __j1MODULE_H__
#include "PugiXml/src/pugixml.hpp"
#include "p2SString.h"

class j1App;
struct Collider; 

enum TypeInput;
class GUIElement;

class j1Module
{
public:

	j1Module() : active(false)
	{}

	void Init()
	{
		active = true;
	}

	// Called before render is available
	virtual bool Awake(const pugi::xml_node& node)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}
	bool colision = false;
	virtual void OnCollision(Collider*, Collider*) {
		colision = true;
	}
	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}
	virtual bool Save(pugi::xml_node &node) {
		return true;
	}
	virtual bool Load(const pugi::xml_node &node) {
		return true;
	}

	virtual void HandleInput(GUIElement* input, TypeInput type_input)
	{
	}

public:
	p2SString	name;
	bool		active;
	virtual void Activate();
	virtual void Desactivate();
	bool GetActive() const;
};

#endif // __j1MODULE_H__