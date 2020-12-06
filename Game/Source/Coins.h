#ifndef __COINS_H__
#define __COINS_H__

#include "Entity.h"
#include "PugiXml\src\pugixml.hpp"

class Coins : public Entity
{
public:

	Coins(iPoint pos);

	virtual ~Coins();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();


public:

	bool isCollected = false;
private:
	iPoint position;
	int numPoints;
	iPoint* pointsCollision;

	Animation* currentAnimation = new Animation();

	uint coinFx;

	SDL_Texture* texture;
};
#endif // _COINS_H_

