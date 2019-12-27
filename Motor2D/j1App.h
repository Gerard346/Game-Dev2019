#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "j1Module.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Timer.h"
#include "j1PerfTimer.h"
#include "Brofiler/Brofiler.h"

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
class j1FadeToBlack;
class j1Animation;
class EntityManager;
class j1Pathfinding;
class j1Gui;
class j1Fonts;
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
	void WantToSaveCheckpoints();
	void WantToLoad();
	bool IsLoading();
	void WantToLoadCheckpoints();
	float Getdt() const;
	uint GetCappedFrames() const;
	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;

	void SetCappedFrames();

private:

	void Load();
	void LoadCheckPoints();

	void Save();

	void SaveCheckPoints();
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
	j1Animation*		animation = nullptr;
	j1Scene*			scene = nullptr;
	j1Map*				map = nullptr;
	j1Colliders*		colliders = nullptr;
	j1Player*			player = nullptr;
	j1FadeToBlack*		fade = nullptr;
	EntityManager*		entity = nullptr;
	j1Pathfinding*		path = nullptr;
	j1Gui*				gui = nullptr;
	j1Fonts*				font = nullptr;
private:

	bool want_to_save = false;
	bool want_to_save_checkpoints = true;

	bool want_to_load = false;
	bool want_to_load_checkpoints = false;

	bool is_fps_capped = true;
	
	p2SString cap_string = "";
	p2SString vsync_string = "";

	p2List<j1Module*>	modules;
	uint				frames;
	float				dt;

	pugi::xml_document config_file;
	pugi::xml_node main_node;
	int					argc;
	char**				args;
	uint					capped_frames = 0;
	j1Timer				time_since_startup;
	j1Timer				frame_time;
	j1Timer				fps_timer;
	j1Timer				timer_psec;
	uint32_t			curr_frames = NULL;
	float				avg_fps = 0.0f;
	float				seconds_since_startup = 0.0f;
	uint64_t			frame_count = 0.0f;
	uint32_t			last_frame_ms = 0.0f;
	uint32_t			frames_on_last_update = 0.0f;
};

extern j1App* App; 

#endif