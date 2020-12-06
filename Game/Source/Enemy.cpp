#include "Enemy.h"
#include "Player.h"
#include "Pathfinding.h"

#include "Defs.h"
#include "Log.h"

Enemy::Enemy() : Entity()
{
	name.Create("Enemy");
}

Enemy::Enemy(TypeEntity pTypeEntity, iPoint pPosition, float pVelocity, SDL_Texture* pTexture)
	: Entity(pTypeEntity, pPosition, pVelocity, pTexture)
{
	positionInitial = pPosition;
	lastPath = new DynArray<iPoint>();
	name.Create("Enemy");
}


Enemy::~Enemy()
{}

bool Enemy::Start()
{
	active = true;
	if (entityData->type == GROUND_ENEMY)
	{
		entityData->texture = app->tex->Load("Assets/Textures/enemy_walk.png");

		idleAnim->loop = true;
		idleAnim->speed = 0.04f;

		for (int i = 0; i < 8; i++)
			idleAnim->PushBack({ (48 * i),0, 48, 48 });

		walkAnim->loop = true;
		walkAnim->speed = 0.04f;

		for (int i = 0; i < 4; i++)
			walkAnim->PushBack({ (48 * i),48, 48, 48 });

		deadAnim->loop = false;
		deadAnim->speed = 0.08f;

		for (int j = 0; j < 3; j++)
		{
			for (int i = 0; i < 2; i++)
				deadAnim->PushBack({ 192 + (67 * i),48, 67, 48 });
		}
	}
	else if (entityData->type == AIR_ENEMY)
	{
		entityData->texture = app->tex->Load("Assets/Textures/enemy_fly.png");

		idleAnim->loop = true;
		idleAnim->speed = 0.04f;

		for (int i = 0; i < 7; i++)
			idleAnim->PushBack({ (83 * i),0, 83, 45 });

		isDetectedAnim->loop = false;
		isDetectedAnim->speed = 0.25f;

		for (int i = 0; i < 7; i++)
			isDetectedAnim->PushBack({ (83 * i),45, 83, 50 });
		
		
		walkAnim->loop = true;
		walkAnim->speed = 0.2f;

		for (int i = 0; i < 7; i++)
			walkAnim->PushBack({ (83 * i), 95, 83, 47 });

		deadAnim->loop = false;
		deadAnim->speed = 0.09f;


		for (int i = 0; i < 5; i++)
			deadAnim->PushBack({ (83 * i),142, 83, 50 });
	}

	entityData->velocity = 1;

	checkDestination->Start();

	entityData->currentAnimation = idleAnim;

	if (entityData->type == GROUND_ENEMY)
	{
		entityData->numPoints = 4;
		entityData->pointsCollision = new iPoint[4]{ { 0, 0 }, { 48 , 0 }, { 48,-48 }, { 0 ,-48 } };
	}
	else if (entityData->type == AIR_ENEMY)
	{
		entityData->numPoints = 4;
		entityData->pointsCollision = new iPoint[4]{ { 0, 0 }, { 83 , 0 }, { 83,-47 }, { 0 ,-47 } };
	}

	destination = app->player->playerData.position;
	return true;
}

