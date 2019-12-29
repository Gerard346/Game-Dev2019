#ifndef __GUIELEMENT_H__
#define __GUIELEMENT_H__
#include "p2List.h"
#include "p2Point.h"
#include "SDL/include/SDL_rect.h"

struct j1Module;

enum TypeGUI {
	UNDEFINED,
	BUTTON,
	SLIDER_UI,
	IMAGE,
	TEXT,
	TEXT_BOX,
	WINDOW
};

class GUIElement {

public:

	GUIElement(TypeGUI type_gui = TypeGUI::UNDEFINED);
	~GUIElement();

	virtual bool Update(float dt);
	bool UpdateChilds(float dt);

	virtual void HandleInput();

	virtual bool CleanUp();
	bool CleanUpChilds();

	virtual bool Draw(bool debug);
	void DebugDraw(SDL_Color color);
	bool DrawChilds(bool debug);

	SDL_Rect GetScreenRect()const;
	SDL_Rect GetLocalRect()const;
	iPoint GetScreenPos()const;
	iPoint GetLocalPos()const;

	virtual void SetLocalPos(iPoint);
	virtual void SetLocalRect(SDL_Rect elem);

	void AddChild(GUIElement* elem, int child_layer = 0);

	void SetParent(const GUIElement*);
	GUIElement* GetParent() const;
	virtual void SetLayer(int lay);
	int GetLayer()const;

	void SetElemActive(bool active);
	bool GetElemActive() const;
	bool GetElemInteractive()const;
	void SetElemInteractive(bool);
	void SetInputTarget(j1Module* module);

	void SetElemsInteractiveForChilds(bool);

	bool RectIsIn(const SDL_Rect& target, int x_vel, int y_vel, bool x_axis) const;
	bool RectCheck(const SDL_Rect& in_target, const SDL_Rect& out_target, bool x_axis)const;

	bool MouseIn();

	virtual void AddPosition(int x, int y);
public:
	p2List<GUIElement*> childs;
	GUIElement* parent = nullptr;
protected:
	TypeGUI type_gui = TypeGUI::UNDEFINED;
	j1Module* input_target = nullptr;

	bool active_elem = true;
	bool interactive = true;

	SDL_Rect box_elem = { 0,0,0,0 };
	int layer = 0;
};
#endif