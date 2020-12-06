#include "FireBall.h"
#include "Player.h"
#include "Audio.h"

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
	entityData->position = startPos;
	entityData->texture = app->tex->Load("Assets/Textures/fire_ball.png");
	SDL_QueryTexture(entityData->texture, NULL, NULL, &texW, &texH);
	texW = texW / 5;

	fireBallAnim->speed = 0.3;
	for (int i = 0; i < 5; i++) {
		fireBallAnim->PushBack({ texW * i, 0, texW, texH });
	}

	explsionTex = app->tex->Load("Assets/Textures/fire_ball_explosion.png");
	int imgW, imgH;
	SDL_QueryTexture(explsionTex, NULL, NULL, &imgW, &imgH);
	imgW = imgW / 2;

	explosionAnim->loop = false;
	explosionAnim->speed = 0.65;
	for (int i = 0; i < 2; i++) {
		explosionAnim->PushBack({ imgW * i, 0, imgW, imgH });
	}
	explosionAnim->PushBack({ 0, 0, 0, 0 });

	entityData->pointsCollision = new iPoint[4]{ { 0, 0 }, { texW , 0 }, { texW,-texH }, { 0 ,-texH } };

	hitFx = app->audio->LoadFx("Assets/Audio/Fx/hit.wav");

	cooldown = 2;
	startexplosion = false;

	app->player->SetStateShoot(&stateShoot);
	app->player->SetPositionShoot(&entityData->position);
	app->player->SetCollidersShoot(entityData->pointsCollision);
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

	if (lastState == SHOOT && stateShoot == WAIT){
		explosionAnim->Reset();
		explosionPos = entityData->position;
		app->audio->PlayFx(hitFx);
		BackToPos0();
	}

	switch (stateShoot)
	{
	case CAN_SHOOT:
		return true;
		break;
	case SHOOT:
		fireBallAnim->Update();
		if (direc == MoveDirection::WALK_R)
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
	lastState = stateShoot;

	if (startexplosion)
		explosionAnim->Update();
	

	return true;
}

bool FireBall::PostUpdate()
{

	SDL_Rect rect;
	rect = fireBallAnim->GetCurrentFrame();
	if (direc== MoveDirection::WALK_R)
		app->render->DrawTexture(entityData->texture, entityData->position.x, entityData->position.y, &rect);	
	else
		app->render->DrawTextureFlip(entityData->texture, entityData->position.x, entityData->position.y, &rect);

	if (startexplosion)
	{
		if (explosionAnim->HasFinished()) {
			startexplosion = false;
		}
		rect = explosionAnim->GetCurrentFrame();

		app->render->DrawTexture(explsionTex, explosionPos.x, explosionPos.y, &rect);

	}

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
	startexplosion = true;
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