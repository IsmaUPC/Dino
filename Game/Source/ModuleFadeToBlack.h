#ifndef __MODULEFADETOBLACK_H__
#define __MODULEFADETOBLACK_H__

#include "Module.h"

#include "SDL/include/SDL.h"

class SDL_Rect;

class ModuleFadeToBlack : public Module
{
public:

	ModuleFadeToBlack();

	//Destructor
	virtual ~ModuleFadeToBlack();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	bool FadeToBlack(Module* toDisable, Module* toEnable, float frames = 60);

private:

	enum Fade_Step
	{
		NONE,
		TO_BLACK,
		FROM_BLACK
	} currentStep = Fade_Step::NONE;

	int frameCount = 0;
	int maxFadeFrames = 0;

	SDL_Rect screenRect = {NULL};

	Module* moduleToEnable = nullptr;
	Module* moduleToDisable = nullptr;
};

#endif // !__MODULEFADETOBLACK_H__