#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__


#include "Module.h"
#include "Entity.h"



struct EntitySpawnPoint
{
	TypeEntity type = TypeEntity::UNKNOWN;
	int x, y;
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
	bool AddEntity(int type, int x, int y);
	void HandleEntitiesSpawn();
	void HandleEntitiesDespawn();

public:
	
	List<EntitySpawnPoint*> spawnQueue;
	List<Entity*> entities;


	bool isAlive = false;


};

#endif // __MODULE_H__