
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
	img = app->tex->Load("Assets/textures/A.png");
	SDL_QueryTexture(img, NULL, NULL, &imgW, &imgH);
	app->render->camera.x = app->render->camera.y = 0;
	return true;
}

bool SceneWin::StartModules()
{
	img = app->tex->Load("Assets/textures/A.png");
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

	return true;
}

bool SceneWin::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (app->input->GetKey(SDL_SCANCODE_KP_ENTER) == KEY_DOWN) {
		app->fade->FadeToBlack(this, (Module*)app->sceneIntro, 60.f);
		return true;
	}

	app->render->DrawTexture(img, app->render->camera.x, app->render->camera.y);
	return ret;
}

bool SceneWin::CleanUp()
{
	if (!active)
		return true;

	LOG("Freeing scene");
	app->tex->UnLoad(img);
	img = nullptr;
	active = false;
	return true;
}
