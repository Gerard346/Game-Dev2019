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
	TEXT_BOX
};

class GUIElement {

public:

	GUIElement(TypeGUI type_gui = UNDEFINED);
	~GUIElement();

	virtual bool Update(float dt);
	bool UpdateChilds(float dt);

	virtual bool CleanUp();
	bool CleanUpChilds();

	virtual bool Draw(bool debug);
	void DebugDraw(SDL_Color color);
	bool DrawChilds(bool debug);

	virtual void SetBoxElem(SDL_Rect elem);
	virtual void SetPosElem(iPoint position);

	void AddChild(GUIElement* elem, int child_layer = 0);

	void SetLayer(int lay);
	int GetLayer()const;

	void SetElemActive(bool active);
	bool GetElemActive() const;

public:
	p2List<GUIElement*> childs;

protected:
	TypeGUI type_gui = UNDEFINED;

	bool active_elem = true;
	bool interactive = true;

	SDL_Rect box_elem = { 0,0,0,0 };
	int layer = 0;
};