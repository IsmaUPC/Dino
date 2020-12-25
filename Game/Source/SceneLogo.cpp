#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "SceneLogo.h"
#include "ModuleFadeToBlack.h"

#include "Defs.h"
#include "Log.h"

#define LOGO_FADE_SPEED 100.0f

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
	//transition = false;

	img = app->tex->Load("Assets/Textures/scene_logo.png");

	SDL_QueryTexture(img, NULL, NULL, &imgW, &imgH);
	app->render->camera.x = app->render->camera.y = 0;
	logo = {imgX,imgY,imgW,imgH};
	state = 0;
	timeCounter = 0.0f;
	logoAlpha = 0.0f;
	
	return true;
}

bool SceneLogo::PreUpdate()
{
	return true;
}

bool SceneLogo::Update(float dt)
{

	if (state == 0)
	{
		state = 1;
	}
	else if (state == 1)
	{
		logoAlpha += (LOGO_FADE_SPEED * dt);

		if (logoAlpha > 255.0f)
		{
			logoAlpha = 255.0f;
			state = 2;
		}
	}
	else if (state == 2)
	{
		// Waiting for 3 seconds
		timeCounter += dt;
		if (timeCounter >= 3.0f) state = 3;
	}
	else if (state == 3)
	{
		if(logoAlpha!=0)logoAlpha -= (LOGO_FADE_SPEED * dt);

		if (logoAlpha < 0.0f)
		{
			logoAlpha = 0.0f;
			app->fade->FadeToBlack(this, (Module*)app->sceneIntro, 60.f);
		}
	}

	return true;
}

bool SceneLogo::PostUpdate()
{
	bool ret = true;
	
	SDL_SetTextureAlphaMod(img, logoAlpha);
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

