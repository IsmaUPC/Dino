#include "App.h"
#include "Render.h"
#include "SceneManager.h"
#include "SceneControl.h"
#include "GuiSettings.h"


GuiSettings::GuiSettings(iPoint Position, SceneControl* moduleObserver)
{
	active = false;

	initialPos = Position;

	int separacion = 50;

	sldMusic = new GuiSlider(11, { initialPos.x, initialPos.y, 200, 40 }, "MUSIC", 0, 100);
	sldMusic->SetObserver(moduleObserver);

	sldFx = new GuiSlider(12, { initialPos.x, initialPos.y + separacion, 200, 40 }, "FX", 0, 100);
	sldFx->SetObserver(moduleObserver);

	chBxFullScreen = new GuiCheckBox(13, { initialPos.x, initialPos.y + separacion * 2, 200, 40 }, "Fullscreen");
	chBxFullScreen->SetObserver(moduleObserver);

	chBxVSync = new GuiCheckBox(14, { initialPos.x, initialPos.y + separacion * 3, 200, 40 }, "VSync",true);
	chBxVSync->SetObserver(moduleObserver);

	chBxVSync->state = GuiControlState::DISABLED;

	btnBack = new GuiButton(10, { initialPos.x + 50, initialPos.y + separacion * 4, 150, 40 }, "BACK");
	btnBack->SetObserver(moduleObserver);

}

GuiSettings::~GuiSettings()
{

}

bool GuiSettings::Update(float dt)
{
	if (active)
	{
		sldMusic->Update(dt);
		sldFx->Update(dt);
		chBxFullScreen->Update(dt);
		chBxVSync->Update(dt);
		btnBack->Update(dt);
	}

	return true;
}

bool GuiSettings::Draw()
{
	if (active)
	{
		sldMusic->Draw();
		sldFx->Draw();
		chBxFullScreen->Draw();
		chBxVSync->Draw();
		btnBack->Draw();
	}

	return true;
}

bool GuiSettings::CleanUp()
{

	active = false;

	return true;
}

void GuiSettings::MovePosition()
{
	int separacion = 50;
	int x = initialPos.x - app->render->camera.x;
	int y = initialPos.y - app->render->camera.y;

	sldMusic->bounds.x = x;
	sldMusic->bounds.y = y;

	sldFx->bounds.x = x;
	sldFx->bounds.y = y + separacion * 1;

	chBxFullScreen->bounds.x = x;
	chBxFullScreen->bounds.y = y + separacion * 2;

	chBxVSync->bounds.x = x;
	chBxVSync->bounds.y = y + separacion * 3;

	btnBack->bounds.x = x + 50;
	btnBack->bounds.y = y + separacion * 4;

}

