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

	UNKNOWN,
	PLAYER,
	GROUND_ENEMY,
	AIR_ENEMY,
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
	Animation* currentAnimation = nullptr;
	TypeEntity typeEntity;
	float velocity;
	TypeEntity type;
	SDL_Texture* texture;
public:
	EntityData(TypeEntity pTypeEntity, fPoint pPosition, float pVelocity, SDL_Texture* pTexture) :
		position(pPosition), state(IDLE), direction(WALK_L), velocity(pVelocity),
		texture(pTexture), typeEntity(pTypeEntity)
	{}
	EntityData::EntityData() {}
};

class Entity : public Module
{
public:


	Entity(TypeEntity pTypeEntity, fPoint pPosition, float pVelocity, SDL_Texture* pTexture);
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