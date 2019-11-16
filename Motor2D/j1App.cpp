#include "p2Defs.h"
#include "p2Log.h"
#include "j1Colliders.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1FadeToBlack.h"
#include "j1Animation.h"

// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;

	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	audio = new j1Audio();
	scene = new j1Scene();
	map = new j1Map();
	colliders = new j1Colliders();
	player = new j1Player();
	fade = new j1FadeToBlack();
	animation = new j1Animation();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(animation);
	AddModule(scene);
	AddModule(map);
	AddModule(colliders);
	AddModule(player);
	AddModule(fade);
	// render last to swap buffer
	AddModule(render);

	LOG("Constructor time: %d ms", frame_time.Read());
}

// Destructor
j1App::~j1App()
{
	// release modules
	p2List_item<j1Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();
}

void j1App::AddModule(j1Module* module)
{
	module->Init();
	modules.add(module);
}

void j1App::WantToSave()
{
	want_to_save = true;
}

void j1App::WantToLoad()
{
	want_to_load = true;
}

float j1App::Getdt() const
{
	return dt;
}

// Called before render is available
bool j1App::Awake()
{
	
	pugi::xml_parse_result result = config_file.load_file("../Motor2D/Config.xml");
	if (result == false ) {
		LOG(result.description());
	}
	main_node = config_file.first_child();
	if (main_node == NULL) {
		LOG("main node not found");
	}
	bool ret = true;

	p2List_item<j1Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		pugi::xml_node module_node = main_node.child(item->data->name.GetString());
		if (strcmp(module_node.name(), "window") == 0){
			capped_frames = module_node.child("capped_frames").attribute("frames").as_int();
		}

		if (item->data->Awake(module_node) == false) {//if the module is not null and has a node in config file we call awake with the node data.
			LOG("Error on %s Awake", item->data->name.GetString());
			break;
		}
		item = item->next; 
	}

	return ret;
}

// Called before the first frame
bool j1App::Start()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}
	frame_time.Start();
	return ret;
}

// Called each loop iteration
bool j1App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
void j1App::PrepareUpdate()
{
	dt = frame_time.ReadSec();
	if (dt > 0.2f) {
		dt = 0.0f;
	}
	last_frame_ms = dt * 1000;
	frame_time.Start();

	frame_count += 1;
	curr_frames += 1;

	if (timer_psec.Read() >= 1000) {
		frames_on_last_update = curr_frames;
		curr_frames = 0;
		timer_psec.Start();
	}
}

// ---------------------------------------------
void j1App::FinishUpdate()
{
	if (want_to_load == true) {
		Load();
		want_to_load = false;
	}
	if (want_to_save == true) {
		Save();
		want_to_save = false;
	}

	avg_fps = (float)frame_count / fps_timer.ReadSec();
	
	if (is_fps_capped) {
		float delay = (1000 / capped_frames) - frame_time.Read();
		
		if (delay > 0 && delay < 9000) {
			SDL_Delay(delay);
		}
	}
	
	if (is_fps_capped) {
		cap_string = "On";
	}
	else {
		cap_string = "Off";
	}
	if (App->render->vsync) {
		vsync_string = "On";
	}
	else {
		vsync_string = "Off";
	}
	static char title[256];
	sprintf_s(title, 256, "Avg.FPS: %.2f MS of the last frame: %02u FPS: %i Frames Capped: %s Vsync: %s ",
		avg_fps, last_frame_ms, frames_on_last_update,cap_string.GetString(), vsync_string.GetString());

	App->win->SetTitle(title);

}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

void j1App::Load()
{
	pugi::xml_document load_file;

	pugi::xml_parse_result result = load_file.load_file("saved_game.xml");
	if (result == false) {
		LOG(result.description());
	}

	pugi::xml_node load_main = load_file.first_child();

	p2List_item<j1Module*>* item;
	item = modules.start;

	while (item != NULL)
	{
		pugi::xml_node node = load_main.child(item->data->name.GetString());
		if (node == NULL) {
			LOG("Error loading node");
			
		}
		else {
			item->data->Load(node);
		}
		item = item->next;

	}
}

void j1App::Save()
{

	pugi::xml_document save_file;
	p2List_item<j1Module*>* item;

	pugi::xml_node save_main = save_file.append_child("save");
	item = modules.start;

	while (item != NULL)
	{
		pugi::xml_node new_node = save_main.append_child(item->data->name.GetString());
		item->data->Save(new_node);
		item = item->next;

	}
	save_file.save_file("saved_game.xml");
}


