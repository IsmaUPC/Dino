#ifndef __SCENEINTRO_H__
#define __SCENEINTRO_H__

#include "Module.h"

struct SDL_Texture;

class SceneIntro : public Module
{
public:

	SceneIntro();

	// Destructor
	virtual ~SceneIntro();

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
	
	SDL_Texture* img;
	int imgX = 0, imgY = 0, imgW = 0, imgH = 0;

};

#endif // __SCENEINTRO_H__



