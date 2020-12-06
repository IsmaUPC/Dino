#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "EntityManager.h"
#include "Coins.h"
#include "SceneIntro.h"
#include "Map.h"
#include "ModuleFadeToBlack.h"
#include "Pathfinding.h"

#include <SDL_mixer\include\SDL_mixer.h>

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;
	numThisScene = 1;
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	app->SaveConfigRequested();

	victory=false;
	app->player->win = false;
	// Load map
	app->SetLastScene((Module*)this);

	//app->map->Load("Mapa_PixelArt.tmx");
	if (app->map->Load("mapa_pixelart_cp.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;

		if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}
	app->map->active = true;
	//Positions Initials
	app->player->positionInitial = { 432,1170 };
	app->entityManager->AddEntity(GROUND_ENEMY, 43, 27);
	app->entityManager->AddEntity(GROUND_ENEMY, 30, 17);
	app->entityManager->AddEntity(GROUND_ENEMY, 20, 14);
	app->entityManager->AddEntity(AIR_ENEMY, 38, 24);
	

	//Active calls
	app->player->Init();
	app->player->Start();

	// Load music
	app->audio->PlayMusic("Assets/Audio/Music/HADES_8bits.ogg");
	img = app->tex->Load("Assets/Textures/sky.png");
	animationFather.texture = app->tex->Load("Assets/Textures/dino_orange.png");
	
	animationFather.position = { 2352, 495 };
	idleAnim->loop = true;
	idleAnim->speed = 0.025;

	for (int i = 0; i < 4; i++)
		idleAnim->PushBack({ 117 * i,0, 117, 117 });

	animationFather.currentAnimation = idleAnim;

	SDL_QueryTexture(img, NULL,NULL,&imgW,&imgH);

	app->render->camera.y -= imgH;

	return true;
}

void Scene::SetDebugCollaider(bool value)
{
	if (value == NULL)
		debugCollisions = !debugCollisions;
	else
		debugCollisions = value;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{


	//DEBUG KEYS
	DebugKeys();

	//Draw Background
	Parallax();

	// Draw map
	app->map->Draw();
	if (app->input->GetKey(SDL_SCANCODE_M)==KEY_UP)
	{
		app->input->GetMousePosition(app->map->tileDestiny.x, app->map->tileDestiny.y);
		app->map->tileDestiny = app->map->WorldToMap( (app->render->camera.x*-1+  app->map->tileDestiny.x),
			(app->render->camera.y -app->map->tileDestiny.y)*-1);
		//app->map->tileDestiny = { 19, 25 };
		//app->map->ResetPath(app->map->tileDestiny);
	}

	iPoint vec;
	vec.x = 0, vec.y = 0;
	app->input->GetMousePosition(vec.x, vec.y);

	idleAnim->speed = (dt * 100) * 0.025f;

	animationFather.currentAnimation->Update();

	if(app->player->win)victory = true;

	else if (app->player->CheckGameOver(1) && lose == false && app->player->godMode == false)
	{
		LOG("GAME OVER!");
		lose = true;
	}
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;
	SDL_Rect rectFather;
	rectFather = animationFather.currentAnimation->GetCurrentFrame();

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	if (victory == true)
	{
		victory = false;
		app->fade->FadeToBlack(this, (Module*)app->sceneWin, 0);
		return true;
	}
	if (lose == true)
	{
		lose = false;
		app->fade->FadeToBlack(this, (Module*)app->sceneLose, 0);
		return true;
	}
	app->render->DrawTextureFlip(animationFather.texture, animationFather.position.x, animationFather.position.y - (rectFather.h), &rectFather);
	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	if (!active)
		return true;

	LOG("Freeing scene");
	Mix_HaltMusic();
	app->map->CleanUp();
	app->tex->UnLoad(img);
	app->tex->UnLoad(animationFather.texture);
	app->player->CleanUp();
	app->entityManager->CleanUp();
	app->audio->UnloadFxs();

	active = false;
	return true;
}

void Scene::Parallax()
{
	speedImg = -0.9f;
	imgX = (int)(app->render->camera.x / 6) - 10;
	imgX *= speedImg;

	imgY = (int)((app->render->camera.y / 6) + 1250) * 0.2f;

	app->render->DrawTexture(img, imgX, imgY);
}

void Scene::DebugKeys()
{
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		app->render->camera.x = 0;
		app->player->playerData.position = app->player->positionInitial;
		app->player->playerData.direction = WALK_R;
		Mix_RewindMusic();
	}

	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
		app->SaveConfigRequested();

	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		app->LoadConfigRequested();

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
		SetDebugCollaider();
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		if (app->player->godMode == false)app->player->godMode = true;
		else app->player->godMode = false;
	}
}

