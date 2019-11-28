#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include <math.h>
#include "j1Input.h"
#include "j1Window.h"

j1Pathfinding::j1Pathfinding() : j1Module()
{
	name.create("pathfinding");
}

// Destructor
j1Pathfinding::~j1Pathfinding()
{}

// Called before render is available
bool j1Pathfinding::Awake(const pugi::xml_node& config)
{
	bool ret = true;

	str_load_tex = (char*)config.child("debug_texture").child_value();

	ResetPath();

	return ret;
}

bool j1Pathfinding::Start()
{
	debug_tex = App->tex->Load(str_load_tex);
	return true;
}

bool j1Pathfinding::PreUpdate()
{
	if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
	{
		if (prev_click_point.x == 0 && prev_click_point.y == 0)
		{
			App->input->GetMousePosition(prev_click_point.x, prev_click_point.y);
			prev_click_point.x -= App->render->camera.x / App->win->GetScale();
			prev_click_point.y -= App->render->camera.y / App->win->GetScale();
		}
		else
		{
			iPoint p;
			App->input->GetMousePosition(p.x, p.y);
			p.x -= App->render->camera.x / App->win->GetScale();
			p.y -= App->render->camera.y / App->win->GetScale();

			PropagateASTAR(prev_click_point, p);

			prev_click_point.x = prev_click_point.y = 0;
		}
	}



	return true;
}

bool j1Pathfinding::Update(float f)
{
	Draw();
	return true;
}

void j1Pathfinding::ResetPath()
{
	path.Clear();
	frontier.Clear();
	visited.clear();
	breadcrumbs.clear();
	memset(cost_so_far, 0, sizeof(uint) * COST_MAP * COST_MAP);
}

void j1Pathfinding::Path(int x, int y)
{
	path.Clear();
	iPoint goal = App->map->WorldToMap(x, y);

	int goal_index = visited.find(goal);
	if (goal_index == -1)
	{
		return;
	}
	path.PushBack(goal);
	while (true)
	{
		path.PushBack(breadcrumbs[goal_index]);
		goal_index = visited.find(breadcrumbs[goal_index]);
		if (visited[goal_index] == breadcrumbs[goal_index])
		{
			break;
		}
	}
}

void j1Pathfinding::PropagateASTAR(iPoint origin, iPoint goal)
{
	iPoint map_goal = App->map->WorldToMap(goal.x, goal.y);

	ResetPath();

	if (walkability_layer == nullptr)
	{
		return;
	}

	iPoint map_origin = App->map->WorldToMap(origin.x, origin.y);
	frontier.Push(map_origin, 0);
	visited.add(map_origin);
	breadcrumbs.add(map_origin);

	while (frontier.Count() > 0)
	{
		iPoint curr;
		if (frontier.Pop(curr))
		{
			iPoint neighbors[4];
			neighbors[0].create(curr.x + 1, curr.y + 0);
			neighbors[1].create(curr.x + 0, curr.y + 1);
			neighbors[2].create(curr.x - 1, curr.y + 0);
			neighbors[3].create(curr.x + 0, curr.y - 1);

			for (uint i = 0; i < 4; ++i)
			{
				int neighbor_cost = MovementCost(neighbors[i].x, neighbors[i].y);
				if (neighbor_cost > 0)
				{
					if (visited.find(neighbors[i]) == -1)
					{
						int x_distance = map_goal.x > neighbors[i].x ? map_goal.x - neighbors[i].x : neighbors[i].x - map_goal.x;
						int y_distance = map_goal.y > neighbors[i].y ? map_goal.y - neighbors[i].y : neighbors[i].y - map_goal.y;

						frontier.Push(neighbors[i], neighbor_cost + cost_so_far[neighbors[i].x][neighbors[i].y] + x_distance + y_distance);
						visited.add(neighbors[i]);
						breadcrumbs.add(curr);
						cost_so_far[neighbors[i].x][neighbors[i].y] = neighbor_cost;

						if (neighbors[i].x == map_goal.x && neighbors[i].y == map_goal.y)
						{
							Path(goal.x, goal.y);
							return;
						}
					}
				}
			}
		}
	}
}

