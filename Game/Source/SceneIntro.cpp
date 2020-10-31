#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneIntro.h"
#include "ModuleFadeToBlack.h"


#include "Defs.h"
#include "Log.h"

SceneIntro::SceneIntro() : Module()
{
	name.Create("sceneIntro");
}

SceneIntro::~SceneIntro()
{
}

bool SceneIntro::Awake()
{
	LOG("Loading Scene");
	bool ret = true;
	return ret;
}

bool SceneIntro::Start()
{
	img = app->tex->Load("Assets/textures/A.png");
	SDL_QueryTexture(img, NULL, NULL, &imgW, &imgH);
	app->render->camera.x = app->render->camera.y = 0;
	return true;
}

bool SceneIntro::StartModules()
{
	img = app->tex->Load("Assets/textures/A.png");
	SDL_QueryTexture(img, NULL, NULL, &imgW, &imgH);
	app->render->camera.x = app->render->camera.y = 0;
	return true;
}



bool SceneIntro::PreUpdate()
{

	return true;
}

bool SceneIntro::Update(float dt)
{
	SString title("Scene Intro");
	app->win->SetTitle(title.GetString());

	return true;
}

bool SceneIntro::PostUpdate()
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

bool SceneIntro::CleanUp()
{
	if (!active)
		return true;

	LOG("Freeing scene");
	app->tex->UnLoad(img);
	img = nullptr;
	active = false;
	return true;
}
