#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Point.h"
#include "Module.h"
#include "Animation.h"


#include "PugiXml\src\pugixml.hpp"


enum State {

	IDLE,
	WALK_L,
	WALK_R,

};

struct PlayerData
{
	iPoint position;
	State state;
	Animation* currentAnimation;
	float velocity;
	bool rightDirection;

	SDL_Texture* texture;
};


class Player : public Module
{
public:

	Player();

	virtual ~Player();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	bool Load(const char* path);


private:

	bool LoadPlayer();

public:


	PlayerData playerData;
	
private:

	Animation idleAnim;
	Animation* walkAnim = new Animation();
	Animation* atakAnim = new Animation();
	Animation* damageAnim = new Animation();
	Animation* runAnim = new Animation( );



	pugi::xml_document playerFile;
	SString folder;


};

#endif // _PLAYER_H_


