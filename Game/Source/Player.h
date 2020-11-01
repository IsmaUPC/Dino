#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Point.h"
#include "Module.h"
#include "Animation.h"


#include "PugiXml\src\pugixml.hpp"


enum State {

	IDLE,
	JUMP,
	WALK,
	RUN,
};

enum MoveDirection {

	WALK_R,
	WALK_L,
};

struct PlayerData
{
	iPoint position;
	State state;
	MoveDirection direction;
	Animation* currentAnimation;
	int velocity;
	bool isJumped;
	bool isJumpedAgain;
	
	SDL_Texture* texture;
	static const int numPoints = 6;
	int pointsCollision[numPoints][2] = { { 1,0 },{47 , 0},{ 1,-54 },{47 ,-54 }, {1,-47},{47,-47} };
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

	void Jump();

	void MovePlayer(MoveDirection playerDirection);

	bool PostUpdate();

	bool CleanUp();

	bool Load(const char* path);

	/*bool LoadState(pugi::xml_node& data);
	bool SaveState(pugi::xml_node& data) const;*/

	bool CollisionPlayer(iPoint nextPosition);
	bool CollisionJumping(iPoint positionMapPlayer);
	bool CheckCollision(iPoint positionMapPlayer);
	bool CheckVictory(iPoint positionMapPlayer);
	bool CheckGameOver();

private:

	bool LoadPlayer();
	void Fallings();
	void MoveToDirection(int velocity);

public:

	PlayerData playerData;
	bool godMode = false;
	iPoint positionInitial = {432,1170};
private:
	float velY = 0;
	float velX = 0;

	Animation idleAnim;
	Animation* walkAnim = new Animation();
	Animation* atakAnim = new Animation();
	Animation* damageAnim = new Animation();
	Animation* runAnim = new Animation( );
	Animation* jumpAnim = new Animation( );

	int velWalk;
	bool isDead = false;

	pugi::xml_document playerFile;
	SString folder;
	iPoint tmp;
	int nextYDown;
	int nextYUp;

};

#endif // _PLAYER_H_


