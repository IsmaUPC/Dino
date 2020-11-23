#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneIntro.h"
#include "ModuleFadeToBlack.h"

#include <SDL_mixer\include\SDL_mixer.h>

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
	app->SetLastScene((Module*)this);

	app->audio->PlayMusic("Assets/audio/music/MusicIntro.ogg");
	bgIntro = app->tex->Load("Assets/textures/TitleScreen.png");
	animationIntro.texture = app->tex->Load("Assets/textures/DinoSprites.png");
	animationIntro.position = { 400 , 345 };
	idleAnim.loop = true;
	idleAnim.speed = 0.005f;

	for (int i = 0; i < 4; i++)
		idleAnim.PushBack({ 336 * i,0, 336, 336 });

	animationIntro.currentAnimation = &idleAnim;

	SDL_QueryTexture(bgIntro, NULL, NULL, &imgW, &imgH);
	app->render->camera.x = app->render->camera.y = 0;
	return true;
}

bool SceneIntro::PreUpdate()
{
	return true;
}

bool SceneIntro::Update(float dt)
{

	animationIntro.currentAnimation->Update();
	
	return true;
}

bool SceneIntro::PostUpdate()
{
	bool ret = true;
	SDL_Rect rectIntro;
	rectIntro = animationIntro.currentAnimation->GetCurrentFrame();

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (app->input->GetKey(SDL_SCANCODE_KP_ENTER) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN 
		|| app->input->GetKey(SDL_SCANCODE_RETURN2) == KEY_DOWN) 
	{
		app->fade->FadeToBlack(this, (Module*)app->scene, 60.f);
		return true;
	}

	app->render->DrawTexture(bgIntro, app->render->camera.x, app->render->camera.y);
	app->render->DrawTexture(animationIntro.texture, animationIntro.position.x, animationIntro.position.y, &rectIntro);
	return ret;
}

bool SceneIntro::CleanUp()
{
	if (!active)
		return true;

	LOG("Freeing scene");
	Mix_HaltMusic();
	app->tex->UnLoad(bgIntro);
	app->tex->UnLoad(animationIntro.texture);
	bgIntro = nullptr;
	active = false;
	return true;
}
