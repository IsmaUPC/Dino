#include "App.h"
#include "Lives.h"
#include "Audio.h"
#include "Player.h"



Lives::Lives(iPoint pos) : Entity()
{
	name.Create("Lives");

	position = pos;
}

Lives::~Lives()
{}
 
bool Lives::Start()
{
	//iPoint pathInit = app->map->WorldToMap(positionInitial.x, positionInitial.y);
	//app->map->ResetPath(pathInit);
	active = true;
	texture = app->tex->Load("Assets/Textures/lives.png");

	liveFx = app->audio->LoadFx("Assets/Audio/Fx/live.wav");

	numPoints = 4;
	pointsCollision = new iPoint[4]{ { 0, 0 }, { 48 , 0 }, { 48,-48 }, { 0 ,-48 } };

	currentAnimation->loop = true;
	currentAnimation->speed = 0.06f;

	for (int i = 0; i < 3; i++)
		currentAnimation->PushBack({ (40 * i),0, 40, 40 });


	return true;
}

bool Lives::Awake(pugi::xml_node& config)
{
	LOG("Loading Lives Parser");
	bool ret = true;


	return ret;
}

bool Lives::PreUpdate()
{
	iPoint currentPositionPlayer = app->player->playerData.position;
	iPoint auxPositionLive[4];
	for (int i = 0; i < 4; i++)
	{
		auxPositionLive[i] = { position.x + pointsCollision[i].x,
			position.y + pointsCollision[i].y };
	}
	iPoint auxPositionPlayer[6];
	for (int i = 0; i < 6; i++)
	{
		auxPositionPlayer[i] = { currentPositionPlayer.x + app->player->playerData.pointsCollision[i].x,
			-48 + currentPositionPlayer.y + app->player->playerData.pointsCollision[i].y };

	}
	if (collision.IsInsidePolygons(auxPositionPlayer, app->player->playerData.numPoints, auxPositionLive, numPoints)
		&& collision.IsInsidePolygons(auxPositionLive, numPoints, auxPositionPlayer, app->player->playerData.numPoints))
	{
		app->audio->PlayFx(liveFx);
		app->player->LivePlus();
		isCollected = true;
		pendingToDelete = true;
	}
	return false;
}

bool Lives::Update(float dt)
{
	currentAnimation->Update();
	return true;
}
bool Lives::PostUpdate()
{

	SDL_Rect rectLives;
	rectLives = currentAnimation->GetCurrentFrame();
	// Draw player in correct direction
	app->render->DrawTexture(texture, position.x, position.y, &rectLives);
	return true;

}

bool Lives::CleanUp()
{
	if (!active)
		return true;

	app->tex->UnLoad(entityData->texture);
	pendingToDelete = true;
	active = false;

	return true;
}