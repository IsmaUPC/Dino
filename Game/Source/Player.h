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
	WALK_UP,
	WALK_DOWN,
	JUMP,

};

struct PlayerData
{
	iPoint position;
	State state;
	Animation* currentAnimation;
	int velocity;
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

	void PlayerMoveAnimation();

	void PlayerControls();

	void MovePlayer(State playerState);

	bool PostUpdate();

	bool CleanUp();

	bool Load(const char* path);

	bool CollisionPlayer(iPoint nextPosition);

private:

	bool LoadPlayer();

	void Jump();

public:


	PlayerData playerData;
	
private:

	Animation idleAnim;
	Animation* walkAnim = new Animation();
	Animation* atakAnim = new Animation();
	Animation* damageAnim = new Animation();
	Animation* runAnim = new Animation( );

	float velY=0;
	//int gravity=1;

	pugi::xml_document playerFile;
	SString folder;
	iPoint tmp;
	int nextY;


};

#endif // _PLAYER_H_