bool Enemy::Awake(pugi::xml_node& config)
{
	LOG("Loading Enemy Parser");
	bool ret = true;
	
	return ret;
}
bool Enemy::Radar(iPoint distance)
{
	if (entityData->position.DistanceManhattan(distance) < range) return true;

	return false;
}
int Enemy::CalculateDistance(iPoint origin, iPoint destination)
{
	return abs(origin.x - destination.x) + abs(origin.y - destination.y);;
}
void Enemy::CreatePathEnemy(iPoint mapPositionEnemy, iPoint mapPositionDestination)
{
	if (checkDestination->check(1000))
	{
		//if (destination != mapPositionDestination)//player if move
		//{
			app->pathfinding->ResetPath(mapPositionEnemy);
			checkDestination->Start();
			app->pathfinding->ComputePathAStar(mapPositionEnemy, mapPositionDestination);
			lastPath = app->pathfinding->GetLastPath();
			//destination = mapPositionDestination;
		//}
	}
}
int Enemy::GetCurrentPositionInPath(iPoint mapPositionEnemy)
{
	int i;
	for (i = 0; i < lastPath->Count(); i++)
	{
		if (mapPositionEnemy == iPoint({ lastPath->At(i)->x, lastPath->At(i)->y })) break;
	}
	return i;
}
void Enemy::MoveEnemy(iPoint nextAuxPositionEenemy, iPoint mapPositionEnemy, TypeEntity type)
{
	int positionEnemyX = entityData->position.x;
	int positionEnemyY = entityData->position.y;
	int velocity = entityData->velocity;;
	if (type == GROUND_ENEMY)
	{
		if (nextAuxPositionEenemy.x < positionEnemyX && (CheckCollision({ mapPositionEnemy.x, mapPositionEnemy.y + 1 }) == 1 || CheckCollision({ mapPositionEnemy.x, mapPositionEnemy.y + 2 }) == 1))
		{
			entityData->position.x -= velocity;
		}
		else if (nextAuxPositionEenemy.x > positionEnemyX && (CheckCollision({ mapPositionEnemy.x + 1, mapPositionEnemy.y + 1 }) == 1 || CheckCollision({ mapPositionEnemy.x + 1, mapPositionEnemy.y + 2 }) == 1))
		{
			entityData->position.x += velocity;
		}
		if (nextAuxPositionEenemy.y > positionEnemyY && (CheckCollision({ mapPositionEnemy.x, mapPositionEnemy.y + 1 }) == 1 || CheckCollision({ mapPositionEnemy.x, mapPositionEnemy.y + 2 }) == 1))
		{
			entityData->position.y += velocity + 1;
		}
	}
	else if (type == AIR_ENEMY)
	{
		if (nextAuxPositionEenemy.x < positionEnemyX)
		{
			entityData->position.x -= velocity;
		}
		else if (nextAuxPositionEenemy.x > positionEnemyX)
		{
			entityData->position.x += velocity;
		}
		if (nextAuxPositionEenemy.y < positionEnemyY)
		{
			entityData->position.y -= velocity;
		}
		else if (nextAuxPositionEenemy.y > positionEnemyY)
		{
			entityData->position.y += velocity;
		}
	}
}
void Enemy::MoveEnemyNULL(iPoint mapPositionEnemy)
{
	if (entityData->type == GROUND_ENEMY) //if the next position is destination continue with current direction
	{
		if (entityData->direction == WALK_L && (CheckCollision({ mapPositionEnemy.x, mapPositionEnemy.y + 1 }) == 1 || CheckCollision({ mapPositionEnemy.x, mapPositionEnemy.y + 2 }) == 1))
			entityData->position.x -= entityData->velocity;
		else if (entityData->direction == WALK_R && (CheckCollision({ mapPositionEnemy.x + 1, mapPositionEnemy.y + 1 }) == 1 || CheckCollision({ mapPositionEnemy.x + 1, mapPositionEnemy.y + 2 }) == 1))
			entityData->position.x += entityData->velocity;

	}
	else if (entityData->type == AIR_ENEMY)
	{
		if (entityData->direction == WALK_L)
			entityData->position.x -= entityData->velocity;
		else if (entityData->direction == WALK_R)
			entityData->position.x += entityData->velocity;
	}
}
bool Enemy::PreUpdate()
{
	iPoint currentPositionPlayer = app->player->playerData.position;
	if (Radar(currentPositionPlayer) && entityData->state != DEADING)
	{
		entityData->state = WALK;
		if (entityData->type == AIR_ENEMY && isDetected == false)
		{
			entityData->currentAnimation = isDetectedAnim;
		}
		if (isDetected == false)app->pathfinding->ResetPath(app->map->WorldToMap(entityData->position.x, entityData->position.y)), isDetected = true;
		if (entityData->type == GROUND_ENEMY)entityData->currentAnimation = walkAnim;
		else if (entityData->currentAnimation->HasFinished())
		{
			entityData->currentAnimation = walkAnim;
		}
		iPoint auxPositionEnemey[4];
		for (int i = 0; i < 4; i++)
		{
			auxPositionEnemey[i] = { entityData->position.x + entityData->pointsCollision[i].x,
				entityData->position.y + entityData->pointsCollision[i].y };
		}
		iPoint auxPositionPlayer[6];
		for (int i = 0; i < 6; i++)
		{
			auxPositionPlayer[i] = { currentPositionPlayer.x + app->player->playerData.pointsCollision[i].x,
				-48 + currentPositionPlayer.y + app->player->playerData.pointsCollision[i].y };

		}
		if (collision.IsInsidePolygons(auxPositionPlayer, app->player->playerData.numPoints, auxPositionEnemey, entityData->numPoints) && collision.IsInsidePolygons(auxPositionEnemey, entityData->numPoints, auxPositionPlayer, app->player->playerData.numPoints))
		{
			entityData->state = DEADING;
			entityData->currentAnimation = deadAnim;
			app->player->SetHit();
		}
	}
	else if (entityData->state != DEADING)entityData->state = IDLE, entityData->currentAnimation = idleAnim, isDetected = false;
	if (entityData->state == DEADING && entityData->currentAnimation->HasFinished())pendingToDelete = true, entityData->state = DEAD;
	return true;
}
bool Enemy::Update(float dt)
{
	//entityData->type == AIR_ENEMY && !Radar(app->player->playerData.position)
	//if (Radar(app->player->playerData.position)) returning = false;
	//entityData->state == WALK && Radar(app->player->playerData.position)
	//CalculateDistance(app->player->playerData.position, positionInitial) < range
	if (entityData->state == WALK && Radar(app->player->playerData.position))
	{
		//Direction
		if (entityData->position.x < app->player->playerData.position.x)entityData->direction = WALK_R;
		else entityData->direction = WALK_L;
		//If player move
		iPoint mapPositionEnemy = app->map->WorldToMap(entityData->position.x, entityData->position.y);
		iPoint worldPositionPalyer = app->player->playerData.position;
		iPoint mapPositionPalyer = app->map->WorldToMap(worldPositionPalyer.x, worldPositionPalyer.y);

		//Cerate Path
		CreatePathEnemy(mapPositionEnemy, mapPositionPalyer);
		int i = GetCurrentPositionInPath(mapPositionEnemy);

		//Move Enemy
		if (lastPath->At(i + 1) != NULL)
		{
			iPoint nextPositionEnemy = *lastPath->At(i + 1);
			iPoint nextAuxPositionEenemy = MapToWorld(nextPositionEnemy);
			MoveEnemy(nextAuxPositionEenemy, mapPositionEnemy, entityData->type);
		}
	}
	else if (entityData->type == AIR_ENEMY && CalculateDistance(app->player->playerData.position, positionInitial) > range) // if position enemy is diferent of init position, return to position initial
	{
		//Direction
		if (entityData->position.x < positionInitial.x)entityData->direction = WALK_R;
		else entityData->direction = WALK_L;

		//if(entityData->state != RETURN)entityData->state = RETURNING;
		iPoint mapPositionEnemy = app->map->WorldToMap(entityData->position.x, entityData->position.y);
		iPoint mapPositionInitial = app->map->WorldToMap(positionInitial.x, positionInitial.y);
		if (entityData->position != positionInitial)
		{
			entityData->currentAnimation = walkAnim;
			//if (returning == false)
			//{
			CreatePathEnemy(mapPositionEnemy, mapPositionInitial);
			//returning = true;
		//}
			int i = GetCurrentPositionInPath(mapPositionEnemy);
			if (lastPath->At(i + 1) != NULL)
			{
				iPoint nextPositionEnemy = *lastPath->At(i + 1);
				iPoint nextAuxPositionEenemy = MapToWorld(nextPositionEnemy);
				MoveEnemy(nextAuxPositionEenemy, mapPositionEnemy, entityData->type);
			}
		}
	}
	entityData->currentAnimation->Update();
	return true;
}
bool Enemy::PostUpdate()
{

	SDL_Rect rectEnemy;
	rectEnemy = entityData->currentAnimation->GetCurrentFrame();
	// Draw player in correct direction
	if (entityData->direction == MoveDirection::WALK_L)
		app->render->DrawTexture(entityData->texture, entityData->position.x, entityData->position.y, &rectEnemy);
	if (entityData->direction == MoveDirection::WALK_R)
		app->render->DrawTextureFlip(entityData->texture, entityData->position.x - (rectEnemy.w - idleAnim->frames->w), entityData->position.y, &rectEnemy);

	return true;
}

bool Enemy::CleanUp()
{

	if (!active)
		return true;
	pendingToDelete = true;
	app->tex->UnLoad(entityData->texture);
	active = false;

	return true;
}
