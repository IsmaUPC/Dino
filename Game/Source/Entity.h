#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Module.h"
#include "Point.h"
#include "Animation.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "Map.h"
#include "Collisions.h"

#include "Defs.h"
#include "Log.h"


enum TypeEntity {

	PLAYER,
	GROUND_ENENMY,
	AIR_ENEMY,
	UNKNOWN,
};


enum State {

	IDLE,
	JUMP,
	WALK,
	RUN,
};

enum MoveDirection {

	WALK_R,
	WALK_L,
};
enum TypeCollision {

	VICTORY,
	COLLISION,
	CHECK_POINT,
};
struct EntityData
{
	fPoint position;
	State state;
	MoveDirection direction;
	Animation* currentAnimation;
	float velocity;
	TypeEntity type;
	SDL_Texture* texture;
};
class Entity : public Module
{

public:

	Entity();
	~Entity();

	// Called before render is available
	virtual bool Awake(pugi::xml_node&);

	// Called before the first frame
	virtual bool Start();
	// Called each loop iteration
	virtual bool PreUpdate();

	// Called each loop iteration
	virtual bool Update(float dt);
	// Called each loop iteration
	virtual bool PostUpdate();
	// Called before quitting
	virtual bool CleanUp();

	// Virtual methods to Load state
	bool LoadState(pugi::xml_node&);
	// Virtual methods to Save state
	bool SaveState(pugi::xml_node&) const;
	//Transform fPoint to iPoint
	iPoint TransformFPoint(fPoint fpoint);
public:


	bool isAlive= false;
	bool pendingToDelete = false;
	List<EntityData>* entityData;
	Collisions collision;

};

#endif // __MODULE_H__