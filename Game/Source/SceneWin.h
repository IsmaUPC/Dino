#ifndef __SCENEWIN_H__
#define __SCENEWIN_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

struct AnimationWin
{
	iPoint position;
	Animation* currentAnimation;
	SDL_Texture* texture;
};

class SceneWin :public Module
{
public:

	SceneWin();

	// Destructor
	virtual ~SceneWin();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();
	bool StartModules();
	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:
	AnimationWin animationSon;
	Animation idleAnimSon;
	AnimationWin animationFather;
	Animation idleAnimFather;

	SDL_Texture* img;
	int imgX = 0, imgY = 0, imgW = 0, imgH = 0;

};

#endif // !__SCENEWIN_H__
