﻿#include "GUI.h"
#include "Player.h"
#include "FireBall.h"
#include "EntityManager.h"
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

	int imgH = 0;
	int imgW = 0;

	headTex = app->tex->Load("Assets/Textures/GUI/dino_head.png");
	SDL_QueryTexture(headTex, NULL, NULL, &headW, &imgH);

	headAnim->PushBack({0,0,headW,imgH });

	imgH = 0;
	imgW = 0;
	arrowTex = app->tex->Load("Assets/Textures/GUI/arrows.png");
	SDL_QueryTexture(arrowTex, NULL, NULL, &imgW, &imgH);
	imgW = imgW / 2;

	arrowAnim->PushBack({ 0,0,imgW,imgH });
	buttonEAnim->PushBack({ imgW,0,imgW,imgH });


	imgCoin = app->tex->Load("Assets/Textures/GUI/coin.png");

	imgH = 0;
	imgW = 0;
	fireBallTex = app->tex->Load("Assets/Textures/GUI/fire_ball.png");
	SDL_QueryTexture(fireBallTex, NULL, NULL, &imgW, &imgH);
	imgH = imgH / 2;

	fireBallOnAnim->PushBack(	{ 0,0,imgW,imgH		});
	fireBallOffAnim->PushBack(	{ 0,imgH,imgW,imgH	});

	respawn = &app->player->playerData.respawns;
	coins = &app->player->playerData.coins;

	//Text
	hudFont = app->fonts->Load("Assets/Textures/GUI/hud_font.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.,0123456789им?!*$%&()+-/:;<=>@__     ", 5, 705, 225);

	fireBallState = app->player->playerData.stateShoot;

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


	//Respawns
	SDL_Rect rectGUI;
	rectGUI = headAnim->GetCurrentFrame();
	for (int i = 0; i < *respawn; i++)
		app->render->DrawTexture(headTex,point0.x +((headW + headSeparation)*i),point0.y,&rectGUI);
	
	//ChecKPoints HUD
	rectGUI = buttonEAnim->GetCurrentFrame();
	if (app->player->GetInCheckPoint())
		app->render->DrawTexture(arrowTex, app->player->playerData.position.x+10, app->player->playerData.position.y - 90, &rectGUI);
	
	rectGUI = arrowAnim->GetCurrentFrame();
	if (app->player->GetCheckPointMove())
	{
		app->render->DrawTexture(arrowTex, app->player->playerData.position.x + 60, app->player->playerData.position.y - 40, &rectGUI);
		app->render->DrawTextureFlip(arrowTex,app->player->playerData.position.x - 45, app->player->playerData.position.y - 40, &rectGUI);
	}

	//Coins
	point0.x = -app->render->camera.x;
	point0.y = -app->render->camera.y;

	app->render->DrawTexture(imgCoin, point0.x+22, point0.y + 90);

	point0.x = point0.x + 90;
	point0.y = point0.y + 100;

	sprintf_s(coinText,7," x%3d",*coins);

	app->fonts->BlitText(point0.x, point0.y, hudFont, coinText);

	point0.x = -app->render->camera.x;
	point0.y = -app->render->camera.y;
	point0.x = point0.x + (WINDOW_W - 100);
	point0.y = point0.y + (WINDOW_H - 100);
	//FireBall
	if (*fireBallState == 0)
	{
		LOG("Puedes disparar");
		rectGUI = fireBallOnAnim->GetCurrentFrame();
		app->render->DrawTexture(fireBallTex, point0.x, point0.y,&rectGUI);
	}
	else
	{
		LOG("No puedes disparar");
		rectGUI = fireBallOffAnim->GetCurrentFrame();
		app->render->DrawTexture(fireBallTex, point0.x, point0.y, &rectGUI);
	}

	return true;
}

bool GUI::CleanUp()
{
	if (!active){
		return true;
	}
	app->tex->UnLoad(headTex);
	app->tex->UnLoad(arrowTex);
	app->tex->UnLoad(imgCoin);

	active = false;

	pendingToDelete = true;
	return true;
}

