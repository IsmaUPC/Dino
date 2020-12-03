#include "App.h"
#include "Window.h"
#include "Entity.h"

#include "Defs.h"
#include "Log.h"

#define VSYNC true

Entity::Entity(TypeEntity pTypeEntity, fPoint pPosition, float pVelocity, SDL_Texture* pTexture) : Module()
{
	name.Create("Entity");
	isAlive = true;
	EntityData(pTypeEntity, pPosition, pVelocity, pTexture);

	
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


