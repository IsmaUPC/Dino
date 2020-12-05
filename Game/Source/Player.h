#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"

struct PlayerData
{
	iPoint position;
	State state;
	MoveDirection direction;
	Animation* currentAnimation;
	float velocity ;
	bool isJumped;
	bool isJumpedAgain;
	
	SDL_Texture* texture;
	static const int numPoints = 6;
	iPoint pointsCollision[numPoints] = { { 0,0 },{46 , 0},{ 46,-26 },{46 ,-54 }, {0, -54},{0, -26} };
};


class Player : public Entity
{
public:

	Player();
	Player(TypeEntity pTypeEntity, iPoint pPosition, float pVelocity, SDL_Texture* pTexture);
	virtual ~Player();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	void PlayerMoveAnimation();

	void PlayerControls(float dt);

	void Jump(float dt);

	void MovePlayer(MoveDirection playerDirection, float dt);

	bool PostUpdate();

	bool CleanUp();

	bool CollisionPlayer(iPoint nextPosition);
	bool CollisionJumping(iPoint positionMapPlayer);
	//bool CheckVictory(iPoint positionMapPlayer);
	bool CheckGameOver(int level);

	void SetHit();

	void activeCheckpoint(iPoint positionMapPlayer);

	//iPoint TransformFPoint(fPoint fpoint);
	iPoint TransformIPointMapToFPointWorld(iPoint ipoint);

private:

	//bool DownY(iPoint Position);
	// Load state game
	bool LoadState(pugi::xml_node& data);
	// Save state game
	bool SaveState(pugi::xml_node& data)const;

	void Fallings(float dt);
	void MoveToDirection(int velocity);

public:

	PlayerData playerData;
	bool godMode = false;
	iPoint positionInitial;	
	bool win= false;

private:
	
	float gravity = 0.5f;
	float velY = 0;

	float velX = 0;
	uint lives = 3;

	Animation* idleAnim= new Animation();
	Animation* walkAnim = new Animation();
	Animation* atakAnim = new Animation();
	Animation* damageAnim = new Animation();
	Animation* runAnim = new Animation( );
	Animation* jumpAnim = new Animation( );

	pugi::xml_document playerFile;
	SString folder;
	iPoint tmp;

	uint bonfireFx;

	//CheckPoint's vars
	List<iPoint> checkPoints;
	List<iPoint> cameraPosCP;
	int lastCP;
	bool checkpointMove;
	bool endUpdate;
};

#endif // _PLAYER_H_


