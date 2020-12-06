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
	int headSeparation = 10;
	int headPositionX = 20;
	int headPositionY = 20;

	int headW = 0;
	int headH = 0;
	int arrowW = 0;
	int arrowH = 0;


	uint* respawn = 0;
	bool inCheckPoint;

	iPoint point0 = { 0,0 };
	SDL_Texture* headTex;
	SDL_Texture* arrowTex;

	Animation* headAnim = new Animation();
	Animation* arrowAnim = new Animation();
	Animation* buttonEAnim = new Animation();

	//Text
	int coinCount = 3;

	int hudFont = 0;

	char coinText[10] = { "\0" };
};

#endif // __GUI_H__