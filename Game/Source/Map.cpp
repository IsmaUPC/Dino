#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    folder.Create(config.child("folder").child_value());

    return ret;
}

void Map::Draw()
{
    if(mapLoaded == false) return;

    // L03: TODO 6: Iterate all tilesets and draw all their 
    // images in 0,0 (you should have only one tileset for now)
	int x = 0, y = 0;
	for (int i = 0; i < mapInfo.tiles.count(); i++)
	{
		if (i > 0)
		{
			x += mapInfo.tiles.At(i-1)->data->texWidth;
			y += mapInfo.tiles.At(i-1)->data->texHeight;
		}
		app->render->DrawTexture(mapInfo.tiles.At(i)->data->texture, x, y);
	}
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    // L03: TODO 2: Make sure you clean up any memory allocated from tilesets/map

	// Inicializamos currentTile para que apunte al primer nodo de la List
	ListItem<TileSet*>* currentTile = mapInfo.tiles.start;
	
	// Aqui recorremos un clear de cada item recorriendo la List
	while (currentTile != NULL ){
		mapInfo.tiles.clear();
	}
	//mapData.tiles.~ListItem();

    mapFile.reset();

    return true;
}

// Load new map
bool Map::Load(const char* filename)
{
    bool ret = true;
    SString tmp("%s%s", folder.GetString(), filename);

    pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());
	
    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
        ret = false;
    }

    if(ret == true)
    {
        // L03: TODO 3: Create and call a private function to load and fill all your map data
		ret=LoadMap();
	}

    // L03: TODO 4: Create and call a private function to load a tileset
    // remember to support more any number of tilesets!
	if (ret == true)
	{
		for (pugi::xml_node tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
		{
			TileSet* set = new TileSet();

			if (ret == true) ret = LoadTilesDetails(tileset, set);
			if (ret == true) ret = LoadTilesImage(tileset, set);
			
			mapInfo.tiles.add(set);

		}
	}

    if(ret == true)
    {
        // L03: TODO 5: LOG all the data loaded iterate all tilesets and LOG everything
		LOG("Successfully parsed map XML file: %s", filename);
		LOG("Width: %d	Hight: %d", mapInfo.width, mapInfo.height);
		LOG("TileWidth: %d	TileHight: %d", mapInfo.tileWidth, mapInfo.tileHeight);
		for ( int i = 0; i <mapInfo.tiles.count(); i++)
		{
			LOG("TileSet ----");
			LOG("Name: %s	FirstGid: %d", mapInfo.tiles.At(i)->data->name.GetString(),mapInfo.tiles.At(i)->data->firstGid);
			LOG("Tile width: %d", mapInfo.tiles.At(i)->data->tile_Width);
			LOG("Tile Height: %d", mapInfo.tiles.At(i)->data->tile_Height);
			LOG("Spacing: %d", mapInfo.tiles.At(i)->data->spacing);
			LOG("Margin: %d", mapInfo.tiles.At(i)->data->margin);
		}
    }

    mapLoaded = ret;

    return ret;
}

bool Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		mapInfo.width = map.attribute("width").as_int(0);
		mapInfo.height = map.attribute("height").as_int(0);
		mapInfo.tileWidth = map.attribute("tilewidth").as_int(0);
		mapInfo.tileHeight = map.attribute("tileheight").as_int(0);
		mapInfo.nextObjectid = map.attribute("nextobjectid").as_int(0);
	}
	
	return ret;
}

bool Map::LoadTilesDetails(pugi::xml_node& tileset, TileSet* set)
{

	set->name = tileset.attribute("name").as_string("");
	set->firstGid = tileset.attribute("firstgid").as_int(0);
	set->tile_Width = tileset.attribute("tilewidth").as_int(0);
	set->tile_Height = tileset.attribute("tileheight").as_int(0);
	set->spacing = tileset.attribute("spacing").as_int(0);
	set->margin = tileset.attribute("margin").as_int(0);

	return true;
}

bool Map::LoadTilesImage(pugi::xml_node& tileset, TileSet* set)
{
	pugi::xml_node image = tileset.child("image");
	set->texture = app->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string("")));
	set->texWidth = image.attribute("width").as_int(0);
	set->texHeight = image.attribute("height").as_int(0);

	return true;
}

