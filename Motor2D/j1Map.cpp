#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Colliders.h"
#include <math.h>

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
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

bool j1Map::PreUpdate()
{
	if (map_loaded == false)
		return true;
	
	for (int i = 0; i < map_info.layers_info.Count(); i++) {
		info_layer* layer_data = map_info.layers_info[i];
		if (layer_data->attributes->Get_Int("Slider") == 1) {
			layer_data->Update(0.0f);
		}
	}
	return true;
}

void j1Map::Draw()
{

	if (map_loaded == false)
		return;
	
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
				if (!App->render->Blit(tileset_info->img, layer_data->pos_x+x * tileset_info->tilewidth, layer_data->pos_y+ y * tileset_info->tileheight, &tile_rect)) {
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


	// TODO 2: clean up all layer data
	// Remove all layers


	// Clean up the pugui tree
	map_file.reset();
	
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

	for (int i = 0; i < map_info.layers_info.Count(); i++) {
		info_layer* layer_data = map_info.layers_info[i];
		//LOG("--------- %s", layer_data->name.GetString());

		if (layer_data->attributes->Get_Int("Colliders") == 0) {
			continue;
		}

		COLLIDER_TYPE collider_type = COLLIDER_TYPE::COLLIDER_NONE;
		bool fixed_collider_id = false;


		if (layer_data->attributes->Get_Int("Slider") == 1) {
			collider_type = App->colliders->TileIDToColliderTile(layer_data->attributes->Get_Int("Collider_Type"));
			fixed_collider_id = true;
		}
		for (int y = 0; y < map_info.height; y++) {
			for (int x = 0; x < map_info.width; x++) {
				uint tile = layer_data->Get(x, y);
				if (tile == 0) {
					continue;
				}
				
				const info_tileset* tileset_info = GetTilesetInfoFromTileID((int)layer_data->Get(x, y));
				
				if (!fixed_collider_id) {
					collider_type = App->colliders->TileIDToColliderTile(tile);
				}

				SDL_Rect collider_rect = { x*tileset_info->tilewidth, y*tileset_info->tileheight, tileset_info->tilewidth,tileset_info->tileheight};
				if (!fixed_collider_id) {
					App->colliders->AddCollider(collider_rect, collider_type);
					if (collider_type == COLLIDER_PLAYER) {
						map_info.p_spaw_point = iPoint (x*tileset_info->tilewidth,y*tileset_info->tileheight);
					}
				}
				else {
					((mutable_layer*)layer_data)->collider.PushBack(App->colliders->AddCollider(collider_rect, collider_type));
				}

			}
		}
	}

	return ret;
}

void j1Map::ChangeMap(const char* path)
{
	App->map->CleanUp();
	App->map->Load(path);
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
	
	for (int i = 0; i < map_info.tilesets_info.Count(); i++) {
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

info_layer* j1Map::LoadLayerAttributes(const pugi::xml_node &node, info_layer* layer)
{
	pugi::xml_node property_node = node.first_child();
	layer->attributes = new Attributes();
	while (property_node != NULL) {

		Attribute* new_attribute = new Attribute();

		new_attribute->name = property_node.attribute("name").as_string();
		new_attribute->value = property_node.attribute("value").as_int();
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
	return layer;
}

void mutable_layer::Update(float dt)
{
	int delta_x = 0;
	int delta_y = 0;

	if (velocity_x != 0)
	{
		delta_x = pos_x;
		pos_x += velocity_x;

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
		pos_y += velocity_y;
		
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

}
