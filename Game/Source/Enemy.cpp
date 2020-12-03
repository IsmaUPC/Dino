#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Enemy.h"
#include "Player.h"
#include "Input.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

Enemy::Enemy() : Entity()
{
	name.Create("Enemy");
}

Enemy::~Enemy()
{}

bool Enemy::Start()
{
	//iPoint pathInit = app->map->WorldToMap(positionInitial.x, positionInitial.y);
	//app->map->ResetPath(pathInit);
	enemyData.texture = app->tex->Load("Assets/textures/Enemy_Walk.png");
	enemyData.position = positionInitial;

	return true;
}

bool Enemy::Awake(pugi::xml_node& config)
{
	LOG("Loading Enemy Parser");
	bool ret = true;

	enemyData.velocity = 1;

	idleAnim->loop = true;
	idleAnim->speed = 0.04f;

	for (int i = 0; i < 8; i++)
		idleAnim->PushBack({ (48 * i),0, 48, 48 });

	walkAnim->loop = true;
	walkAnim->speed = 0.04f;

	for (int i = 0; i < 4; i++)
		walkAnim->PushBack({(48 * i),48, 48, 48 });

	deadAnim->loop = true;
	deadAnim->speed = 0.04f;

	for (int i = 0; i < 2; i++)
		deadAnim->PushBack({ 192+(67 * i),48, 67, 48 });


	enemyData.currentAnimation = idleAnim;
	return ret;
}
bool Enemy::Radar(iPoint distance)
{
	if (enemyData.position.DistanceManhattan(distance) < range) return true;

	return false;
}
bool Enemy::PreUpdate()
{
	iPoint currentPositionPlayer = TransformFPoint(app->player->playerData->position);
	if (Radar(currentPositionPlayer))
	{
		enemyData.currentAnimation = walkAnim; 
		//state=::WALK;
		//Pathfinding
		iPoint auxPositionEnemey[4];
		for (int i = 0; i < 4; i++)
		{
			auxPositionEnemey[i] = { enemyData.position.x + enemyData.pointsCollision[i].x,
				enemyData.position.y + enemyData.pointsCollision[i].y };
		}
		iPoint auxPositionPlayer[6];
		for (int i = 0; i < 6; i++)
		{
			auxPositionPlayer[i] = { 15+currentPositionPlayer.x + app->player->playerData->pointsCollision[i].x,
				14+currentPositionPlayer.y + app->player->playerData->pointsCollision[i].y };
		}
		if (collision.IsInsidePolygons(auxPositionEnemey,enemyData.numPoints, auxPositionPlayer, app->player->playerData->numPoints))
		{
			enemyData.currentAnimation = deadAnim;
			pendingToDelete = true;
			app->player->SetHit();
		}
	}
	//else state::IDLE;
	return true;
}
bool Enemy::Update(float dt)
{
	enemyData.currentAnimation->Update();

	return true;
}
bool Enemy::PostUpdate()
{

	SDL_Rect rectEnemy;
	rectEnemy = enemyData.currentAnimation->GetCurrentFrame();
	// Draw player in correct direction
	if (enemyData.direction == MoveDirection::WALK_R)
		app->render->DrawTexture(enemyData.texture, enemyData.position.x - 15, enemyData.position.y - (rectEnemy.h - 10), &rectEnemy);
	if (enemyData.direction == MoveDirection::WALK_L)
		app->render->DrawTextureFlip(enemyData.texture, enemyData.position.x - 15, enemyData.position.y - (rectEnemy.h - 10), &rectEnemy);

	return true;
}

bool Enemy::CleanUp()
{

	if (!active)
		return true;

	app->tex->UnLoad(enemyData.texture);
	active = false;

	return true;
}