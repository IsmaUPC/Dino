#include "EntityManager.h"
#include "SceneManager.h"
#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entity_manager");

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
	active = true;
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
	if (!app->sceneManager->GetIsPause())
	{
		HandleEntitiesSpawn();
		for (ListItem<Entity*>* entiti = entities.start; entiti; entiti = entiti->next) entiti->data->Update(dt);
		HandleEntitiesDespawn();
	}

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
	item = entities.start;

	while (item != NULL && ret == true)
	{
		if (item->data->active == true)
			ret = item->data->CleanUp();

		item = item->next;
	}
	active = false;
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
bool EntityManager::AddEntity(TypeEntity pType, int pX, int pY,int num)
{
	
	if (spawnQueue.Add(new EntitySpawnPoint(pType, pX, pY))) return true;
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
		entities.Add(new GUI(info.type, { info.x,info.y }, 1, tex));
		entities.end->data->Start();
		break;
	case TypeEntity::FIREBALL:
		entities.Add(new FireBall(info.type, { info.x,info.y }, 1, tex));
		entities.end->data->Start();
		break;
	case TypeEntity::COIN:
		entities.Add(new Coins(info.type, { info.x,info.y }, 1, tex));
		entities.end->data->Start();
		break;
	case TypeEntity::LIVE:
		entities.Add(new Lives(info.type, { info.x,info.y }, 1, tex));
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

bool EntityManager::LoadState(pugi::xml_node& entityManagerNode)
{
	bool ret = true;
	pugi::xml_node entitiesNode = entityManagerNode.child("entities").first_child();
	pugi::xml_node aux;

	if (entitiesNode != NULL)
	{
		for (ListItem<Entity*>* entiti = entities.start; entiti; entiti = entiti->next)
		{
			entiti->data->pendingToDelete = true;
		}

		entityManagerNode.next_sibling();
		while (entitiesNode)
		{
			AddEntity((TypeEntity)entitiesNode.attribute("type").as_int(), entitiesNode.attribute("x").as_int(), entitiesNode.attribute("y").as_int(), 0);
			entitiesNode = entitiesNode.next_sibling();
		}
	}
	return ret;
}
bool EntityManager::SaveState(pugi::xml_node& entityManagerNode) const
{
	
	entityManagerNode.remove_child("entities");
	entityManagerNode.append_child("entities").set_value(0);

	pugi::xml_node entitiesNode= entityManagerNode.child("entities");
	pugi::xml_node entity_node = entitiesNode;

	for (ListItem<Entity*>* entiti = entities.start; entiti; entiti = entiti->next)
	{
		entitiesNode.append_child("entity").append_attribute("type").set_value(entiti->data->entityData->type);
		entitiesNode.last_child().append_attribute("x").set_value(entiti->data->entityData->position.x);
		entitiesNode.last_child().append_attribute("y").set_value(entiti->data->entityData->position.y);
		entitiesNode.last_child().append_attribute("state").set_value(entiti->data->entityData->state);
	}

	return true;
}
