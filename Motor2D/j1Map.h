#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "p2DynArray.h"
#include "SDL/include/SDL_rect.h"
#define MAX_TILESET 10

struct Attribute {
	p2SString name;
	int value = 0;
};

struct Attributes
{
	p2DynArray<Attribute*> attributes_info;

	int Get_Int(const char*);

	void LoadLayerAttributes(const pugi::xml_node&);
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

struct info_tileset {
	// TODO 7: Create a method that receives a tile id and returns it's Rect
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
	p2SString name = "";
	int width = 0;
	int height = 0;
	int* tiles = nullptr;
	// TODO 6: Short function to get the value of x,y
	inline uint Get(int x, int y) const {
		return tiles[x + y*width];
	}
	Attributes attributes;
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
	p2DynArray<info_layer> layers_info;
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

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);
	// TODO 8: Create a method that translates x,y coordinates from map positions to world positions

private:
	bool LoadMapInfo(const pugi::xml_node&);
	bool LoadTilesetInfo(const pugi::xml_node&);
	bool LoadLayerInfo(const pugi::xml_node&, info_layer* layer);
	
	map_orientation StringToOrientationEnum(const char*) const;
	render_orderer StringToRenderOrdererEnum(const char*) const;

	const info_tileset* GetTilesetInfoFromTileID(int)const;
	
public:

	
	info_map map_info;


private:
	
	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;

};

#endif // __j1MAP_H__