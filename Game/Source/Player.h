#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Space4HD.h"

struct PlayerData : public PhysicsEngine
{
	//iPoint position;
	State state;
	MoveDirection direction;
	Animation* currentAnimation;
	//float velocity;
	bool isJumped;
	bool isJumpedAgain;
	uint lives = 0;
	uint respawns = 0;
	uint coins = 0;
	uint *stateShoot = 0;
	iPoint* shootPosition;
	iPoint* shootPointsCollision;

	SDL_Texture* texture;
	static const int numPoints = 5;
	
	fPoint pointsCollision[numPoints] = { { 0,0 }, {24,0}, {48 , 0},{48 ,-54 }, {0, -54} };
	//fPoint pointsCollision[numPoints] = { { 0,54 }, {23,54}, {46 , 54},{46 ,-54 }, {-46, -54} };

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

	void MoveHit();

	void GravityDown(float dt);

	void SpeedAnimationCheck(float dt);

	void MoveBetweenCheckPoints();

	void CameraPlayer();

	void PlayerMoveAnimation();

	void PlayerControls(float dt);

	void Jump();

	void MovePlayer(MoveDirection playerDirection, float dt);

	bool PostUpdate();

	bool CleanUp();

	bool CollisionPlayer(fPoint nextPosition);
	bool CollisionJumping(iPoint positionMapPlayer);
	//bool CheckVictory(iPoint positionMapPlayer);
	bool CheckGameOver(int level);

	void SetHit();

	void CoinPlus() { playerData.coins++; };

	void ActiveCheckpoint(iPoint positionMapPlayer);

	//iPoint TransformFPoint(fPoint fpoint);
	iPoint IPointMapToWorld(iPoint ipoint);


	bool GetInCheckPoint() { return inCheckPoint; };
	bool GetCheckPointMove() { return checkpointMove; };

	void SetStateShoot(uint *state) { playerData.stateShoot = state; };
	void SetPositionShoot(iPoint* position) { playerData.shootPosition = position; };
	void SetCollidersShoot(iPoint* colliders) { playerData.shootPointsCollision = colliders; };

private:

	//bool DownY(iPoint Position);
	// Load state game
	bool LoadState(pugi::xml_node& data);
	// Save state game
	bool SaveState(pugi::xml_node& data)const;

	void GravityDownCollision(float dt);
	void MoveToDirection(int velocity);

public:
	//PhysicsEngine playerData;
	PlayerData playerData;
	bool godMode = false;
	fPoint positionInitial;	
	bool win= false;

private:
	
	int levelScene;

	float gravity = 0.3f;
	float velY = 0;
	float velX = 0;

	Animation* idleAnim= new Animation();
	/*Animation* walkAnim = new Animation();
	Animation* atakAnim = new Animation();
	Animation* damageAnim = new Animation();
	Animation* runAnim = new Animation( );
	Animation* jumpAnim = new Animation( );*/

	pugi::xml_document playerFile;
	SString folder;
	fPoint tmp;

	uint bonfireFx;

	//CheckPoint's vars
	bool inCheckPoint;
	List<iPoint> checkPoints;
	List<iPoint> cameraPosCP;
	int lastCP;
	bool checkpointMove;
	bool endUpdate;
	bool jumpHit;

	State lastState;
	MoveDirection hitDirection;
};

#endif // _PLAYER_H_


