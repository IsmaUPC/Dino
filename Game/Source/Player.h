#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"



//
//enum State {
//
//	IDLE,
//	JUMP,
//	WALK,
//	RUN,
//};
//
//enum MoveDirection {
//
//	WALK_R,
//	WALK_L,
//};
//enum TypeCollision{
//
//	VICTORY,
//	COLLISION,
//	CHECK_POINT,
//};

struct PlayerData
{
	fPoint position;
	State state;
	MoveDirection direction;
	Animation* currentAnimation;
	float velocity;
	bool isJumped;
	bool isJumpedAgain;
	
	SDL_Texture* texture;
	static const int numPoints = 6;
	int pointsCollision[numPoints][2] = { { 1,0 },{47 , 0},{ 1,-54 },{47 ,-54 }, {1,-47},{47,-47} };
};


class Player : public Entity
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

	bool CollisionPlayer(iPoint nextPosition);
	bool CollisionJumping(iPoint positionMapPlayer);
	int CheckCollision(iPoint positionMapPlayer);
	bool CheckVictory(iPoint positionMapPlayer);
	bool CheckGameOver(int level);

	void activeCheckpoint(iPoint positionMapPlayer);

	iPoint TransformFPoint(fPoint fpoint);
	fPoint TransformIPointMapToFPointWorld(iPoint ipoint);

private:
	// Load state game
	bool LoadState(pugi::xml_node& data);
	// Save state game
	bool SaveState(pugi::xml_node& data)const;

	void Fallings(float dt);
	void MoveToDirection(int velocity);

public:

	PlayerData playerData;
	bool godMode = false;
	fPoint positionInitial;	
	bool win= false;

private:
	
	float gravity = 9;
	float velY = 0;
	float velX = 0;

	Animation* idleAnim= new Animation();
	Animation* walkAnim = new Animation();
	Animation* atakAnim = new Animation();
	Animation* damageAnim = new Animation();
	Animation* runAnim = new Animation( );
	Animation* jumpAnim = new Animation( );

	int velWalk;
	bool isDead = false;

	pugi::xml_document playerFile;
	SString folder;
	fPoint tmp;

	//CheckPoint's vars
	List<iPoint> checkPoints;
	List<iPoint> cameraPosCP;
	int lastCP;
	bool checkpointMove;
	bool endUpdate;
};

#endif // _PLAYER_H_


