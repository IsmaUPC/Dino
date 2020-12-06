﻿#include "GUI.h"
#include "Player.h"
#include "ModuleFonts.h"

#include "Defs.h"
#include "Log.h"

GUI::GUI() : Entity()
{
	name.Create("GUI");
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
	headTex = app->tex->Load("Assets/Textures/GUI/dino_head.png");
	SDL_QueryTexture(headTex, NULL, NULL, &headW, &headH);

	headAnim->PushBack({0,0,headW,headH });

	arrowTex = app->tex->Load("Assets/Textures/GUI/arrows.png");
	SDL_QueryTexture(arrowTex, NULL, NULL, &arrowW, &arrowH);
	arrowW = arrowW / 2;

	arrowAnim->PushBack({ 0,0,arrowW,arrowH });
	buttonEAnim->PushBack({ arrowW,0,arrowW,arrowH });

	respawn = &app->player->playerData.respawns;

	//Text
	hudFont = app->fonts->Load("Assets/Textures/GUI/hud_font.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.,0123456789им?!*$%&()+-/:;<=>@__     ", 5, 705, 225);


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
	point0.x = -app->render->camera.x;
	point0.y = -app->render->camera.y;
	
	point0.x = point0.x + headPositionX;
	point0.y = point0.y + headPositionY;

	SDL_Rect rectGUI;
	rectGUI = headAnim->GetCurrentFrame();
	for (int i = 0; i < *respawn; i++)
		app->render->DrawTexture(headTex,point0.x +((headW + headSeparation)*i),point0.y,&rectGUI);
	
	rectGUI = buttonEAnim->GetCurrentFrame();
	if (app->player->GetInCheckPoint())
		app->render->DrawTexture(arrowTex, app->player->playerData.position.x+10, app->player->playerData.position.y - 90, &rectGUI);
	
	rectGUI = arrowAnim->GetCurrentFrame();
	if (app->player->GetCheckPointMove())
	{
		app->render->DrawTexture(arrowTex, app->player->playerData.position.x + 60, app->player->playerData.position.y - 40, &rectGUI);
		app->render->DrawTextureFlip(arrowTex,app->player->playerData.position.x - 45, app->player->playerData.position.y - 40, &rectGUI);
	}


	//Text
	point0.x = -app->render->camera.x;
	point0.y = -app->render->camera.y;

	point0.x = point0.x + 100;
	point0.y = point0.y + 100;

	sprintf_s(coinText,5,"%3d",coinCount);

	app->fonts->BlitText(point0.x, point0.y, hudFont, coinText);

	return true;
}

bool GUI::CleanUp()
{
	if (!active){
		return true;
	}
	app->tex->UnLoad(headTex);
	app->tex->UnLoad(arrowTex);

	active = false;

	pendingToDelete = true;
	return true;
}

