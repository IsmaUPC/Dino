#include "GUI.h"
#include "Player.h"

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
	//SDL_QueryTexture(heartTex, NULL, NULL, &headW, &headH);
	headW = 100;
	headH = 50;

	headAnim->PushBack({0,0,headW,headH });
	headAnim->loop = false;


	heartTex = app->tex->Load("Assets/Textures/heart.png");
	SDL_QueryTexture(heartTex, NULL, NULL, &heartW, &heartH);
	heartW = heartW / 3;

	heartAnim->loop = true;
	heartAnim->speed = 0.13;

	for (int i = 0; i < 3; i++){
		heartAnim->PushBack({ heartW * i,0,heartW,heartH });
	}
	heartAnim->PushBack({ heartW,0,heartW,heartH });

	arrowTex = app->tex->Load("Assets/Textures/arrows.png");
	SDL_QueryTexture(arrowTex, NULL, NULL, &arrowW, &arrowH);
	arrowW = arrowW / 2;

	arrowAnim->PushBack({ 0,0,arrowW,arrowH });
	buttonEAnim->PushBack({ arrowW,0,arrowW,arrowH });

	lives = &app->player->playerData.lives;
	respawn = &app->player->playerData.respawns;


	return true;
}

bool GUI::PreUpdate()
{
	return true;
}

bool GUI::Update(float dt)
{
	heartAnim->Update();

	return true;
}

bool GUI::PostUpdate()
{
	point0.x = -app->render->camera.x;
	point0.y = -app->render->camera.y;
	
	point0.x = point0.x + headPositionX;
	point0.y = point0.y + headPositionY;

	SDL_Rect rectGUI;
	rectGUI = headAnim->GetCurrentFrame();
	for (int i = 0; i < *respawn; i++)
		app->render->DrawTexture(headTex,point0.x +((headW + headSeparation)*i),point0.y,&rectGUI);
	

	//TODO: Son demasiado grandes ARREGLAR
	point0.y = point0.y + (headH);
	rectGUI = heartAnim->GetCurrentFrame();
	for (int i = 0; i < *lives; i++)
		app->render->DrawTexture(heartTex,point0.x +((heartW + headSeparation)*i),point0.y,&rectGUI );
	
	rectGUI = buttonEAnim->GetCurrentFrame();
	if (app->player->GetInCheckPoint())
	{
		LOG("En CheckPoint");	
		app->render->DrawTexture(arrowTex, app->player->playerData.position.x+10, app->player->playerData.position.y - 90, &rectGUI);

	}
	rectGUI = arrowAnim->GetCurrentFrame();
	if (app->player->GetCheckPointMove())
	{
		LOG("puedo moverme entre CheckPoints");
		app->render->DrawTexture(arrowTex, app->player->playerData.position.x + 60, app->player->playerData.position.y - 40, &rectGUI);
		app->render->DrawTextureFlip(arrowTex,app->player->playerData.position.x - 45, app->player->playerData.position.y - 40, &rectGUI);

	}
	return true;
}

bool GUI::CleanUp()
{
	if (!active){
		return true;
	}
	app->tex->UnLoad(headTex);
	app->tex->UnLoad(heartTex);

	active = false;

	return true;
}

