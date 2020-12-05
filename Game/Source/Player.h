#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"

struct PlayerData
{
	fPoint position;
	State state;
	MoveDirection direction;
	Animation* currentAnimation;
	float velocity ;
	bool isJumped;
	bool isJumpedAgain;
	
	SDL_Texture* texture;
	static const int numPoints = 6;
	iPoint pointsCollision[numPoints] = { { 0,0 },{46 , 0},{ 0,-54 },{46 ,-54 }, {0,-26},{46,-26} };
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

	void PlayerControls();

	void Jump();

	void MovePlayer(MoveDirection playerDirection);

	bool PostUpdate();

	bool CleanUp();

	bool CollisionPlayer(iPoint nextPosition);
	bool CollisionJumping(iPoint positionMapPlayer);
	bool CheckVictory(iPoint positionMapPlayer);
	bool CheckGameOver(int level);

	void SetHit();

	void activeCheckpoint(iPoint positionMapPlayer);

	//iPoint TransformFPoint(fPoint fpoint);
	fPoint TransformIPointMapToFPointWorld(iPoint ipoint);

private:
	// Load state game
	bool LoadState(pugi::xml_node& data);
	// Save state game
	bool SaveState(pugi::xml_node& data)const;

	void Fallings(float dt);
	void MoveToDirection(int velocity);

public:

	PlayerData* playerData = new PlayerData();
	bool godMode = false;
	fPoint positionInitial;	
	bool win= false;

private:
	
	float gravity = 9;
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
	fPoint tmp;

	uint bonfireFx;

	//CheckPoint's vars
	List<iPoint> checkPoints;
	List<iPoint> cameraPosCP;
	int lastCP;
	bool checkpointMove;
	bool endUpdate;
};

#endif // _PLAYER_H_


