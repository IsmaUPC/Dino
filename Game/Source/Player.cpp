#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Player.h"

#include "Input.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

Player::Player() : Module()
{
    name.Create("player");

}

Player::~Player()
{}

bool Player::Start() {

	playerData.texture = app->tex->Load("Assets/textures/Dino_Green.png");
	playerData.position = positionInitial;

	return true;
}

bool Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player Parser");
	bool ret = true;
	
	playerData.velocity = 1;
	//playerData.position = { 432,1170 };
	playerData.isJumped = false;

	idleAnim->loop = true;
	idleAnim->speed = 0.025f;
	walkAnim->loop = true;
	walkAnim->speed = 0.04f;
	damageAnim->loop = true;
	damageAnim->speed = 0.025f;
	runAnim->loop = true;
	runAnim->speed = 0.08f;
	
	jumpAnim->loop = true;
	jumpAnim->speed = 0.12f;

	for (int i = 0; i < 4; i++)
		idleAnim->PushBack({ 78 * i,0, 78, 78 });

	for (int i = 0; i < 6; i++)
		jumpAnim->PushBack({ 312 + (78 * i),0, 78, 78 });

	for (int i = 0; i < 6; i++)
		walkAnim->PushBack({ 312 + (78 * i),0, 78, 78 });

	for (int i = 0; i < 3; i++)
		atakAnim->PushBack({ 480 + (78 * i),0, 78, 78 });

	for (int i = 0; i < 4; i++)
		damageAnim->PushBack({ 1008 + (78 * i),0, 78, 78 });
	
	for (int i = 0; i < 4; i++)
		runAnim->PushBack({ 1319 + (78 * i),0, 78, 78 });

	playerData.currentAnimation = idleAnim;
	return ret;
}


bool Player::LoadState(pugi::xml_node& player) 
{
	bool ret=true;
		playerData.position.x = player.child("position").attribute("x").as_int(playerData.position.x);
		playerData.position.y = player.child("position").attribute("y").as_int(playerData.position.y);
	return ret;
}
bool Player::SaveState(pugi::xml_node& player) const
{
	pugi::xml_node positionPlayer = player.child("position");


	positionPlayer.attribute("x").set_value(playerData.position.x) ;
	positionPlayer.attribute("y").set_value( playerData.position.y) ;

	return true;
}


bool Player::PreUpdate() {

	return true;
}

bool Player::Update(float dt) {

	if(godMode==false)Fallings();

	if (godMode == false)playerData.currentAnimation->Update();
	else playerData.currentAnimation = idleAnim;

	int followPositionPalyerX = (WINDOW_W / 2) + (playerData.position.x * -1);
	int followPositionPalyerY = (WINDOW_H / 2) + (playerData.position.y * -1)+200;

	if (app->render->camera.x <= -1 && app->render->camera.x >= -((app->map->data.width * app->map->data.tileWidth)- WINDOW_W))
		app->render->camera.x = followPositionPalyerX;
	else if (followPositionPalyerX<-1 && followPositionPalyerX>-((app->map->data.width * app->map->data.tileWidth) - WINDOW_W))
		app->render->camera.x = followPositionPalyerX;

	if (app->render->camera.y <= -48 && app->render->camera.y >= -((app->map->data.height * app->map->data.tileHeight) - (WINDOW_H+(4* app->map->data.tileHeight))))
			app->render->camera.y = followPositionPalyerY;
		else if (followPositionPalyerY<-48 && followPositionPalyerY>-((app->map->data.height * app->map->data.tileHeight)-(WINDOW_H+(4 * app->map->data.tileHeight))))
			app->render->camera.y = followPositionPalyerY;


	// Move player inputs control
		PlayerControls();

	return true;
}

void Player::PlayerMoveAnimation()
{
	switch (playerData.state)
	{
	case IDLE:
		playerData.currentAnimation = idleAnim;
		break;

	case WALK:
		playerData.currentAnimation = walkAnim;
		break;

	case JUMP:
		playerData.currentAnimation = jumpAnim;
		break;

	case RUN:
		playerData.currentAnimation = runAnim;
		break;

	default:
		break;
	}
}

void Player::PlayerControls()
{
	//MovePlayer();
	
		// Comprobamos si las tecas están pulsadas al mismo tiempo
	if (!(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		&& (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT))
	{
		if (playerData.state == State::IDLE)
		{
			playerData.state = State::WALK;
			velX = playerData.velocity ;

		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)MovePlayer(MoveDirection::WALK_R);
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)MovePlayer(MoveDirection::WALK_L);
	}	// Any key is pressed or A and D pressed in same time, set player in IDLE state
	else if(playerData.state== State::IDLE) playerData.state = State::IDLE;

	// Player Jump
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)Jump();

	if (godMode == true)
	{
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)playerData.position.y -= playerData.velocity;
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)playerData.position.y += playerData.velocity;
	}

	// Player Run
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT&& playerData.state == State::WALK)
	{
			playerData.state = State::RUN;
		velX = playerData.velocity * 2;
	}

	PlayerMoveAnimation();
}

