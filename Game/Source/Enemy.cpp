#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Enemy.h"

#include "Input.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

Enemy::Enemy() : Module()
{
	name.Create("Enemy");
}

Enemy::~Enemy()
{}

bool Enemy::Start()
{
	iPoint pathInit = app->map->WorldToMap(positionInitial.x, positionInitial.y);
	app->map->ResetPath(pathInit);
	enemyData.texture = app->tex->Load("Assets/textures/EnemigoTerrestrePeque.png");
	enemyData.position = positionInitial;

	return true;
}

bool Enemy::Awake(pugi::xml_node& config)
{
	LOG("Loading Player Parser");
	bool ret = true;

	enemyData.velocity = 1;

	walkAnim->loop = true;
	walkAnim->speed = 0.035f;

	for (int i = 0; i < 3; i++)
		walkAnim->PushBack({ (57 * i),0, 57, 60 });

	enemyData.currentAnimation = walkAnim;
	return ret;
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
	if (enemyData.direction == MoveDirectionEnemy::WALK_R_ENEMY)
		app->render->DrawTexture(enemyData.texture, enemyData.position.x - 15, enemyData.position.y - (rectEnemy.h - 10), &rectEnemy);
	if (enemyData.direction == MoveDirectionEnemy::WALK_L_ENEMY)
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