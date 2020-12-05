#ifndef __GUI_H__
#define __GUI_H__

#include "Entity.h"
#include "PugiXml\src\pugixml.hpp"

class GUI : public Entity
{
public:

	GUI();
	GUI(TypeEntity pTypeEntity, iPoint pPosition, float pVelocity, SDL_Texture* pTexture);

	virtual ~GUI();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

public:

private:

	// positions vars
	int headSeparation = 20;
	int headPositionX = 20;
	int headPositionY = 20;

	int headW = 0;
	int headH = 0;

	iPoint point0 = {0,0};
	SDL_Texture* headTex;
	Animation* headAnim = new Animation();
};

#endif // __GUI_H__