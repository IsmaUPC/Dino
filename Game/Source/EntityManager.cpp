#include "EntityManager.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("EntityManager");

}

// Destructor
EntityManager::~EntityManager()
{}

// Called before EntityManager is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	bool ret = true;


	return ret;
}

// Called before the first frame
bool EntityManager::Start()
{
	LOG("EntityManager start");
	// back background
	return true;
}

// Called each loop iteration
bool EntityManager::PreUpdate()
{
	for (ListItem<Entity*>* entiti = entities.start; entiti; entiti = entiti->next)
		if(entiti->data->pendingToDelete)entities.Del(entiti);
	

	for (ListItem<Entity*>* entiti = entities.start; entiti; entiti = entiti->next) entiti->data->PreUpdate();

	return true;
}

bool EntityManager::Update(float dt)
{
	HandleEntitiesSpawn();
	for (ListItem<Entity*>* entiti = entities.start; entiti; entiti=entiti->next) entiti->data->Update(dt);
	HandleEntitiesDespawn();

	return true;
}

bool EntityManager::PostUpdate()
{
	for (ListItem<Entity*>* entiti = entities.start; entiti; entiti = entiti->next) entiti->data->PostUpdate();

	return true;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	LOG("Destroying EntityManager");

	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		if (item->data->active == true)
			ret = item->data->CleanUp();

		item = item->prev;
	}

	return ret;
	//return true;
}


// Load Game State


bool EntityManager::AddEntity(TypeEntity pType, int pX, int pY)
{
	iPoint positionSpawn = app->entity->MapToWorld(iPoint(pX,pY));
	if (spawnQueue.Add(new EntitySpawnPoint(pType, positionSpawn.x, positionSpawn.y))) return true;
	else return false;
}

void EntityManager::HandleEntitiesSpawn()
{
	for (ListItem<EntitySpawnPoint*>* spawnEntiti = spawnQueue.start; spawnEntiti; spawnEntiti = spawnEntiti->next)
	{
		if (spawnEntiti->data->type != TypeEntity::UNKNOWN)
		{
			// Spawn a new enemy if the screen has reached a spawn position
			//if (spawnEntiti->data->x * SCREEN_SIZE < app->render->camera.x + (app->render->camera.w * SCREEN_SIZE) + SPAWN_MARGIN)
			//{
				LOG("Spawning enemy at %d", spawnEntiti->data->x * SCREEN_SIZE);

				SpawnEnemy(*spawnEntiti->data);
				spawnEntiti->data->type = TypeEntity::UNKNOWN; // Removing the newly spawned enemy from the queue
			//}
		}
	}
}

void EntityManager::SpawnEnemy(const EntitySpawnPoint& info)
{
	Entity* iterateEntity;
	SDL_Texture* tex=nullptr;
	// Find an empty slot in the enemies array
	ListItem<Entity*>* entiti= entities.start;
	switch (info.type)
	{
	case TypeEntity::PLAYER:
		entities.Add(new Player(info.type,{ info.x,info.y }, 1, tex));
		entities.end->data->Start();
		break;

	case TypeEntity::GROUND_ENEMY:
		entities.Add(new Enemy(info.type, { info.x,info.y }, 1, tex));
		entities.end->data->Start();
		//entities.end->data->Awake();
		break;

	case TypeEntity::AIR_ENEMY:
		entities.Add(new Enemy(info.type, { info.x,info.y }, 1, tex));
		entities.end->data->Start();
		break;
	case TypeEntity::HUD:
		entities.Add(new GUI());
		entities.end->data->Start();
		break;
	case TypeEntity::FIREBALL:
		entities.Add(new FireBall());
		entities.end->data->Start();
		break;
	case TypeEntity::COIN:
		entities.Add(new Coins({info.x,info.y}));
		entities.end->data->Start();
	break;
	}
}
	



void EntityManager::HandleEntitiesDespawn()
{
	/*fPoint positionEntity;
	int cameraX= app->render->camera.x;
	for (ListItem<Entity*>* entiti = entities.start; entiti; entiti = entiti->next)
	{
		positionEntity = entiti->data->entityData->position;
		if ((positionEntity.x * SCREEN_SIZE < (cameraX - SPAWN_MARGIN)- SPAWN_MARGIN)
			|| (positionEntity.x * SCREEN_SIZE > (cameraX + WINDOW_W) + SPAWN_MARGIN))
		{
			LOG("DeSpawning enemy at %d", positionEntity.x * SCREEN_SIZE);
			entities.Del(entiti);
		}
	}*/
}

bool EntityManager::LoadState(pugi::xml_node& entities)
{
	bool ret = true;

	return ret;
}
bool EntityManager::SaveState(pugi::xml_node& entitiesNode) const
{

	//pugi::xml_node rootSaveFile= entitiesNode.child("entity");

	//for (ListItem<Entity*>* entiti = entities.start; entiti; entiti = entiti->next)
	//{
	//	//rootSaveFile
	//	rootSaveFile.next_sibling().append_child("entity");

	//}

	return true;
}
