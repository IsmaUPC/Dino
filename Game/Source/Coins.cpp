#include "App.h"
#include "Coins.h"


Coins::Coins() : Entity()
{
	name.Create("Enemy");
}

Coins::~Coins()
{}

bool Coins::Start()
{
	//iPoint pathInit = app->map->WorldToMap(positionInitial.x, positionInitial.y);
	//app->map->ResetPath(pathInit);
	coinsData.texture = app->tex->Load("Assets/textures/Monedas.png");
	coinsData.position = positionInitial;

	return true;
}

bool Coins::Awake(pugi::xml_node& config)
{
	LOG("Loading Coins Parser");
	bool ret = true;


	idleAnim->loop = true;
	idleAnim->speed = 0.06f;

	for (int i = 0; i < 3; i++)
		idleAnim->PushBack({(40 * i),0, 40, 40 });

	coinsData.currentAnimation = idleAnim;
	return ret;
}
bool Coins::Update(float dt)
{
	coinsData.currentAnimation->Update();
	return true;
}
bool Coins::PostUpdate()
{

	SDL_Rect rectCoins;
	rectCoins = coinsData.currentAnimation->GetCurrentFrame();
	// Draw player in correct direction
	app->render->DrawTexture(coinsData.texture, coinsData.position.x - 15, coinsData.position.y - (rectCoins.h - 10), &rectCoins);
	return true;
}

bool Coins::CleanUp()
{

	if (!active)
		return true;

	app->tex->UnLoad(coinsData.texture);
	active = false;

	return true;
}