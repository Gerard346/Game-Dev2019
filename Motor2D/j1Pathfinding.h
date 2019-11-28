#ifndef __j1PATHFINDING_H__
#define __j1PATHFINDING_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2DynArray.h"
#include "p2PQueue.h"
#include "p2Point.h"
#include "j1Module.h"

#define COST_MAP 100
#define DISTANCE_TO_REACH 25
#define MIN_DISTANCE 16

class SDL_Texture;
class info_layer;

enum typePathfinding {
	NONE_PATH,
	WALK,
	FLY
};
// ----------------------------------------------------
class j1Pathfinding : public j1Module
{
public:

	j1Pathfinding();

	// Destructor
	virtual ~j1Pathfinding();

	// Called before render is available
	bool Awake(const pugi::xml_node& conf)override;
	bool Start();
	bool PreUpdate() override;
	bool Update(float f)override;

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();


	// Pathfinding
	int MovementCost(int x, int y) const;
	void ResetPath();
	void DrawPath();
	void Path(int x, int y);

	// Propagation style
	void PropagateASTAR(iPoint origin, iPoint goal);
	bool PropagateASTARf(fPoint origin, fPoint goal, p2DynArray<iPoint>& ref);
	bool PropagateBFS(const iPoint&, const iPoint&, p2List<iPoint>*, p2PQueue<iPoint>*);

	bool CanReach(const iPoint, const iPoint);

	void TypePathfinding(typePathfinding);

private:

	bool IsWalkable(const iPoint& position) const;

private:

	mutable info_layer* walkability_layer = nullptr;

	p2PQueue<iPoint>	frontier;
	p2List<iPoint>		visited;
	p2List<iPoint>		breadcrumbs;
	uint				cost_so_far[COST_MAP][COST_MAP];
	p2DynArray<iPoint>	path;

	SDL_Texture* debug_tex = nullptr;

	char* str_load_tex = nullptr;

	iPoint prev_click_point;
};

#endif // __j1PATHFINDING_H__