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

	bool LoadState(pugi::xml_node& data);

	void RemoveState(pugi::xml_node& data)const;
	bool ComprobeState(int id);

private:
	AnimationIntro animationIntro;
	Animation* idleAnim=new Animation();

	SDL_Texture* bgIntro;
	int imgX = 0, imgY = 0, imgW = 0, imgH = 0;
	bool transition;

	//Timer timer;
	pugi::xml_document sceneFile;
	pugi::xml_node sceneStateFile;

	GuiButton* btnStart;
	GuiButton* btnContinue;
	GuiButton* btnRemove;
	GuiButton* btnScrollBar;
};

#endif // __SCENEINTRO_H__



