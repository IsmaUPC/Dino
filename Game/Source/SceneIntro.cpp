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

	btnPlay = new GuiButton(1, { WINDOW_W / 2 - 200 / 2, 400, 200, 40 }, "PLAY");
	btnPlay->SetObserver(this);

	btnContinue = new GuiButton(2, { WINDOW_W / 2 - 200 / 2, 450, 200, 40 }, "CONTINUE");
	btnContinue->SetObserver(this);

	btnRemove = new GuiButton(3, { WINDOW_W / 2 - 200 / 2, 500, 200, 40 }, "REMOVE");
	btnRemove->SetObserver(this);

	btnSettings = new GuiButton(4, { WINDOW_W / 2 - 200 / 2, 550, 200, 40 }, "SETTINGS");
	btnSettings->SetObserver(this);

	btnCredits = new GuiButton(5, { WINDOW_W / 2 - 200 / 2, 600, 200, 40 }, "CREDITS");
	btnCredits->SetObserver(this);

	btnExit = new GuiButton(6, { WINDOW_W / 2 - 200 / 2, 650, 200, 40 }, "EXIT");
	btnExit->SetObserver(this);


	menuSettings = new GuiSettings({ WINDOW_W / 2 + 300, 300 },this);
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

	btnPlay->Update(dt);
	btnContinue->Update(dt);
	btnRemove->Update(dt);
	btnSettings->Update(dt);
	btnCredits->Update(dt);
	btnExit->Update(dt);
	menuSettings->Update(dt);

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
	
	btnPlay->Draw();
	btnContinue->Draw();
	btnRemove->Draw();
	btnSettings->Draw();
	btnCredits->Draw();
	btnExit->Draw();

	menuSettings->Draw();

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
		else if (control->id == 4)
		{
			LOG("SETTINGS");
			btnPlay->state = GuiControlState::DISABLED;
			btnContinue->state = GuiControlState::DISABLED;
			btnSettings->state = GuiControlState::DISABLED;
			btnCredits->state = GuiControlState::DISABLED;
			btnExit->state = GuiControlState::DISABLED;
			btnRemove->state = GuiControlState::DISABLED;
			menuSettings->AbleDisableSetting();
		}
		else if (control->id == 5) LOG("CREDITS");
		else if (control->id == 6) LOG("EXIT");
		else if (control->id == 12)
		{
			LOG("RETURN");
			btnPlay->state = GuiControlState::NORMAL;
			btnContinue->state = GuiControlState::NORMAL;
			btnSettings->state = GuiControlState::NORMAL;
			btnCredits->state = GuiControlState::NORMAL;
			btnExit->state = GuiControlState::NORMAL;
			btnRemove->state = GuiControlState::NORMAL;
			menuSettings->AbleDisableSetting();
		}
	}
	case GuiControlType::SLIDER:
	{
		/*if (control->id == 3)
		{
			int newVolum;
			newVolum = btnScrollBar->GetValue();
			LOG("%d", newVolum);
		}*/
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

//bool SceneIntro::SaveState(pugi::xml_node& data) const
//{
//	data.child("level").attribute("lvl").set_value(0);
//	
//	return true;
//}

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
		else if (id == 3)RemoveState(sceneStateFile), currentScene=0;
	}
	sceneFile.reset();
	return ret;
}

