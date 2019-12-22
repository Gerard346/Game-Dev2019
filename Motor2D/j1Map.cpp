#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Colliders.h"
#include <math.h>
#include "j1Window.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1FadeToBlack.h"
#include "EntityManager.h"
#include "j1Pathfinding.h"

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(const pugi::xml_node& config)
{
	BROFILER_CATEGORY("AwakeMap", Profiler::Color::ForestGreen);

	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());
	level1_path = (char*)config.child("tmx_level_1").child_value();
	level2_path = (char*)config.child("tmx_level_2").child_value();

	return ret;
}

bool j1Map::Start()
{
	App->render->camera.x = 0;
	
	if (want_to_load_map)
	{
		want_to_load_map = false;
		App->player->setAlive();
		if (App->player->p_current_lvl == Lvl_2)
		{
			//App->player->StartFromLvl2();
			LoadMaplvl2();
			//App->map->ChangeMap(level2_path);
		}
		else
		{
			//App->player->StartFromLvl1();
			LoadMaplvl1();
			//App->map->ChangeMap(level1_path);
		}

	}
	else
	{
		LoadMaplvl1();
	}

	return true;
}

bool j1Map::PreUpdate()
{
	if (map_loaded == false)
		return true;
	
	for (int i = 0; i < map_info.layers_info.Count(); i++) {
		info_layer* layer_data = map_info.layers_info[i];
		if (layer_data->type != default_layer) {
			layer_data->Update(App->Getdt());
		}
	}
	return true;
}