bool j1Pathfinding::PropagateASTARf(fPoint origin, fPoint goal, p2DynArray<iPoint>& ref)
{
	iPoint map_goal = App->map->WorldToMap(goal.x, goal.y);

	ResetPath();

	if (walkability_layer == nullptr)
	{
		return false;

	}

	iPoint map_origin = App->map->WorldToMap(origin.x, origin.y);
	frontier.Push(map_origin, 0);
	visited.add(map_origin);
	breadcrumbs.add(map_origin);

	while (frontier.Count() > 0)
	{
		iPoint curr;
		if (frontier.Pop(curr))
		{
			iPoint neighbors[4];
			neighbors[0].create(curr.x + 1, curr.y + 0);
			neighbors[1].create(curr.x + 0, curr.y + 1);
			neighbors[2].create(curr.x - 1, curr.y + 0);
			neighbors[3].create(curr.x + 0, curr.y - 1);

			for (uint i = 0; i < 4; ++i)
			{
				int neighbor_cost = MovementCost(neighbors[i].x, neighbors[i].y);
				if (neighbor_cost > 0)
				{
					if (visited.find(neighbors[i]) == -1)
					{
						int x_distance = map_goal.x > neighbors[i].x ? map_goal.x - neighbors[i].x : neighbors[i].x - map_goal.x;
						int y_distance = map_goal.y > neighbors[i].y ? map_goal.y - neighbors[i].y : neighbors[i].y - map_goal.y;

						frontier.Push(neighbors[i], neighbor_cost + cost_so_far[neighbors[i].x][neighbors[i].y] + x_distance + y_distance);
						visited.add(neighbors[i]);
						breadcrumbs.add(curr);
						cost_so_far[neighbors[i].x][neighbors[i].y] = neighbor_cost;

						if (neighbors[i].x == map_goal.x && neighbors[i].y == map_goal.y)
						{
							Path(goal.x, goal.y);

							if (path.Count() > 0)
							{
								ref.Clear();
								for (int i = 0; i < path.Count(); i++)
								{
									ref.PushBack(*path.At(i));
								}
								return true;
							}
							else
							{
								return false;
							}							
						}
					}
				}
			}
		}
	}
	return false;
}

bool j1Pathfinding::CanReach(const iPoint origin, const iPoint destination)
{
	p2List<iPoint> closed_list;
	p2PQueue<iPoint> open_list;
	open_list.Push(origin,0);
	uint distance_to_loop = origin.DistanceManhattan(destination) * DISTANCE_TO_REACH;

	while (distance_to_loop > 0)
	{
		if (PropagateBFS(origin, destination, &closed_list, &open_list))
		{
			//LOG("TRUE");
			closed_list.clear();
			open_list.Clear();
			return true;
		}
		distance_to_loop--;
	}
	//LOG("FALSE");
	closed_list.clear();
	open_list.Clear();
	return false;
}

void j1Pathfinding::TypePathfinding(typePathfinding type)
{
	switch (type)
	{
	case NONE:
		break;
	case WALK:
		walkability_layer = App->map->GetLayer("Walkability");
		break;
	case FLY:
		walkability_layer = App->map->GetLayer("WalkabilityFly");
		break;
	default:
		break;
	}
}

bool j1Pathfinding::IsWalkable(const iPoint& position) const
{
	return walkability_layer->Get(position.x, position.y) > 0;
}

bool j1Pathfinding::PropagateBFS(const iPoint& origin, const iPoint& destination, p2List<iPoint>* closed, p2PQueue<iPoint>* open_list)
{

	if (walkability_layer == nullptr)
	{
		return false;
	}

	p2List<iPoint>* closed_list;
	if (closed == nullptr)
		closed_list = &this->visited;
	else closed_list = closed;

	p2PQueue<iPoint>* open_l;
	if (open_list == nullptr)
		open_l = &this->frontier;
	else open_l = open_list;

	if (closed_list->find(destination) != -1)
	{
		return true;
	}

	iPoint point;

	if (open_l->start != NULL && closed_list->find(destination) == -1)
	{
		open_l->Pop(point);

		if (open_l->find(point) == -1)
			closed_list->add(point);
		iPoint neightbour[4];

		neightbour[0] = { point.x - 1, point.y };
		neightbour[1] = { point.x + 1, point.y };
		neightbour[2] = { point.x, point.y - 1 };
		neightbour[3] = { point.x, point.y + 1 };

		for (uint i = 0; i < 4; i++)
		{
			if (closed_list->find(neightbour[i]) == -1 && IsWalkable(neightbour[i]))
			{
				open_l->Push(neightbour[i],0);
				closed_list->add(neightbour[i]);
			}
		}
	}

	return false;
}
int j1Pathfinding::MovementCost(int x, int y) const
{
	int ret = -1;

	if (x >= 0 && x < walkability_layer->width && y >= 0 && y < walkability_layer->height)
	{
		int id = walkability_layer->Get(x, y);

		if (id == 6)
		{
			ret = 1;
		}
		else
		{
			ret = -1;
		}
	}

	return ret;
}

void j1Pathfinding::DrawPath()
{
	iPoint point;

	// Draw visited
	p2List_item<iPoint>* item = visited.start;

	while (item)
	{
		point = item->data;
		iPoint pos = App->map->MapToWorld(point.x, point.y);

		App->render->DrawQuad({ pos.x, pos.y, 16,16}, 255, 0, 0, 150);

		item = item->next;
	}

	// Draw frontier
	for (uint i = 0; i < frontier.Count(); ++i)
	{
		point = *(frontier.Peek(i));

		iPoint pos = App->map->MapToWorld(point.x, point.y);

		App->render->DrawQuad({ pos.x, pos.y, 16,16 }, 0, 255, 0, 150);
	}

	// Draw path
	for (uint i = 0; i < path.Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path[i].x, path[i].y);

		App->render->DrawQuad({ pos.x, pos.y, 16,16 }, 0, 0, 255, 150);
		break;
	}
}

void j1Pathfinding::Draw()
{
	DrawPath();
}

bool j1Pathfinding::CleanUp()
{
	LOG("Unloading pathfinding");

	ResetPath();

	return true;
}
