#include "j1Console.h"
#include "j1Gui.h"
#include "GUIElement.h"
#include "j1Fonts.h"
#include "GUI_Button.h"
#include "GUI_Image.h"
#include "GUI_Window.h"
#include "GUI_String.h"
#include "GUI_Scroll.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Gui.h"

j1Console::j1Console() : j1Module()
{
}

j1Console::~j1Console()
{
}

bool j1Console::Awake(const pugi::xml_node& node)
{
	return true;
}

bool j1Console::Start()
{
	main_console = App->gui->GenerateElemGUI(TypeGUI::UNDEFINED);
	main_console->SetLocalRect({ 0,0,App->render->camera.w / (int)App->win->GetScale(),175 });
	main_console->SetElemInteractive(false);

	container_text = App->gui->GenerateElemGUI(TypeGUI::UNDEFINED);
	container_text->SetLocalRect({ 10,10,(App->render->camera.w / (int)App->win->GetScale())-20, 135 });
	container_text->SetElemInteractive(false);

	main_console->AddChild(container_text);

	//Scroll console
	scroll_console = (GUI_Scroll*)App->gui->GenerateElemGUI(TypeGUI::SLIDER_UI);

	scroll_console->SetTypeScroll(VERTICAL);
	scroll_console->SetScrollBackground({ 234,275,16,175 }, 0);
	scroll_console->SetScrollImage({ 218, 270,32,32 }, 0);
	scroll_console->SetLocalPos({ 490, 0 });
	scroll_console->SetMaxValue(1.0f);
	scroll_console->SetElemInteractive(true);
	scroll_console->SetInputTarget(this);

	main_console->AddChild(scroll_console);


	App->gui->SetConsoleGUI(main_console);

	return true;
}

bool j1Console::PreUpdate()
{
	return true;
}

bool j1Console::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_GRAVE) == KEY_DOWN) {
		main_console->SetElemActive(!main_console->GetElemActive());
	}

	return true;
}

bool j1Console::PostUpdate()
{
	return true;
}

bool j1Console::CleanUp()
{
	main_console->CleanUp();
	active = false;

	return true;
}

void j1Console::AddLog(char* string)
{
	if (container_text == nullptr || active == false)return;

	GUI_String* new_string = (GUI_String*)App->gui->GenerateElemGUI(TypeGUI::TEXT);
	new_string->SetColor(App->gui->WHITE);
	new_string->SetText(string);
	new_string->FitBox();

	if (logs_positions.count() > 0)
	{
		iPoint target_pos = logs_positions.At(logs_positions.count() - 1)->data;
		target_pos.y += 20;

		new_string->SetLocalPos(target_pos);
	}

	logs_positions.add(new_string->GetLocalPos());

	container_text->AddChild(new_string);
}
