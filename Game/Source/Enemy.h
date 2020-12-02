#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Point.h"
#include "Module.h"
#include "Animation.h"

#include "PugiXml\src\pugixml.hpp"
enum MoveDirectionEnemy {

	WALK_R_ENEMY,
	WALK_L_ENEMY,
};

struct EnemyData
{
	iPoint position;
	MoveDirectionEnemy direction;
	Animation* currentAnimation;
	int velocity;

	SDL_Texture* texture;
	//static const int numPoints = 6;
	//int pointsCollision[numPoints][2] = { { 1,0 },{47 , 0},{ 1,-54 },{47 ,-54 }, {1,-47},{47,-47} };
};

class Enemy : public Module
{
public:
	Enemy();

	virtual ~Enemy();

	bool Awake(pugi::xml_node&);

	bool Start();

	//bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();


public:
	iPoint positionInitial;
	EnemyData enemyData;
private:
	Animation* walkAnim = new Animation();
};
#endif // _ENEMY_H_
