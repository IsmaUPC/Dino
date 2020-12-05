#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "PugiXml\src\pugixml.hpp"

class Enemy : public Entity
{
public:
	Enemy();
	Enemy(TypeEntity pTypeEntity, iPoint pPosition, float pVelocity, SDL_Texture* pTexture);

	virtual ~Enemy();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool Radar(iPoint distance);

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

private:
	Animation* idleAnim = new Animation();
	Animation* walkAnim = new Animation();
	Animation* deadAnim = new Animation();
	int range = 400;
	bool isDetected = false;
	//Timer
	Timer* checkDestination = new Timer();
	iPoint destination;
	//We store the created path here
	DynArray<iPoint>* lastPath;
};
#endif // _ENEMY_H_
