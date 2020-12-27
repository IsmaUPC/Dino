#include "GuiSettings.h"



GuiSettings::GuiSettings(iPoint Position, SceneControl* moduleObserver)
{
	active = false;

	initialPos = Position;

	int separacion = 50;

	sldMusic = new GuiSlider(10, { initialPos.x, initialPos.y, 200, 40 }, "MUSIC", 0, 100);
	sldMusic->SetObserver(moduleObserver);

	sldFx = new GuiSlider(11, { initialPos.x, initialPos.y + separacion, 200, 40 }, "FX", 0, 100);
	sldFx->SetObserver(moduleObserver);

	btnBack = new GuiButton(12, { initialPos.x, initialPos.y + separacion * 2, 150, 40 }, "BACK");
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
	}

	return true;
}

bool GuiSettings::CleanUp()
{

	active = false;

	return false;
}

