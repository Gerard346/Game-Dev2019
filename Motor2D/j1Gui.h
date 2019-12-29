#ifndef __j1GUI_H__
#define __j1GUI_H__
#include "j1Module.h"
#include "p2List.h"

#define CURSOR_WIDTH 2

struct GUIElement;
struct SDL_Texture;

enum TypeGUI;

enum TypeInput {
	MOUSE_LEFT_DOWN,
	MOUSE_LEFT_UP,
	MOUSE_LEFT_REPEAT,
	MOUSE_RIGHT_DOWN,
	MOUSE_RIGHT_UP,
	MOUSE_RIGHT_REPEAT,
	MOUSE_IN,
	MOUSE_OUT,
	SCROLL_MOVE
};

class j1Gui : public j1Module
{
public:

	j1Gui();

	virtual ~j1Gui();

	bool Awake(const pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	GUIElement* GenerateElemGUI(TypeGUI type);

	void SetSceneGUI(GUIElement* elem);

	SDL_Texture* GetAtlas() const;
	int GetTopLayer();
	int CalculateLayer();

	SDL_Texture* GetTexture(int id)const;
	int AddTexture(SDL_Texture* new_tex);

	void SetDebug();

	void SetConsoleGUI(GUIElement* elem);
private:
	GUIElement* gui_scene = nullptr;
	GUIElement* gui_console = nullptr;

	bool debug = false;

	SDL_Texture* atlas = nullptr;
	p2SString atlas_file_name;

	int top_layer = 0;
	p2List<SDL_Texture*> textures;

	int CalculateTopLayer(const GUIElement* element, int& layer);

public:
	SDL_Color LIGHT_GREEN = { 102, 255, 102, 75 };
	SDL_Color BROWN = { 153, 76, 0, 75 };
	SDL_Color YELLOW = { 255,255,0,75 };
	SDL_Color RED = { 255,0,0,75 };
	SDL_Color MAGENTA = { 204,0,204,75 };
	SDL_Color WHITE = { 255,255,255,100 };
	SDL_Color BLACK = { 0,0,0,100 };
	SDL_Color UI_YELLOW = { 255,255,0,100 };
	SDL_Color UI_BROWN = { 255, 255, 255, 255 };
	SDL_Color BLUE = { 0, 51, 102, 255 };
	SDL_Color BLUE_LIGHT = { 0, 102, 153, 255 };


};

#endif