#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneLose.h"
#include "ModuleFadeToBlack.h"

#include "Defs.h"
#include "Log.h"

SceneLose::SceneLose() : Module()
{
	name.Create("sceneLose");

}

SceneLose::~SceneLose()
{
}

bool SceneLose::Awake()
{
	LOG("Loading SceneLose");
	bool ret = true;
	return ret;
}

bool SceneLose::Start()
{
	img = app->tex->Load("Assets/textures/A.png");
	SDL_QueryTexture(img, NULL, NULL, &imgW, &imgH);
	app->render->camera.x = app->render->camera.y = 0;
	return true;
}

bool SceneLose::StartModules()
{
	img = app->tex->Load("Assets/textures/A.png");
	SDL_QueryTexture(img, NULL, NULL, &imgW, &imgH);
	app->render->camera.x = app->render->camera.y = 0;
	return true;
}

bool SceneLose::PreUpdate()
{
	return true;
}

bool SceneLose::Update(float dt)
{
	SString title("Scene Lose");
	app->win->SetTitle(title.GetString());

	return true;
}

bool SceneLose::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (app->input->GetKey(SDL_SCANCODE_KP_ENTER) == KEY_DOWN) {
		app->fade->FadeToBlack(this, (Module*)app->scene, 60.f);
		return true;
	}

	app->render->DrawTexture(img, app->render->camera.x, app->render->camera.y);
	return ret;
}

bool SceneLose::CleanUp()
{
	if (!active)
		return true;

	LOG("Freeing scene");
	app->tex->UnLoad(img);
	img = nullptr;
	active = false;
	return true;
}
