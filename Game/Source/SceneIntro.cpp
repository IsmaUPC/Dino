#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "SceneIntro.h"
#include "SceneManager.h"

#include <SDL_mixer\include\SDL_mixer.h>

#include "Defs.h"
#include "Log.h"

SceneIntro::SceneIntro()
{
	active = true;
	name.Create("sceneIntro");

	// GUI: Initialize required controls for the screen
	btnStart = new GuiButton(1, { 1280 / 2 - 300 / 2, 300, 300, 80 }, "START");
	btnStart->SetObserver(this);

	btnContinue = new GuiButton(2, { 1280 / 2 - 300 / 2, 400, 300, 80 }, "CONTINUE");
	btnContinue->SetObserver(this);

	btnRemove = new GuiButton(3, { 1280 / 2 - 300 / 2, 500, 300, 80 }, "REMOVE");
	btnRemove->SetObserver(this);

	btnScrollBar = new GuiButton(4, { 100, 100, 100, 50 }, "VALUE");
	btnScrollBar->SetObserver(this);
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
	transition = false;

	app->audio->PlayMusic("Assets/Audio/Music/music_intro.ogg");
	bgIntro = app->tex->Load("Assets/Textures/title_screen.png");
	animationIntro.texture = app->tex->Load("Assets/Textures/dino_sprites.png");
	animationIntro.position = { 400 , 345 };
	idleAnim->loop = true;
	idleAnim->speed = 0.005f;

	for (int i = 0; i < 4; i++)
		idleAnim->PushBack({ 336 * i,0, 336, 336 });

	animationIntro.currentAnimation = idleAnim;

	SDL_QueryTexture(bgIntro, NULL, NULL, &imgW, &imgH);
	app->render->camera.x = app->render->camera.y = 0;
	
	ComprobeState(2);
	return true;
}

bool SceneIntro::PreUpdate()
{
	return true;
}

bool SceneIntro::Update(float dt)
{

	animationIntro.currentAnimation->Update();
	
	idleAnim->speed = (dt * 100) * 0.05f;

	btnStart->Update(dt);
	btnContinue->Update(dt);
	btnRemove->Update(dt);
	btnScrollBar->Update(dt);

	return true;
}

bool SceneIntro::PostUpdate()
{
	bool ret = true;
	SDL_Rect rectIntro;
	rectIntro = animationIntro.currentAnimation->GetCurrentFrame();

	//if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		//ret = false;

	if ((app->input->GetKey(SDL_SCANCODE_KP_ENTER) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN 
		|| app->input->GetKey(SDL_SCANCODE_RETURN2) == KEY_DOWN))//has quitado un timer 
	{
		transition = true;
		TransitionToScene(SceneType::LEVEL1);
		return true;
	}

	app->render->DrawTexture(bgIntro, app->render->camera.x, app->render->camera.y);
	app->render->DrawTexture(animationIntro.texture, animationIntro.position.x, animationIntro.position.y, &rectIntro);
	
	btnStart->Draw();
	btnContinue->Draw();
	btnRemove->Draw();
	btnScrollBar->Draw();

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
bool SceneIntro::OnGuiMouseClickEvent(GuiControl* control)
{
	bool ret = true;
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1) TransitionToScene(SceneType::LEVEL1);// PLAY
		else if (control->id == 2 && currentScene != 0)
		{
			// CONTINUED in last scene
			if (currentScene == 1)TransitionToScene(SceneType::LEVEL1);
			if (currentScene == 2)TransitionToScene(SceneType::LEVEL2);
			isContinue = true;
		}
		else if (control->id == 3) ComprobeState(3);
	}
	default: break;
	}
	return true;
}

bool SceneIntro::LoadState(pugi::xml_node& data)
{
	currentScene = data.child("level").attribute("lvl").as_int(0);
	return true;
}

void SceneIntro::RemoveState(pugi::xml_node& data)const
{
	data.child("level").attribute("lvl").set_value(0);
}

bool SceneIntro::ComprobeState(int id)
{
	bool ret = true;
	pugi::xml_parse_result result = sceneFile.load_file("save_game.xml");

	// Check result for loading errors
	if (result == NULL)
	{
		LOG("Could not load map xml file savegame.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		sceneStateFile = sceneFile.first_child();
		sceneStateFile = sceneStateFile.child("scene_manager");
		if(id==2)LoadState(sceneStateFile);
		else if (id == 3)RemoveState(sceneStateFile);
	}
	sceneFile.reset();
	return ret;
}

