#ifndef __FIREBALL_H__
#define __FIREBALL_H__

#include "Entity.h"
#include "PugiXml\src\pugixml.hpp"

enum StateShoot {

	CAN_SHOOT,
	SHOOT,
	WAIT,
};

class FireBall : public Entity
{
public:
	FireBall();

	virtual ~FireBall();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void SetPosition(iPoint pos);

	void BackToPos0();

	void Shoot();


	uint stateShoot;
private:

	int direc;

	iPoint startPos;
	Animation* fireBallAnim = new Animation();
	int texW, texH;
	int cooldown;

	Timer frameTime;
};

#endif // __FIREBALL__H__