void Player::Jump()
{
	if (playerData.isJumped && !playerData.isJumpedAgain)
	{
		velY = -1.75;
		playerData.isJumpedAgain = true;
	}
	if (!playerData.isJumped)
	{
		velY = -2.5;
		playerData.isJumped = true;
	}

	playerData.state = State::JUMP;
	MovePlayer(playerData.direction);
}

void Player::MovePlayer(MoveDirection playerDirection)
{
	tmp =(iPoint) playerData.position;
	playerData.direction = playerDirection;

	switch (playerData.state)
	{
	case IDLE:
		break;	

	case JUMP:

		MoveToDirection(velX);
		break;

	case WALK:
		MoveToDirection(velX);
		break;

	case RUN:
		MoveToDirection(velX);
		break;

	default:
		break;
	}

	if (CollisionPlayer(playerData.position))playerData.position = tmp;
}

void Player::MoveToDirection(int velocity) {
	switch (playerData.direction)
	{	

	case WALK_L:
		playerData.position.x -= velocity;
		break;
	case WALK_R:
		playerData.position.x += velocity;
		break;

	default:
		break;
	}
}

void Player::Fallings()
{	
	tmp = (iPoint)playerData.position;

	nextYDown = (playerData.position.y + velY);
	nextYUp= (playerData.position.y - velY);
	// Horizontal Collision
	if (!CollisionPlayer({ playerData.position.x + playerData.velocity,  nextYDown }) || !CollisionPlayer({ playerData.position.x - playerData.velocity,  nextYDown }))
	{
		playerData.position.y += velY;
		velY += 0.035f;
	}
	else {
 		velY = (int)0;
	} // Verctical collision
	if (CollisionJumping({ playerData.position.x + playerData.velocity,  nextYDown }))
	{
		playerData.isJumped = false;
		playerData.isJumpedAgain = false;
		playerData.state = State::IDLE;

	}
	if (CollisionPlayer(playerData.position))playerData.position = tmp;
}

bool Player::PostUpdate() {

	SDL_Rect rectPlayer;
	rectPlayer = playerData.currentAnimation->GetCurrentFrame();
	if (playerData.direction == MoveDirection::WALK_R )
		app->render->DrawTexture(playerData.texture, playerData.position.x -15, playerData.position.y - (rectPlayer.h - 10), &rectPlayer);
	if (playerData.direction == MoveDirection::WALK_L)
		app->render->DrawTextureFlip(playerData.texture, playerData.position.x -15, playerData.position.y - (rectPlayer.h - 10), &rectPlayer);
	
	return true;
}

bool Player::CleanUp() {

	if (!active)
		return true;

	app->tex->UnLoad(playerData.texture);
	active = false;

	return true;
}

bool Player::CollisionPlayer(iPoint nextPosition) {

	iPoint positionMapPlayer;
	int y = (int)nextPosition.y;
	int x = (int)nextPosition.x;

	for (int i = 0; i < playerData.numPoints; i++)
	{
		positionMapPlayer = app->map->WorldToMap(x+playerData.pointsCollision[i][0], y+playerData.pointsCollision[i][1]);
		if (CheckCollision(positionMapPlayer)) return true;
	}

	return false;
}

bool Player::CollisionJumping(iPoint nextPosition) {

	iPoint positionMapPlayer;
	int y = (int)nextPosition.y;

	positionMapPlayer = app->map->WorldToMap((int)nextPosition.x+18, y);
	if (CheckCollision(positionMapPlayer)) return true;
	positionMapPlayer = app->map->WorldToMap((int)nextPosition.x+48-18, y);
	if (CheckCollision(positionMapPlayer)) return true;

	return false;
}

bool Player::CheckCollision(iPoint positionMapPlayer)
{
	if (godMode == false)
	{
		if (app->map->data.layers.At(2)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0)
		{
			return true;
		}
	}

	return false;
}

bool Player::CheckVictory(iPoint positionMapPlayer_)
{
	iPoint positionMapPlayer;
	int y = (int)positionMapPlayer_.y;

	positionMapPlayer = app->map->WorldToMap((int)positionMapPlayer_.x + 18, y);
	if (app->map->data.layers.At(2)->data->Get(positionMapPlayer.x, positionMapPlayer.y) == app->map->data.tilesets.At(2)->data->firstgid)
	{
		return true;
	}
	return false;
}

bool Player::CheckGameOver()
{
	if (playerData.position.y>1720)
	{
		isDead = true;
		return true;
	}
	return false;
}
