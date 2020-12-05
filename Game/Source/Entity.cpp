#include "Entity.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

Entity::Entity(TypeEntity pTypeEntity, iPoint pPosition, float pVelocity, SDL_Texture* pTexture) : Module()
{
	name.Create("Entity");
	isAlive = true;
	entityData = new EntityData(pTypeEntity, pPosition, pVelocity, pTexture);
}

Entity::Entity() : Module()
{
	EntityData();
	name.Create("Entity");
	isAlive = true;
}

// Destructor
Entity::~Entity()
{}

// Called before Entity is available
bool Entity::Awake(pugi::xml_node& config)
{
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Entity::Start()
{
	LOG("Entity start");
	// back background
	return true;
}

// Called each loop iteration
bool Entity::PreUpdate()
{
	return true;
}

bool Entity::Update(float dt)
{
	return true;
}

bool Entity::PostUpdate()
{

	return true;
}

// Called before quitting
bool Entity::CleanUp()
{
	LOG("Destroying Entity");
	active = false;
	return true;
}

// Load Game State
bool Entity::LoadState(pugi::xml_node& data)
{

	return true;
}

// Save Game State
bool Entity::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.child(" ");

	return true;
}
iPoint Entity::TransformFPoint(fPoint fpoint)
{
	iPoint transformation;

	transformation.x = fpoint.x;
	transformation.y = fpoint.y;

	return transformation;
}
// Comprove position player in array of tiles in mapLayer collision
int Entity::CheckCollision(iPoint positionMap)
{
	uint typeTilePlayer = app->map->data.layers.At(2)->data->Get(positionMap.x, positionMap.y);
	uint firstgidLayerCollisions = app->map->data.tilesets.At(2)->data->firstgid;
	typeTilePlayer -= firstgidLayerCollisions;

	if (app->player->godMode == false) {
		switch (typeTilePlayer)
		{
		case VICTORY:
			//victory
			app->player->win = true;
			return 0;
			break;

		case COLLISION:
			//collision
			return 1;
			break;

		case CHECK_POINT:
			//checkpoint
			app->SaveGameRequest();
			app->player->activeCheckpoint(positionMap);
			return 2;
			break;

		default:
			return -1;
			break;
		}
	}

	return false;
}