#include "App.h"
#include "GuiMenuPause.h"
#include "Audio.h"
#include "SceneManager.h"
#include "SceneControl.h"

#include "Defs.h"
#include "Log.h"

GuiMenuPause::GuiMenuPause(iPoint Position, SceneControl* moduleObserver, SDL_Texture* textureAtlas)
{
	int separacion = 50;

	btnResume = new GuiButton(1, { Position.x , Position.y + separacion*0, 200, 40 }, "RESUME", RECTANGLE, textureAtlas);
	btnResume->SetObserver(moduleObserver);

	btnSettings = new GuiButton(2, { Position.x, Position.y + separacion * 1, 200, 40 }, "SETTINGS",RECTANGLE, textureAtlas);
	btnSettings->SetObserver(moduleObserver);

	btnBackToTitle = new GuiButton(3, { Position.x, Position.y + separacion * 2, 200, 40 }, "TITLE", RECTANGLE, textureAtlas);
	btnBackToTitle->SetObserver(moduleObserver);

	btnExit = new GuiButton(4, { Position.x, Position.y + separacion * 3, 200, 40 }, "EXIT", EXIT, textureAtlas);
	btnExit->SetObserver(moduleObserver);

	menuSettings = new GuiSettings({ WINDOW_W / 2 + 300, 300 },moduleObserver);

	observer = moduleObserver;

	active = false;
	activeMenu = false;	
}

GuiMenuPause::~GuiMenuPause()
{
}

bool GuiMenuPause::Update(float dt)
{
	bool ret = true;
	if (app->sceneManager->GetIsPause())
	{
		active = true;
		MovePosition();
	}

	if (active)
	{
		btnResume->Update(dt);
		btnSettings->Update(dt);
		btnBackToTitle->Update(dt);
		ret = btnExit->Update(dt);
		if (activeMenu) menuSettings->Update(dt);
	}
	return ret;
}

bool GuiMenuPause::Draw()
{
	if (active)
	{
		btnResume->Draw();
		btnSettings->Draw();
		btnBackToTitle->Draw();
		btnExit->Draw();
		if (activeMenu) menuSettings->Draw();
	}
	return true;
}

bool GuiMenuPause::CleanUp()
{

	active = false;
	activeMenu = false;
	return true;
}

bool GuiMenuPause::Event(GuiControl* control)
{

	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1) {
			app->sceneManager->SetPause(false);
			active = false;
			activeMenu = false;
		}
		else if (control->id == 2)
		{
			activeMenu = true;
			btnResume->state = GuiControlState::DISABLED;
			btnSettings->state = GuiControlState::DISABLED;
			btnBackToTitle->state = GuiControlState::DISABLED;
			btnExit->state = GuiControlState::DISABLED;
			menuSettings->MovePosition();
			menuSettings->sldMusic->SetValue(app->audio->GetVolumeMusic());
			menuSettings->sldFx->SetValue(app->audio->GetVolumeFx());
			menuSettings->AbleDisableSetting();
		}
		else if (control->id == 3)
		{
			observer->TransitionToScene(SceneType::INTRO);
		}
		else if (control->id == 4)
		{
			return false;
		}
		else if (control->id == 10)
		{
			activeMenu = false;
			btnResume->state = GuiControlState::NORMAL;
			btnSettings->state = GuiControlState::NORMAL;
			btnBackToTitle->state = GuiControlState::NORMAL;
			btnExit->state = GuiControlState::NORMAL;
			menuSettings->AbleDisableSetting();
			app->SaveConfigRequested();
		}

	}
	case GuiControlType::SLIDER:
	{
		if (control->id == 11)
		{
			int menuMusic;
			menuMusic = menuSettings->sldMusic->GetValue();
			LOG("%d", menuMusic);
			app->audio->SetVolumeMusic(menuMusic);

		}
		if (control->id == 12)
		{
			int menuFx;
			menuFx = menuSettings->sldFx->GetValue();
			LOG("%d", menuFx);
			app->audio->SetVolumeFx(menuFx);
		}
	}
	case GuiControlType::CHECKBOX:
	{
		if (control->id == 13)
		{
			app->fullScreen = menuSettings->chBxFullScreen->GetValue();
			app->win->FullScreen(app->fullScreen);
			app->render->FullScreen();
		}
		if (control->id == 14)
		{
			bool menuVSync;
			menuVSync = menuSettings->chBxVSync->GetValue();
			LOG("%d", menuVSync);
		}
	}
	default: break;
	}
	return true;
}

void GuiMenuPause::MovePosition()
{
	int separacion = 50;

	int x = -app->render->camera.x;
	int y = -app->render->camera.y + 200;

	btnResume->bounds.x = x + 50;
	btnResume->bounds.y = y;

	
	btnSettings->bounds.x = x + 50;
	btnSettings->bounds.y = y + separacion * 1;

	btnBackToTitle->bounds.x = x + 50;
	btnBackToTitle->bounds.y = y + separacion * 2;
		
	btnExit->bounds.x = x + 50;
	btnExit->bounds.y = y + separacion * 3;
		
}
