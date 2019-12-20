#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "p2DynArray.h"
#include "SDL/include/SDL_rect.h"
#include "BaseEntity.h"
#define MAX_TILESET 10

struct Attribute {
	p2SString name;
	float value = 0.0f;
};

struct Attributes
{
	p2DynArray<Attribute*> attributes_info;
	void CleanUpAttributes();
	int Get_Int(const char*);
	float Get_Float(const char*);
};

enum lvl_map {
	lvl_1_map,
	lvl_2_map
};
enum map_orientation {
	no_orientation,
	orthogonal,
	isometric
};

enum render_orderer {
	no_order,
	right_down
};

enum layer_type {
	default_layer = 0,
	slider_layer,
	parallax
};

struct info_tileset {
	int firstgid = 1;
	p2SString name = "";
	int tilewidth = 0;
	int tileheight = 0;
	int spacing = 0;
	int margin = 0;
	p2SString img_path = "";
	int img_width = 0;
	int img_height = 0;
	SDL_Texture* img = nullptr;

	const SDL_Rect TileFromID(uint tile_ID) const {

		int width = img_width / (spacing + tilewidth);
		uint local_tile_ID = tile_ID - firstgid;
		int y = local_tile_ID/width;
		
		int x = local_tile_ID - y * width;
		int y_coords = margin + y*(tileheight + spacing);
		int x_coords = margin + x * (tilewidth + spacing);
		return { x_coords, y_coords, tilewidth, tileheight };
	}
};

struct info_layer {

	info_layer(){};
	info_layer(const info_layer* copy);

	p2SString name = "";
	int width = 0;
	int height = 0;
	int pos_x = 0;
	int pos_y = 0;
	int* tiles = nullptr;
	
	layer_type type = default_layer;
	Attributes* attributes;

	inline uint Get(int x, int y) const {
		return tiles[x + y * width];
	}
	virtual void Update(float dt) {};
};

struct mutable_layer:info_layer {
	mutable_layer(const info_layer* copy);

	int range_x = 0;
	int range_y = 0;
	int velocity_x = 0;
	int velocity_y = 0;
	int current_velocity_y = 0;
	p2DynArray<Collider*> collider;

	void Update(float dt) override;
};

struct parallax_layer : info_layer
{
	parallax_layer(const info_layer* copy);

	float x_delta_relation = 0.0f;
	int initial_pos_x = 0;

	void Update(float dt) override;
};

struct info_map {

	float version = 0.0f;
	map_orientation orientation = no_orientation;
	render_orderer order_renderer = no_order;
	int width = 0;
	int height = 0;
	int tilewidth = 0;
	int tileheight = 0;
	int nextobjectid = 0;
	p2DynArray<info_tileset>tilesets_info;
	p2DynArray<info_layer*> layers_info;
	p2DynArray<std::pair<entityType, iPoint>> entities_info;
	p2DynArray<iPoint> spawn_points; 
	p2DynArray<iPoint> patrol_points;

};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(const pugi::xml_node&) override;

	bool Start() override;
	
	bool PreUpdate() override;
	
	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	void OnCollision(Collider*, Collider*) override;

	void ChangeMap(const char*);

	void LoadMaplvl1();
	void LoadMaplvl2();
	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;

private:

	bool LoadMapInfo(const pugi::xml_node&);
	bool LoadTilesetInfo(const pugi::xml_node&);
	info_layer* LoadLayerInfo(const pugi::xml_node&, info_layer* layer);
	info_layer* LoadLayerAttributes(const pugi::xml_node&, info_layer*);

	map_orientation StringToOrientationEnum(const char*) const;
	render_orderer StringToRenderOrdererEnum(const char*) const;

	
public:

	info_map map_info;

	info_layer* GetLayer(char* name)const;
	const info_tileset* GetTilesetInfoFromTileID(int)const;
	char* GetPathFromLevel(lvl_map lvl);
	
	bool GetPatrolPoints(float map_x, float map_y, fPoint& a, fPoint& b)const;

	void WantToLoadMap() { want_to_load_map = true; }

private:
	
	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
	bool				want_to_load_map = false;
	char*				level1_path = nullptr;
	char*				level2_path = nullptr;
};

#endif // __j1MAP_H__