void j1Map::Draw()
{
	BROFILER_CATEGORY("DrawMap", Profiler::Color::ForestGreen);

	if (map_loaded == false)
		return;
	
	uint window_w, window_h;
	App->win->GetWindowSize(window_w, window_h);

	SDL_Rect camera = { App->render->camera.x, App->render->camera.y, window_w,window_h };
	SDL_Rect limits = { -camera.x / App->win->GetScale() ,0,-camera.x / App->win->GetScale() + camera.w / App->win->GetScale(),0 };

	// TODO 5: Prepare the loop to iterate all the tiles in a layer
	for (int i = 0; i < map_info.layers_info.Count(); i++) {
		info_layer* layer_data = map_info.layers_info[i];
		//LOG("--------- %s", layer_data->name.GetString());
		if (layer_data->attributes->Get_Int("Draw") == 0) {

			continue;
		}

		for (int y = 0; y < map_info.height; y++) {
			for (int x =0 ; x < map_info.width; x++) {
				// TODO 9: Complete the draw function
				uint tile = layer_data->Get(x, y);
				if (tile == 0) {
					continue;
				}
			
				const info_tileset* tileset_info = GetTilesetInfoFromTileID((int)layer_data->Get(x, y));
				
				SDL_Rect tile_rect = tileset_info->TileFromID(tile);
				int draw_x = layer_data->pos_x + x * tileset_info->tilewidth;
				int draw_y = layer_data->pos_y + y * tileset_info->tileheight;

				if (draw_x + tileset_info->tilewidth <  limits .x|| draw_x > limits.w)
				{
					continue;
				}

				if (!App->render->Blit(tileset_info->img, draw_x, draw_y, &tile_rect))
				{
					continue;
				}
				
			}
		}
	}

	
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");
	int layers_count = map_info.layers_info.Count();
	for (int i = 0; i < layers_count; i++) {
		RELEASE(map_info.layers_info[i]->tiles);
		map_info.layers_info[i]->attributes->CleanUpAttributes();
		RELEASE(map_info.layers_info[i]->attributes);
		RELEASE(map_info.layers_info[i]);
	}
	map_info.layers_info.Clear();

	for (int i = 0; i < map_info.tilesets_info.Count(); i++) {
		App->tex->UnLoad(map_info.tilesets_info.At(i)->img);
	}
	map_info.tilesets_info.Clear();
	map_info.entities_info.Clear();
	map_info.patrol_points.Clear();
	map_info.spawn_points.Clear();

	map_file.reset();
	App->colliders->EraseAllColliders();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());
	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	if (ret == true)
	{
		pugi::xml_node map_info_node = map_file.first_child();
		LoadMapInfo(map_info_node);
		//LOG("%s", map_info_node.name());


		pugi::xml_node child_map_info = map_info_node.first_child();
		while (child_map_info != NULL) {

			if (strcmp(child_map_info.name(), "tileset") == 0) {
				LoadTilesetInfo(child_map_info);
			}
			else if (strcmp(child_map_info.name(), "layer") == 0) {
				info_layer* new_layer = new info_layer();
				new_layer = LoadLayerInfo(child_map_info, new_layer);
				map_info.layers_info.PushBack(new_layer);
			}
			//LOG("%s", child_map_info.name());
			child_map_info = child_map_info.next_sibling();//Next on hierachy
		}


		map_loaded = ret;
	}

	for (int i = 0; i < map_info.layers_info.Count(); i++) {
		info_layer* current_layer = map_info.layers_info[i];
		//LOG("Layer ----");
		//LOG("name: %s", current_layer->name.GetString());
		//LOG("tile width: %d tile height: %d", current_layer->width, current_layer->height);
	}

	for (int i = 0; i < map_info.layers_info.Count(); i++)
	{
		info_layer* layer_data = map_info.layers_info[i];

		if (layer_data->attributes->Get_Int("Colliders") == 1)
		{
			COLLIDER_TYPE collider_type = COLLIDER_TYPE::COLLIDER_NONE;
			layer_type cur_layer_type = default_layer;

			if (layer_data->attributes->Get_Int("Slider") == 1)
			{
				collider_type = App->colliders->TileIDToColliderTile(layer_data->attributes->Get_Int("Collider_Type"));
				cur_layer_type = slider_layer;
			}

			for (int y = 0; y < map_info.height; y++)
			{
				for (int x = 0; x < map_info.width; x++)
				{
					uint tile = layer_data->Get(x, y);
					if (tile == 0)
					{
						continue;
					}

					const info_tileset* tileset_info = GetTilesetInfoFromTileID((int)layer_data->Get(x, y));

					if (cur_layer_type == default_layer)
					{
						collider_type = App->colliders->TileIDToColliderTile(tile);
					}

					SDL_Rect collider_rect = { x * tileset_info->tilewidth, y * tileset_info->tileheight, tileset_info->tilewidth,tileset_info->tileheight };

					if (cur_layer_type == default_layer)
					{
						switch (collider_type)
						{
						case COLLIDER_WALL:
						case COLLIDER_FINISH:
						case COLLIDER_DEAD:
							App->colliders->AddCollider(collider_rect, collider_type, this);
							break;
						case COLLIDER_START:
							map_info.spawn_points.PushBack({ x * tileset_info->tilewidth,y * tileset_info->tileheight });
							collider_rect.h = App->render->camera.h;
							App->colliders->AddCollider(collider_rect, collider_type, this);
							break;
						case COLLIDER_PATROL:
							map_info.patrol_points.PushBack({ x * tileset_info->tilewidth,y * tileset_info->tileheight });
							break;
						}
					}
					else if (cur_layer_type == slider_layer)
					{
						((mutable_layer*)layer_data)->collider.PushBack(App->colliders->AddCollider(collider_rect, collider_type, this));
					}
				}
			}
		}
		if (layer_data->attributes->Get_Int("Entities") == 1)
		{
			for (int y = 0; y < map_info.height; y++)
			{
				for (int x = 0; x < map_info.width; x++)
				{
					uint tile = layer_data->Get(x, y);
					if (tile == 0)
					{
						continue;
					}

					entityType entity_type = App->entity->TileIdToEntityType(tile);
					map_info.entities_info.PushBack(std::pair<entityType, iPoint>(entity_type, App->map->MapToWorld(x, y)));
				}
			}
		}
	}

	App->entity->SpawnEntities(map_info.entities_info);

	return ret;
}

void j1Map::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_FINISH && c2->type == COLLIDER_PLAYER && App->fade->isFading() == false) {
		App->player->ChangeLvl();
	}
}

void j1Map::LoadMaplvl1()
{
	App->map->CleanUp();
	App->map->Load(level1_path);
}

void j1Map::LoadMaplvl2()
{
	App->map->CleanUp();
	App->map->Load(level2_path);
}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * map_info.tilewidth;
	ret.y = y * map_info.tileheight;

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	ret.x = (x / map_info.tilewidth);
	ret.y = y / map_info.tileheight;

	return ret;
}

bool j1Map::LoadMapInfo(const pugi::xml_node& node)
{
	map_info.version = node.attribute("version").as_float();
	map_info.orientation = StringToOrientationEnum(node.attribute("orientation").as_string());
	map_info.order_renderer = StringToRenderOrdererEnum(node.attribute("renderorder").as_string());
	map_info.height = node.attribute("height").as_int();
	map_info.width = node.attribute("width").as_int();
	map_info.nextobjectid = node.attribute("nextobjectid").as_int();
	map_info.tileheight = node.attribute("tileheight").as_int();
	map_info.tilewidth = node.attribute("tilewidth").as_int();
	return true;
}

