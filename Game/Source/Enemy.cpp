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
	lastPath = new DynArray<iPoint>();
	name.Create("Enemy");
}


Enemy::~Enemy()
{}

bool Enemy::Start()
{
	//iPoint pathInit = app->map->WorldToMap(positionInitial.x, positionInitial.y);
	//app->map->ResetPath(pathInit);
	entityData->texture = app->tex->Load("Assets/textures/Enemy_Walk.png");

	entityData->velocity = 1;

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
	
	checkDestination->Start();

	entityData->currentAnimation = idleAnim;

	if (entityData->type == GROUND_ENEMY)
	{
		entityData->numPoints = 4;
		entityData->pointsCollision = new iPoint[]{ { 0, 0 }, { 48 , 0 }, { 48,-48 }, { 0 ,-48 } };
	}
	if (entityData->type == AIR_ENEMY)
	{
		entityData->numPoints = 4;
		entityData->pointsCollision = new iPoint[]{ { 0, 0 }, { 48 , 0 }, { 48,-48 }, { 0 ,-48 } };
	}
	iPoint worldPositionPalyer = TransformFPoint(app->player->playerData->position);
	destination=app->map->WorldToMap(worldPositionPalyer.x, worldPositionPalyer.y);
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
bool Enemy::PreUpdate()
{
	//app->pathfinding->ComputePathAStar();
	iPoint currentPositionPlayer = TransformFPoint(app->player->playerData->position);
	if (Radar(currentPositionPlayer) && entityData->state != DEADING)
	{
		entityData->state = WALK;
		entityData->currentAnimation = walkAnim;

		iPoint auxPositionEnemey[4];
		for (int i = 0; i < 4; i++)
		{
			auxPositionEnemey[i] = { entityData->position.x + entityData->pointsCollision[i].x,
				entityData->position.y + entityData->pointsCollision[i].y };
		}
		iPoint auxPositionPlayer[6];
		for (int i = 0; i < 6; i++)
		{
			auxPositionPlayer[i] = {currentPositionPlayer.x + app->player->playerData->pointsCollision[i].x,
				14 + currentPositionPlayer.y + app->player->playerData->pointsCollision[i].y };
		}
		if (collision.IsInsidePolygons(auxPositionEnemey, entityData->numPoints, auxPositionPlayer, app->player->playerData->numPoints))
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
	entityData->currentAnimation->Update();
	if (entityData->state == WALK)
	{
		//Direction
		if (entityData->position.x < app->player->playerData->position.x)entityData->direction = WALK_L;
		else entityData->direction = WALK_R;
		//If player move
		iPoint mapPositionEnemy = app->map->WorldToMap(entityData->position.x, entityData->position.y);
		iPoint worldPositionPalyer = TransformFPoint(app->player->playerData->position);
		iPoint mapPositionPalyer = app->map->WorldToMap(worldPositionPalyer.x, worldPositionPalyer.y);
		if (checkDestination->check(1000))
		{
			if (destination != mapPositionPalyer)
			{
				app->pathfinding->ResetPath(mapPositionEnemy);
				checkDestination->Start();
				app->pathfinding->ComputePathAStar(mapPositionEnemy, mapPositionPalyer);
				lastPath = app->pathfinding->GetLastPath();
				destination = mapPositionPalyer;
			}
		}
		//Move Enemy
		int i;
		for (i = 0; i < lastPath->Count(); i++)
		{
			if (mapPositionEnemy == iPoint({ lastPath->At(i)->x, lastPath->At(i)->y })) break;
		}
		if (lastPath->At(i + 1) != NULL)
		{
			if (lastPath->At(i + 1)->x < mapPositionEnemy.x && CheckCollision({mapPositionEnemy.x, mapPositionEnemy.y+1})==1)
			{
				entityData->position.x -= entityData->velocity;
			}
			else if (lastPath->At(i + 1)->x > mapPositionEnemy.x && CheckCollision({ mapPositionEnemy.x+1, mapPositionEnemy.y + 1 })==1)
			{
				entityData->position.x += entityData->velocity;
			}
			if (lastPath->At(i + 1)->y > mapPositionEnemy.y)
			{
				entityData->position.y += entityData->velocity;
			}
		}
		else //if the next position is destination continue with current direction
		{
			if(entityData->direction == WALK_L) entityData->position.x += entityData->velocity;
			else entityData->position.x -= entityData->velocity;
		}
		if (entityData->type == AIR_ENEMY)
		{
			if (lastPath->At(i + 1)->x < mapPositionEnemy.x)
			{
				entityData->position.x -= entityData->velocity;
			}
			else if (lastPath->At(i + 1)->x > mapPositionEnemy.x)
			{
				entityData->position.x += entityData->velocity;
			}
			if (lastPath->At(i + 1)->x < mapPositionEnemy.x)
			{
				entityData->position.y -= entityData->velocity;
			}
			else if (lastPath->At(i + 1)->x > mapPositionEnemy.x)
			{
				entityData->position.y += entityData->velocity;
			}
		}
	}

	return true;
}
bool Enemy::PostUpdate()
{

	SDL_Rect rectEnemy;
	rectEnemy = entityData->currentAnimation->GetCurrentFrame();
	// Draw player in correct direction
	if (entityData->direction == MoveDirection::WALK_R)
		app->render->DrawTexture(entityData->texture, entityData->position.x, entityData->position.y - (rectEnemy.h - 10), &rectEnemy);
	if (entityData->direction == MoveDirection::WALK_L)
		app->render->DrawTextureFlip(entityData->texture, entityData->position.x - (rectEnemy.w - idleAnim->frames->w), entityData->position.y - (rectEnemy.h - 10), &rectEnemy);

	return true;
}

bool Enemy::CleanUp()
{

	if (!active)
		return true;

	app->tex->UnLoad(entityData->texture);
	active = false;

	return true;
}