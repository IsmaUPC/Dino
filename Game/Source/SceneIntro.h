#ifndef __SCENEINTRO_H__
#define __SCENEINTRO_H__

#include "Scene.h"
#include "Animation.h"
#include "GuiButton.h"

struct SDL_Texture;

struct AnimationIntro
{
	iPoint position;
	Animation* currentAnimation;
	SDL_Texture* texture;
};

class SceneIntro : public SceneControl
{
public:

	SceneIntro();

	// Destructor
	virtual ~SceneIntro();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();
	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool OnGuiMouseClickEvent(GuiControl* control);

private:
	AnimationIntro animationIntro;
	Animation* idleAnim=new Animation();

	SDL_Texture* bgIntro;
	int imgX = 0, imgY = 0, imgW = 0, imgH = 0;
	bool transition;

	//Timer timer;

	GuiButton* btnStart;
	GuiButton* btnExit;
	GuiButton* btnScrollBar;
};

#endif // __SCENEINTRO_H__



