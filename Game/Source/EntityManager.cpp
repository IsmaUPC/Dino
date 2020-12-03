#include "EntityManager.h"

#include "Defs.h"
#include "Log.h"

#define VSYNC true

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

	//for (uint i = 0; i < MAX_ENEMIES; ++i)
	//{
	//	if (enemies[i] != nullptr && enemies[i]->pendingToDelete)
	//	{
	//		delete enemies[i];
	//		enemies[i] = nullptr;
	//	}
	//}
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

	return true;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	LOG("Destroying EntityManager");

	return true;
}


// Load Game State
bool EntityManager::LoadState(pugi::xml_node& data)
{


	return true;
}


// Save Game State
bool EntityManager::SaveState(pugi::xml_node& data) const
{



	return true;
}

bool EntityManager::AddEntity(int type, int x, int y)
{
	bool ret = false;

	//for (uint i = 0; i < MAX_ENEMIES; ++i)
	//{
	//	if (spawnQueue[i].type == ENEMY_TYPE::NO_TYPE)
	//	{
	//		spawnQueue[i].type = type;
	//		spawnQueue[i].x = x;
	//		spawnQueue[i].y = y;
	//		ret = true;
	//		break;
	//	}
	//}

	return ret;
}

void EntityManager::HandleEntitiesSpawn()
{


	//// Iterate all the enemies queue
	//for (uint i = 0; i < MAX_ENEMIES; ++i)
	//{
	//	if (spawnQueue[i].type != ENEMY_TYPE::NO_TYPE)
	//	{
	//		// Spawn a new enemy if the screen has reached a spawn position
	//		if (spawnQueue[i].x * SCREEN_SIZE < App->render->camera.x + (App->render->camera.w * SCREEN_SIZE) + SPAWN_MARGIN)
	//		{
	//			LOG("Spawning enemy at %d", spawnQueue[i].x * SCREEN_SIZE);

	//			SpawnEnemy(spawnQueue[i]);
	//			spawnQueue[i].type = ENEMY_TYPE::NO_TYPE; // Removing the newly spawned enemy from the queue
	//		}
	//	}
	//}
}
void EntityManager::HandleEntitiesDespawn()
{
	/*fPoint positionEntity;
	int cameraX= app->render->camera.x;
	for (ListItem<Entity*>* entiti = entities.start; entiti; entiti = entiti->next)
	{
		positionEntity = entiti->data->entityData.position;
		if ((positionEntity.x * SCREEN_SIZE < (cameraX - SPAWN_MARGIN)- SPAWN_MARGIN)
			|| (positionEntity.x * SCREEN_SIZE > (cameraX + WINDOW_W) + SPAWN_MARGIN))
		{
			LOG("DeSpawning enemy at %d", positionEntity.x * SCREEN_SIZE);
			entities.Del(entiti);
		}
	}*/
}
