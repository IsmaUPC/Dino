#ifndef __SCENEWIN_H__
#define __SCENEWIN_H__

#include "Module.h"

struct SDL_Texture;

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

	SDL_Texture* img;
	int imgX = 0, imgY = 0, imgW = 0, imgH = 0;

};

#endif // !__SCENEWIN_H__
