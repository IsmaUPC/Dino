#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Point.h"
#include "Entity.h"
#include "Animation.h"

#include "PugiXml\src\pugixml.hpp"

struct EnemyData
{
	iPoint position;
	MoveDirection direction;
	Animation* currentAnimation;
	int velocity;

	SDL_Texture* texture;
	static const int numPoints = 4;
	iPoint pointsCollision[numPoints] = {{ 0,0 },{48 , 0},{ 48,-48 },{0 ,-48 }};
};

class Enemy : public Entity
{
public:
	Enemy();

	virtual ~Enemy();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool Radar(iPoint distance);

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();


public:
	iPoint positionInitial;
	EnemyData enemyData;
private:
	Animation* idleAnim = new Animation();
	Animation* walkAnim = new Animation();
	Animation* deadAnim = new Animation();
	int range = 500;
};
#endif // _ENEMY_H_
