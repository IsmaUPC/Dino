#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Player.h"
#include "SceneIntro.h"
#include "Map.h"
#include "ModuleFadeToBlack.h"

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

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// L03: DONE: Load map
	//app->map->Load("hello2.tmx");
	//app->map->Load("iso_walk.tmx");
	app->map->Load("Mapa_PixelArt.tmx");
	
	// Load music
	//app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");

	img = app->tex->Load("Assets/textures/Fondo.png");
	animationFather.texture = app->tex->Load("Assets/textures/Dino_Orange.png");
	
	animationFather.position = { 2352, 495 };
	idleAnim.loop = true;
	idleAnim.speed = 0.025;

	for (int i = 0; i < 4; i++)
		idleAnim.PushBack({ 117 * i,0, 117, 117 });

	animationFather.currentAnimation = &idleAnim;

	SDL_QueryTexture(img, NULL,NULL,&imgW,&imgH);

	app->render->camera.y -= imgH;

	return true;
}

bool Scene::StartModules()
{
	app->player->Init();
	app->player->Start();

	app->map->active = true;

	app->map->Load("Mapa_PixelArt.tmx");
	// Load music
	//app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");
	img = app->tex->Load("Assets/textures/Fondo.png");
	animationFather.texture = app->tex->Load("Assets/textures/Dino_Orange.png");

	animationFather.position = { 2352, 495 };
	idleAnim.loop = true;
	idleAnim.speed = 0.025;

	for (int i = 0; i < 4; i++)
		idleAnim.PushBack({ 117 * i,0, 117, 117 });

	animationFather.currentAnimation = &idleAnim;

	SDL_QueryTexture(img, NULL, NULL, &imgW, &imgH);

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
    // L02: DONE 3: Request Load / Save when pressing L/S
	if(app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		app->LoadGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		app->SaveGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= 3;

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += 3;

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= 3;

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += 3;

	//DEBUG KEYS
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		SetDebugCollaider();

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		app->fade->FadeToBlack(this, (Module*)app->sceneIntro);
		return true;
	}

	//Draw Background
	Parallax();

	// Draw map
	app->map->Draw();

	iPoint vec;
	vec.x = 0, vec.y = 0;
	app->input->GetMousePosition(vec.x, vec.y);
	// L03: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile: %d, %d",
				   app->map->data.width, app->map->data.height,
				   app->map->data.tileWidth, app->map->data.tileHeight,
				   app->map->data.tilesets.count(),
				   app->map->WorldToMap(vec.x - app->render->camera.x, vec.y - app->render->camera.y));

	app->win->SetTitle(title.GetString());

	animationFather.currentAnimation->Update();

	if (app->player->CheckVictory({ app->player->playerData.position.x + app->player->playerData.velocity + 48,
		app->player->playerData.position.y + (int)app->player->velY }) && victory == false)
	{
		LOG("Congratulations, YOU WIN!");
		victory = true;
	}
	if (app->player->CheckGameOver() && lose==false)
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
	app->audio->CleanUp();
	app->map->CleanUp();
	app->tex->UnLoad(img);
	app->tex->UnLoad(animationFather.texture);
	app->player->CleanUp();

	active = false;
	return true;
}

void Scene::Parallax()
{
	if (app->render->camera.x > 0) {
		imgX = app->render->camera.x;
		speedImg = -1;

	}
	else if ((app->render->camera.x + imgW) < WINDOW_W+20)
	{
		imgX = app->render->camera.x - (WINDOW_W - 20);
		speedImg = -1;
	}
	else
	{
		imgX = (int)(app->render->camera.x / 2);
		speedImg = 0.5f;
	}
	app->render->DrawTexture(img, imgX,0,0, speedImg);

}
