#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "SceneLogo.h"
#include "ModuleFadeToBlack.h"

#include "Defs.h"
#include "Log.h"

SceneLogo::SceneLogo() : Module()
{
	name.Create("sceneLogo");
}

SceneLogo::~SceneLogo()
{
}

bool SceneLogo::Awake()
{
	LOG("Loading SceneLogo");
	bool ret = true;
	return ret;
}

bool SceneLogo::Start()
{
	app->SetLastScene((Module*)this);
	transition = false;

	img = app->tex->Load("Assets/Textures/scene_logo.png");

	SDL_QueryTexture(img, NULL, NULL, &imgW, &imgH);
	app->render->camera.x = app->render->camera.y = 0;

	timer.Start();
	return true;
}

bool SceneLogo::PreUpdate()
{

	return true;
}

bool SceneLogo::Update(float dt)
{

	return true;
}

bool SceneLogo::PostUpdate()
{
	bool ret = true;

	//if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		//ret = false;

	if ((app->input->GetKey(SDL_SCANCODE_KP_ENTER) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN
		|| app->input->GetKey(SDL_SCANCODE_RETURN2) == KEY_DOWN) && !transition && timer.ReadSec()> CCOOLDOWNSCENE)
	{
		transition = true;
		app->fade->FadeToBlack(this, (Module*)app->sceneIntro, 80.f);
		return true;
	}

	app->render->DrawTexture(img, app->render->camera.x, app->render->camera.y);
	
	return ret;
}

bool SceneLogo::CleanUp()
{
	if (!active)
		return true;

	LOG("Freeing scene");
	app->tex->UnLoad(img);
	img = nullptr;
	active = false;
	return true;
}

