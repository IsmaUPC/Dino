#ifndef __COINS_H__
#define __COINS_H__

#include "Point.h"
#include "Entity.h"
#include "Animation.h"

#include "PugiXml\src\pugixml.hpp"

struct CoinsData
{
	iPoint position;
	Animation* currentAnimation;

	SDL_Texture* texture;
	//static const int numPoints = 6;
	//int pointsCollision[numPoints][2] = { { 1,0 },{47 , 0},{ 1,-54 },{47 ,-54 }, {1,-47},{47,-47} };
};

class Coins : public Entity
{
public:
	Coins();

	virtual ~Coins();

	bool Awake(pugi::xml_node&);

	bool Start();

	//bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();


public:
	iPoint positionInitial;
	CoinsData coinsData;
private:
	Animation* idleAnim = new Animation();
};
#endif // _COINS_H_

