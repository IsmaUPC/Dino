#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"

#include "PugiXml\src\pugixml.hpp"

// L03: TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSet
{
	SString name;
	int firstGid;
	int	tile_Width;
	int	tile_Height;
	int	spacing;
	int	margin;

	SDL_Texture *texture;
	int texWidth;
	int texHeight;
	int	numTilesWidth;
	int	numTilesHeight;

};

struct Layer
{
	SString	name;
	int width;
	int height;
	uint* data;

};

// L03: TODO 1: Create a struct needed to hold the information to Map node
struct MapInfo
{
	int width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	int	nextObjectid;
	List<TileSet*> tiles;
};

class Map : public Module
{
public:

    Map();

    // Destructor
    virtual ~Map();

    // Called before render is available
    bool Awake(pugi::xml_node& conf);

    // Called each loop iteration
    void Draw();

    // Called before quitting
    bool CleanUp();

    // Load new map
    bool Load(const char* path);

    // L03: TODO 1: Add your struct for map info as public for now
	MapInfo mapInfo;
private:

	bool LoadMap();
	bool LoadTilesDetails(pugi::xml_node& tileset, TileSet* set);
	bool LoadTilesImage(pugi::xml_node& tileset, TileSet* set);

    pugi::xml_document mapFile;
    pugi::xml_node rootMapInfo;
    pugi::xml_node currentTile;
    SString folder;
    bool mapLoaded;

};

#endif // __MAP_H__