#ifndef _j1_CONSOLE_H_
#define _j1_CONSOLE_H_

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "p2List.h"

struct GUI_Image;
struct GUI_String;
struct GUI_Scroll;

class j1Console : public j1Module {

public:

	j1Console();

	virtual ~j1Console();

	void HandleInput(GUIElement* input, TypeInput type_input);

private:

	bool Awake(const pugi::xml_node& node) override;

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

private:
	
	GUIElement* main_console = nullptr;
	GUIElement* container_text = nullptr;
	GUI_Scroll* scroll_console = nullptr;
	p2List<iPoint> logs_positions;

public:

	void AddLog(char* string);

};
#endif

