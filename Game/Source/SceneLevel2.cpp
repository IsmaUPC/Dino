#include "SceneLevel2.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Player.h"
#include "SceneIntro.h"
#include "Map.h"
#include "EntityManager.h"
#include "ModuleFadeToBlack.h"
#include "Pathfinding.h"

#include <SDL_mixer\include\SDL_mixer.h>

#include "Defs.h"
#include "Log.h"

SceneLevel2::SceneLevel2() : Module()
{
	name.Create("sceneLevel2");
}

// Destructor
SceneLevel2::~SceneLevel2()
{}

// Called before render is available
bool SceneLevel2::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneLevel2::Start()
{
	// Load map
	app->SetLastScene((Module*)this);
	if (app->map->Load("dino_map_2.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;

		if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}
	app->player->win = false;
	//Positions initials
	app->player->positionInitial = {576,1534};
	app->entityManager->AddEntity(GROUND_ENEMY, 34, 31);
	app->entityManager->AddEntity(GROUND_ENEMY, 57, 25);
	app->entityManager->AddEntity(GROUND_ENEMY, 77, 8);
	app->entityManager->AddEntity(GROUND_ENEMY, 119, 10);
	app->entityManager->AddEntity(GROUND_ENEMY, 149, 8);
	app->entityManager->AddEntity(GROUND_ENEMY, 130, 8);
	app->entityManager->AddEntity(GROUND_ENEMY, 168, 8);
	app->entityManager->AddEntity(GROUND_ENEMY, 198, 6);
	app->entityManager->AddEntity(GROUND_ENEMY, 77, 29);
	app->entityManager->AddEntity(GROUND_ENEMY, 77, 29);
	app->entityManager->AddEntity(GROUND_ENEMY, 127, 29);
	app->entityManager->AddEntity(GROUND_ENEMY, 145, 29);
	app->entityManager->AddEntity(GROUND_ENEMY, 163, 23);
	app->entityManager->AddEntity(GROUND_ENEMY, 163, 23);
	app->entityManager->AddEntity(GROUND_ENEMY, 203, 15);
	app->entityManager->AddEntity(AIR_ENEMY, 53, 20);
	app->entityManager->AddEntity(AIR_ENEMY, 66, 10);
	app->entityManager->AddEntity(AIR_ENEMY, 66, 10);
	app->entityManager->AddEntity(AIR_ENEMY, 54, 32);
	app->entityManager->AddEntity(AIR_ENEMY, 189, 11);
	app->entityManager->AddEntity(AIR_ENEMY, 201, 9);

	app->entityManager->AddEntity(FIREBALL, 0, 0);

	app->player->Init();
	app->player->Start();
	app->map->active = true;

	// Load music
	app->audio->PlayMusic("Assets/Audio/Music/LOKI_8bits.ogg");
	img = app->tex->Load("Assets/Textures/sky_3.png");
	animationFather.texture = app->tex->Load("Assets/Textures/dino_orange.png");

	animationFather.position = { 10500, 639 };
	idleAnim.loop = true;
	idleAnim.speed = 0.025;

	for (int i = 0; i < 4; i++)
		idleAnim.PushBack({ 117 * i,0, 117, 117 });

	animationFather.currentAnimation = &idleAnim;

	SDL_QueryTexture(img, NULL, NULL, &imgW, &imgH);

	app->render->camera.y -= imgH;

	return true;
}

void SceneLevel2::SetDebugCollaider(bool value)
{
	if (value == NULL)
		debugCollisions = !debugCollisions;
	else
		debugCollisions = value;
}

// Called each loop iteration
bool SceneLevel2::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneLevel2::Update(float dt)
{
	//DEBUG KEYS
	DebugKeys();

	//Draw Background
	Parallax();

	// Draw map
	app->map->Draw();

	iPoint vec;
	vec.x = 0, vec.y = 0;
	app->input->GetMousePosition(vec.x, vec.y);

	if (CheckChangeFPS(app->GetFramerate()))
		idleAnim.speed = (dt * 100) * 0.025f;

	animationFather.currentAnimation->Update();
	
	if (app->player->win)victory = true;

	if (app->player->CheckGameOver(2) && lose == false && app->player->godMode == false)
	{
		LOG("GAME OVER!");
		lose = true;
	}
	return true;
}

// Called each loop iteration
bool SceneLevel2::PostUpdate()
{
	bool ret = true;
	SDL_Rect rectFather;
	rectFather = animationFather.currentAnimation->GetCurrentFrame();

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
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
bool SceneLevel2::CleanUp()
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

void SceneLevel2::Parallax()
{
	speedImg = -0.9f;
	imgX = (int)(app->render->camera.x / 6);
	imgX *= speedImg;

	imgY = (int)((app->render->camera.y / 6)) * 0.2f;

	app->render->DrawTexture(img, imgX, imgY);
}

void SceneLevel2::DebugKeys()
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