bool j1Map::LoadTilesetInfo(const pugi::xml_node& node)
{
	info_tileset new_tileset;

	new_tileset.firstgid = node.attribute("firstgid").as_int();
	new_tileset.name = node.attribute("name").as_string();
	
	new_tileset.tilewidth = node.attribute("tilewidth").as_int();
	new_tileset.tileheight = node.attribute("tileheight").as_int();
	
	new_tileset.spacing = node.attribute("spacing").as_int();
	new_tileset.margin = node.attribute("margin").as_int();
	
	pugi::xml_node node_img = node.child("image");
	
	new_tileset.img_height = node_img.attribute("height").as_int();
	new_tileset.img_width = node_img.attribute("width").as_int();
	
	new_tileset.img_path = node_img.attribute("source").as_string();
	p2SString full_path = folder;
	full_path+=	new_tileset.img_path;
	new_tileset.img = App->tex->Load(full_path.GetString());

	map_info.tilesets_info.PushBack(new_tileset);

	return true;
}

info_layer* j1Map::LoadLayerInfo(const pugi::xml_node& node, info_layer* new_layer)
{
	new_layer->name = node.attribute("name").as_string();
	new_layer->width = node.attribute("width").as_int();
	new_layer->height = node.attribute("height").as_int();

	//LOG("%s", new_layer->name.GetString());
	new_layer= LoadLayerAttributes(node.child("properties"), new_layer);

	int layer_size = new_layer->width * new_layer->height * sizeof(int);
	new_layer->tiles = (int*)malloc(layer_size);
	memset(new_layer->tiles, 0, layer_size);

	pugi::xml_node data_node = node.child("data");
	pugi::xml_node tile_node = data_node.first_child();
	int i = 0;
	while (tile_node != NULL) {
		new_layer->tiles[i] = tile_node.attribute("gid").as_int();

		tile_node = tile_node.next_sibling();
		i += 1;
	}
	
	return new_layer;
}

map_orientation j1Map::StringToOrientationEnum(const char* input_string) const
{
	if ( strcmp(input_string, "orthogonal")==0) return map_orientation::orthogonal;
	if  ( strcmp(input_string, "isometric")==0) return map_orientation::isometric;
	return map_orientation::no_orientation;
}

render_orderer j1Map::StringToRenderOrdererEnum(const char* input_string) const
{
	if (strcmp(input_string, "right_down") == 0) return render_orderer::right_down;
	return render_orderer::no_order;
}

const info_tileset* j1Map::GetTilesetInfoFromTileID(int tile_ID) const
{
	
	for (int i = 0; i < map_info.tilesets_info.Count(); i++) 
	{
		if (i == map_info.tilesets_info.Count() - 1) {
			return  map_info.tilesets_info.At(i);
		}
		
		const info_tileset* tileset_data = map_info.tilesets_info.At(i);
		const info_tileset* next_tileset_data = map_info.tilesets_info.At(i + 1);
		
		if(tileset_data->firstgid <= tile_ID && next_tileset_data->firstgid > tile_ID){
			return map_info.tilesets_info.At(i);
		}
	}

	return nullptr;
}

char* j1Map::GetPathFromLevel(lvl_map lvl)
{
	if (lvl == lvl_1_map) return level1_path;
	if (lvl == lvl_2_map) return level2_path;
	return nullptr;
}

bool j1Map::GetPatrolPoints(float map_x, float map_y, fPoint& a, fPoint& b) const
{
	iPoint origin = { (int)map_x, (int)map_y };
	origin = App->map->WorldToMap(origin.x, origin.y);

	int distance = map_info.width * map_info.height * map_info.tilewidth;
	fPoint oirigin = { (float)map_x, (float)map_y };
	fPoint target = { -1, -1 };

	p2PQueue<fPoint> points_queue;

	for (int i = 0; i < map_info.patrol_points.Count(); i++)
	{
		iPoint cur_point = *map_info.patrol_points.At(i);

		if (App->path->CanReach(origin, App->map->WorldToMap(cur_point.x, cur_point.y)) == false)continue;

		fPoint map_patrol_point = { (float)cur_point.x,(float)cur_point.y };
		int cur_distance = oirigin.DistanceManhattan(map_patrol_point);

		points_queue.Push(map_patrol_point, cur_distance * -1);
	}

	points_queue.Pop(a);
	points_queue.Pop(b);

	return true;
}


