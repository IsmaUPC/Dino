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
	enemyData.texture = app->tex->Load("Assets/textures/EnemigoTerrestrePeque.png");
	enemyData.position = positionInitial;

	return true;
}

bool Enemy::Awake(pugi::xml_node& config)
{
	LOG("Loading Enemy Parser");
	bool ret = true;

	enemyData.velocity = 1;

	walkAnim->loop = true;
	walkAnim->speed = 0.04f;

	for (int i = 0; i < 3; i++)
		walkAnim->PushBack({(57 * i),0, 57, 60 });

	enemyData.currentAnimation = walkAnim;
	return ret;
}
bool Enemy::Radar(iPoint distance)
{
	if (enemyData.position.DistanceManhattan(distance) < range) return true;

	return false;
}
bool Enemy::PreUpdate()
{
	iPoint currentPositionPlayer = TransformFPoint(app->player->playerData.position);
	if (Radar(currentPositionPlayer))
	{
		//Pathfinding
		static const int countEnemy = sizeof(enemyData.pointsCollision);
		iPoint auxPositionEnemey[countEnemy];
		for (int i = 0; i < sizeof(enemyData.pointsCollision); i++)
		{
			auxPositionEnemey[i] = { enemyData.position.x + enemyData.pointsCollision[i].x,
				enemyData.position.y + enemyData.pointsCollision[i].y };
		}
		static const int countPlayer = sizeof(app->player->playerData.pointsCollision);
		iPoint auxPositionPlayer[countPlayer];
		for (int i = 0; i < countPlayer; i++)
		{
			auxPositionPlayer[i] = { currentPositionPlayer.x + app->player->playerData.pointsCollision[i].x,
				currentPositionPlayer.y + app->player->playerData.pointsCollision[i].y };
		}
		if (collision.IsInsidePolygons(auxPositionEnemey, auxPositionPlayer))
		{
			pendingToDelete = true;
			app->player->SetHit();
		}
	}
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