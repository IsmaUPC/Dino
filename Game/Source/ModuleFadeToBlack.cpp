#include "ModuleFadeToBlack.h"

#include "App.h"
#include "Scene.h"
#include "Render.h"

#include "SDL/include/SDL_render.h"
#include "Log.h"

ModuleFadeToBlack::ModuleFadeToBlack()
{
	name.Create("fade");

}

ModuleFadeToBlack::~ModuleFadeToBlack()
{


}

bool ModuleFadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	//screenRect = { 0, 0, WINDOW_W, WINDOW_H };
	// Enable blending mode for transparency
	//SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

bool ModuleFadeToBlack::PreUpdate()
{
	return true;
}

bool ModuleFadeToBlack::Update(float dt)
{
	/*
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return true;

	if (currentStep == Fade_Step::TO_BLACK)
	{
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			// TODO 1: Enable / disable the modules received when FadeToBlacks() gets called
			currentStep = Fade_Step::FROM_BLACK;
			if (moduleToDisable->active == true){
				moduleToDisable->CleanUp();
			}
			if (moduleToEnable->active == true){
				moduleToEnable->Start();
			}
		}
	}
	else
	{
		--frameCount;
		if (frameCount <= 0)
		{
			currentStep = Fade_Step::NONE;
		}
	}
	*/
	return true;
}

bool ModuleFadeToBlack::PostUpdate()
{
	// Exit this function if we are not performing a fade
	/*if (currentStep == Fade_Step::NONE) return true;

	float fadeRatio = (float)frameCount / (float)maxFadeFrames;

	// Render the black square with alpha on the screen
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
	screenRect.x = app->render->camera.x;
	screenRect.y = app->render->camera.y;
	SDL_RenderFillRect(app->render->renderer, &screenRect);
	*/
	return true;
}

bool ModuleFadeToBlack::CleanUp()
{

	return true;
}

bool ModuleFadeToBlack::FadeToBlack(Module* toDisable, Module* toEnable, float frames)
{

	bool ret = true;
	moduleToDisable = toDisable;
	moduleToEnable = toEnable;

	if (moduleToDisable->active == true) {
		moduleToDisable->CleanUp();
	}
	if (moduleToEnable->active == false) {
		toEnable->Init();
 		moduleToEnable->Start();
	}

	/*frameCount = 0;
	if (currentStep == Fade_Step::NONE) {

		currentStep = Fade_Step::TO_BLACK;
		maxFadeFrames = frames;	
		if (toEnable->active == false) {
			toEnable->Init();
		}
		ret = true;
	}*/


	return ret;
}

