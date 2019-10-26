#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "j1Module.h"
#include "PugiXml\src\pugixml.hpp"

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Scene;
class j1Map;
class j1Colliders;
class j1Player;
class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);
	void WantToSave();
	void WantToLoad();
	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;

private:

	void Load();

	void Save();
	// Call modules before each loop iteration
	void PrepareUpdate();
	
	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

public:

	// Modules
	j1Window*			win = nullptr;
	j1Input*			input = nullptr;
	j1Render*			render = nullptr;
	j1Textures*			tex = nullptr;
	j1Audio*			audio = nullptr;
	j1Scene*			scene = nullptr;
	j1Map*				map = nullptr;
	j1Colliders*		colliders = nullptr;
	j1Player*			player = nullptr;

private:
	bool want_to_save = false;
	bool want_to_load = false;
	p2List<j1Module*>	modules;
	uint				frames;
	float				dt;

	
	pugi::xml_document config_file;
	pugi::xml_node main_node;
	int					argc;
	char**				args;
};

extern j1App* App; 

#endif