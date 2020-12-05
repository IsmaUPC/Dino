#include "GUI.h"

#include "Defs.h"
#include "Log.h"

GUI::GUI() : Entity()
{
}

GUI::GUI(TypeEntity pTypeEntity, iPoint pPosition, float pVelocity, SDL_Texture* pTexture)
	: Entity(pTypeEntity, pPosition, pVelocity, pTexture)
{
	name.Create("GUI");
}

GUI::~GUI()
{
}

bool GUI::Awake(pugi::xml_node& config)
{
	LOG("Loading GUI Parser");
	bool ret = true;

	return ret;
}

bool GUI::Start()
{
	headTex = app->tex->Load("Assets/Textures/dino_head.png");

	headAnim->PushBack({0,0,100,50});
	headAnim->loop = false;

	headW = 100;
	headH = 50;

	return true;
}

bool GUI::PreUpdate()
{
	return true;
}

bool GUI::Update(float dt)
{
	return true;
}

bool GUI::PostUpdate()
{
	point0.x = app->render->camera.x;
	point0.y = app->render->camera.y;

	point0.x = point0.x + headPositionX;
	point0.y = point0.y + headPositionY;

	SDL_Rect rectGUI;
	rectGUI = headAnim->GetCurrentFrame();
	for (int i = 0; i < 3; i++){
		app->render->DrawTexture(headTex,point0.x +(headSeparation +(headW*i)),point0.y,&rectGUI);
	}

	return true;
}

bool GUI::CleanUp()
{
	if (!active){
		return true;
	}
	app->tex->UnLoad(headTex);

	active = false;

	return true;
}

