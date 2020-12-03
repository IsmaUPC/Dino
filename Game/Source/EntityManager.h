#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__


#include "Module.h"
#include "Entity.h"
#include "Player.h"



struct EntitySpawnPoint
{
	EntitySpawnPoint(TypeEntity pType, float pX, float pY) : type(pType), x(pX), y(pY) {}
	TypeEntity type = TypeEntity::UNKNOWN;
	float x, y;
};

class EntityManager : public Module
{

public:

	EntityManager();
	~EntityManager();

	// Called before render is available
	 bool Awake(pugi::xml_node&);

	// Called before the first frame
	 bool Start();
	// Called each loop iteration
	 bool PreUpdate();

	// Called each loop iteration
	 bool Update(float dt);
	// Called each loop iteration
	 bool PostUpdate();
	// Called before quitting
	 bool CleanUp();

	// Virtual methods to Load state
	bool LoadState(pugi::xml_node&);
	// Virtual methods to Save state
	bool SaveState(pugi::xml_node&) const;
	bool AddEntity(TypeEntity pType, float pX, float pY);
	void HandleEntitiesSpawn();
	void HandleEntitiesDespawn();
	void EntityManager::SpawnEnemy(const EntitySpawnPoint& info);

public:
	
	List<EntitySpawnPoint*> spawnQueue;
	List<Entity*> entities;


	bool isAlive = false;


};

#endif // __MODULE_H__