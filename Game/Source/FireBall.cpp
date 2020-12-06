#include "FireBall.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

FireBall::FireBall() : Entity()
{
	name.Create("Fire");
}

FireBall::~FireBall()
{
}

bool FireBall::Awake(pugi::xml_node&)
{
	return true;
}

bool FireBall::Start()
{
	entityData->velocity = 12;
	startPos = { -100,-100 };

	entityData->texture = app->tex->Load("Assets/Textures/fire_ball.png");
	SDL_QueryTexture(entityData->texture, NULL, NULL, &texW, &texH);
	texW = texW / 5;


	fireBallAnim->speed = 0.3;
	for (int i = 0; i < 5; i++) {
		fireBallAnim->PushBack({ texW * i, 0, texW, texH });
	}

	entityData->pointsCollision = new iPoint[4]{ { 0, 0 }, { texW , 0 }, { texW,-texH }, { 0 ,-texH } };

	cooldown = 2;
	app->player->SetStateShoot(&stateShoot);

	return true;
}

bool FireBall::PreUpdate()
{
	return true;
}

bool FireBall::Update(float dt)
{
	
	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
		Shoot();
	}
	switch (stateShoot)
	{
	case CAN_SHOOT:
		return true;
		break;
	case SHOOT:
		fireBallAnim->Update();
		if (direc == 0)
			entityData->position.x += entityData->velocity;
		else
			entityData->position.x -= entityData->velocity;

		if (frameTime.ReadSec()>=cooldown) 
		{
			BackToPos0();
		}
		break;
	case WAIT:
		if (frameTime.ReadSec() >=cooldown) 
		{
			BackToPos0();
			stateShoot = CAN_SHOOT;
			entityData->fireBallState = stateShoot;
		}
		break;
	default:
		break;
	}

	return true;
}

bool FireBall::PostUpdate()
{

	SDL_Rect rect;
	rect = fireBallAnim->GetCurrentFrame();
	if (direc==0)
		app->render->DrawTexture(entityData->texture, entityData->position.x, entityData->position.y, &rect);	
	else
		app->render->DrawTextureFlip(entityData->texture, entityData->position.x, entityData->position.y, &rect);


	return true;
}

bool FireBall::CleanUp()
{
	if (!active) {
		return true;
	}
	app->tex->UnLoad(entityData->texture);

	active = false;
	pendingToDelete = true;

	return true;
}

void FireBall::SetPosition(iPoint pos)
{
	entityData->position.x = pos.x;
	entityData->position.y = pos.y;
}

void FireBall::BackToPos0()
{
	entityData->position = startPos;
	stateShoot = WAIT;
	entityData->fireBallState = stateShoot;
}

void FireBall::Shoot()
{
	if (stateShoot == CAN_SHOOT){
		stateShoot = SHOOT;
		entityData->fireBallState = stateShoot;
		frameTime.Start();
		entityData->position = app->player->playerData.position;
		entityData->position.y -= 30;
		direc = app->player->playerData.direction;
	}
}

//TODO: llamar a Shoot desde el player a ver si hace algo