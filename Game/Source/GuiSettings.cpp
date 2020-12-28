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

	chBxFullScreen = new GuiCheckBox(13, { initialPos.x, initialPos.y + separacion*2, 200, 40 }, "FULLSCREEN");
	chBxFullScreen->SetObserver(moduleObserver);

	chBxVSync = new GuiCheckBox(14, { initialPos.x, initialPos.y + separacion * 3, 200, 40 }, "FULLSCREEN",true);
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
		btnBack->Update(dt);
		chBxFullScreen->Update(dt);
		chBxVSync->Update(dt);
	}

	return true;
}

bool GuiSettings::Draw()
{
	if (active)
	{
		sldMusic->Draw();
		sldFx->Draw();
		btnBack->Draw();
		chBxFullScreen->Draw();
		chBxVSync->Draw();
	}

	return true;
}

bool GuiSettings::CleanUp()
{

	active = false;

	return false;
}

