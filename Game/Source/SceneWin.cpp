#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneWin.h"
#include "ModuleFadeToBlack.h"

#include "Defs.h"
#include "Log.h"

SceneWin::SceneWin()
{
	name.Create("sceneWin");

}

SceneWin::~SceneWin()
{
}

bool SceneWin::Awake()
{
	LOG("Loading SceneWin");
	bool ret = true;
	return ret;
}

bool SceneWin::Start()
{
	img = app->tex->Load("Assets/textures/SceneWin.png");
	animationSon.texture = app->tex->Load("Assets/textures/DinoSprites.png");
	animationSon.position = { 340 , 372 };
	idleAnimSon.loop = true;
	idleAnimSon.speed = 0.01;

	for (int i = 0; i < 4; i++)
		idleAnimSon.PushBack({ 336 * i,0, 336, 336 });

	animationSon.currentAnimation = &idleAnimSon;

	animationFather.texture = app->tex->Load("Assets/textures/Dino_Orange_Big.png");
	animationFather.position = { 540 , 180 };
	idleAnimFather.loop = true;
	idleAnimFather.speed = 0.01;

	for (int i = 0; i < 4; i++)
		idleAnimFather.PushBack({ 558 * i,0, 558, 558 });

	animationFather.currentAnimation = &idleAnimFather;

	SDL_QueryTexture(img, NULL, NULL, &imgW, &imgH);
	app->render->camera.x = app->render->camera.y = 0;
	return true;
}

bool SceneWin::StartModules()
{
	img = app->tex->Load("Assets/textures/SceneWin.png");
	animationSon.texture = app->tex->Load("Assets/textures/DinoSprites.png");
	animationSon.position = { 340 , 372 };
	idleAnimSon.loop = true;
	idleAnimSon.speed = 0.01;

	for (int i = 0; i < 4; i++)
		idleAnimSon.PushBack({ 336 * i,0, 336, 336 });

	animationSon.currentAnimation = &idleAnimSon;

	animationFather.texture = app->tex->Load("Assets/textures/Dino_Orange_Big.png");
	animationFather.position = { 540 , 180 };
	idleAnimFather.loop = true;
	idleAnimFather.speed = 0.01;

	for (int i = 0; i < 4; i++)
		idleAnimFather.PushBack({ 558 * i,0, 558, 558 });

	animationFather.currentAnimation = &idleAnimFather;

	SDL_QueryTexture(img, NULL, NULL, &imgW, &imgH);
	app->render->camera.x = app->render->camera.y = 0;
	return true;
}

bool SceneWin::PreUpdate()
{
	return true;
}

bool SceneWin::Update(float dt)
{
	SString title("Scene IntroLose");
	app->win->SetTitle(title.GetString());
	animationSon.currentAnimation->Update();
	animationFather.currentAnimation->Update();
	return true;
}

bool SceneWin::PostUpdate()
{
	bool ret = true;
	SDL_Rect rectSon;
	rectSon = animationSon.currentAnimation->GetCurrentFrame();
	SDL_Rect rectFather;
	rectFather = animationFather.currentAnimation->GetCurrentFrame();

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (app->input->GetKey(SDL_SCANCODE_KP_ENTER) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN
		|| app->input->GetKey(SDL_SCANCODE_RETURN2) == KEY_DOWN) 
	{
		app->fade->FadeToBlack(this, (Module*)app->sceneIntro, 60.f);
		return true;
	}

	app->render->DrawTexture(img, app->render->camera.x, app->render->camera.y);
	app->render->DrawTexture(animationSon.texture, animationSon.position.x, animationSon.position.y, &rectSon);
	app->render->DrawTextureFlip(animationFather.texture, animationFather.position.x, animationFather.position.y, &rectFather);

	return ret;
}

bool SceneWin::CleanUp()
{
	if (!active)
		return true;

	LOG("Freeing scene");
	app->tex->UnLoad(img);
	app->tex->UnLoad(animationSon.texture);
	app->tex->UnLoad(animationFather.texture);
	img = nullptr;
	active = false;
	return true;
}
