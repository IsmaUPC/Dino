#ifndef __LIVES_H__
#define __LIVES_H__

#include "Entity.h"
#include "PugiXml\src\pugixml.hpp"

class Lives : public Entity
{
public:

	Lives(iPoint pos);
	Lives(TypeEntity pTypeEntity, iPoint pPosition, float pVelocity, SDL_Texture* pTexture);

	virtual ~Lives();

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

	uint liveFx;

	SDL_Texture* texture;
};
#endif // _LIVES_H_