/*fPoint j1Map::GetNearestReachablePatrolPoint(float map_x, float map_y) const
{
	iPoint origin = { (int)map_x, (int)map_y };
	origin = App->map->WorldToMap(origin.x, origin.y);

	int distance = map_info.width * map_info.height * map_info.tilewidth;
	fPoint oirigin = { (float)map_x, (float)map_y };
	fPoint target = { -1, -1};
	
	for (int i = 0; i < map_info.patrol_points.Count(); i++)
	{
		iPoint cur_point = *map_info.patrol_points.At(i);

		if (App->path->CanReach(origin, App->map->WorldToMap(cur_point.x, cur_point.y)) == false)continue;

		fPoint map_patrol_point = { (float)cur_point.x,(float)cur_point.y };
		int cur_distance = oirigin.DistanceManhattan(map_patrol_point);
		
		if (cur_distance < distance && cur_distance > MIN_DISTANCE)
		{
			distance = cur_distance;
			target = map_patrol_point;
		}
	}

	return target;
}*/

info_layer* j1Map::GetLayer(char* name)const
{
	for (int i = 0; i < map_info.layers_info.Count(); i++)
	{
		if (strcmp(name, map_info.layers_info[i]->name.GetString()) == 0)
		{
			return map_info.layers_info[i];
		}
	}
	return nullptr;
}

void Attributes::CleanUpAttributes()
{
	for (int i = 0; i < attributes_info.Count(); i++) {
		RELEASE(attributes_info[i]);
	}
}

int Attributes::Get_Int(const char* name)
{
	for (int i = 0; i < attributes_info.Count(); i++) {
		if (strcmp(name, attributes_info[i]->name.GetString()) == 0) {
			return attributes_info[i]->value;
		}
	}
	return 0;
}

float Attributes::Get_Float(const char* name)
{
	for (int i = 0; i < attributes_info.Count(); i++) {
		if (strcmp(name, attributes_info[i]->name.GetString()) == 0) {
			return attributes_info[i]->value;
		}
	}
	return 0;
}

info_layer* j1Map::LoadLayerAttributes(const pugi::xml_node &node, info_layer* layer)
{
	pugi::xml_node property_node = node.first_child();
	layer->attributes = new Attributes();
	while (property_node != NULL) {

		Attribute* new_attribute = new Attribute();

		new_attribute->name = property_node.attribute("name").as_string();
		new_attribute->value = property_node.attribute("value").as_float();
		layer->attributes->attributes_info.PushBack(new_attribute);
		//LOG("%s / %i", new_attribute->name.GetString(), new_attribute->value);
		property_node = property_node.next_sibling();
		
	}
	if (layer->attributes->Get_Int("Slider") == 1) {

		mutable_layer* new_layer = new mutable_layer(layer);

		new_layer->range_x = new_layer->attributes->Get_Int("Range.x");
		new_layer->range_y = new_layer->attributes->Get_Int("Range.y");
		new_layer->velocity_x = new_layer->attributes->Get_Int("Velocity.x");
		new_layer->velocity_y = new_layer->attributes->Get_Int("Velocity.y");

		layer = new_layer;
	}

	if (layer->attributes->Get_Int("Parallax") == 1) {
		parallax_layer* new_layer = new parallax_layer(layer);
		new_layer->x_delta_relation = new_layer->attributes->Get_Float("Velocity_x");

		layer = new_layer;
	}
	return layer;
}

void mutable_layer::Update(float dt)
{
	if (App->scene->GetPause()) {
		return;
	}
	int delta_x = 0;
	int delta_y = 0;

	if (velocity_x != 0)
	{
		delta_x = pos_x;
		pos_x += velocity_x*dt;

		if (abs(pos_x) > range_x)
		{
			pos_x = velocity_x>0?range_x:-range_x;
			velocity_x *= -1;
		}
		delta_x = pos_x - delta_x;
	}
	if (velocity_y != 0)
	{
		delta_y = pos_y;
		pos_y += velocity_y *dt;
		
		if (abs(pos_y) > range_y)
		{
			pos_y = velocity_y>0?range_y:-range_y;
			
			velocity_y *= -1;
		}
		delta_y = pos_y - delta_y;
	}

	for (int i = 0; i < collider.Count(); i++) {
		collider[i]->SetPos(collider[i]->rect.x + delta_x, collider[i]->rect.y + delta_y);
	}
}


info_layer::info_layer(const info_layer* copy):name(copy->name), width(copy->width), height(copy->height), pos_x(copy->pos_x), pos_y(copy->pos_y), tiles(copy->tiles), attributes(copy->attributes)
{
	
}

mutable_layer::mutable_layer(const info_layer* copy):info_layer(copy)
{
	type = layer_type::slider_layer;
}

parallax_layer::parallax_layer(const info_layer* copy) : info_layer(copy)
{
	type = layer_type::parallax;
}

void parallax_layer::Update(float dt)
{
	pos_x = initial_pos_x - App->render->camera.x * x_delta_relation;
}

