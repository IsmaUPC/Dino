#include "App.h"
#include "Coins.h"
#include "Player.h"



Coins::Coins(iPoint pos) : Entity()
{
	name.Create("Enemy");

	position = pos;
}

Coins::~Coins()
{}

bool Coins::Start()
{
	//iPoint pathInit = app->map->WorldToMap(positionInitial.x, positionInitial.y);
	//app->map->ResetPath(pathInit);
	texture = app->tex->Load("Assets/Textures/coin.png");
	
	numPoints = 4;
	pointsCollision = new iPoint[4]{ { 0, 0 }, { 48 , 0 }, { 48,-48 }, { 0 ,-48 } };
	

	currentAnimation->loop = true;
	currentAnimation->speed = 0.06f;

	for (int i = 0; i < 3; i++)
		currentAnimation->PushBack({ (40 * i),0, 40, 40 });


	return true;
}

bool Coins::Awake(pugi::xml_node& config)
{
	LOG("Loading Coins Parser");
	bool ret = true;


	return ret;
}

bool Coins::PreUpdate()
{
	iPoint currentPositionPlayer = app->player->playerData.position;
	iPoint auxPositionCoin[4];
	for (int i = 0; i < 4; i++)
	{
		auxPositionCoin[i] = { position.x + pointsCollision[i].x,
			position.y + pointsCollision[i].y };
	}
	iPoint auxPositionPlayer[6];
	for (int i = 0; i < 6; i++)
	{
		auxPositionPlayer[i] = { currentPositionPlayer.x + app->player->playerData.pointsCollision[i].x,
			-48 + currentPositionPlayer.y + app->player->playerData.pointsCollision[i].y };

	}
	if (collision.IsInsidePolygons(auxPositionPlayer, app->player->playerData.numPoints, auxPositionCoin, numPoints) 
		&& collision.IsInsidePolygons(auxPositionCoin, numPoints, auxPositionPlayer, app->player->playerData.numPoints))
	{
		app->player->CoinPlus();
		isCollected = true;
		pendingToDelete = true;
	}
	return false;
}

bool Coins::Update(float dt)
{
	currentAnimation->Update();
	return true;
}
bool Coins::PostUpdate()
{

	SDL_Rect rectCoins;
	rectCoins = currentAnimation->GetCurrentFrame();
	// Draw player in correct direction
	app->render->DrawTexture(texture, position.x, position.y, &rectCoins);
	return true;

}

bool Coins::CleanUp()
{
	if (!active)
		return true;

	app->tex->UnLoad(entityData->texture);
	pendingToDelete = true;
	active = false;

	return true;
}