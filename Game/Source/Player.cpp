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

	return true;
}

bool Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player Parser");
	bool ret = true;
	
	playerData.velocity = 1;
	playerData.position = { 432,1170 };
	playerData.isJumped = false;

	idleAnim.loop = true;
	idleAnim.speed = 0.025;
	walkAnim->loop = true;
	walkAnim->speed = 0.04;
	damageAnim->loop = true;
	damageAnim->speed = 0.025;
	runAnim->loop = true;
	runAnim->speed = 0.025;
	
	jumpAnim->loop = true;
	jumpAnim->speed = 0.12;

	

	for (int i = 0; i < 4; i++)
		idleAnim.PushBack({ 78 * i,0, 78, 78 });

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

	

	playerData.currentAnimation = &idleAnim;
	return ret;
}

bool Player::Load(const char* filename)
{
	bool ret = true;
	SString tmp("%s%s", folder.GetString(), filename);

	pugi::xml_parse_result result = playerFile.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
		ret = false;
	}

	// Load general info
	if (ret == true)
	{
		ret = LoadPlayer();
	}

	return ret;
}

bool Player::LoadPlayer() {

	bool ret = true;
	pugi::xml_node player ;
	//pugi::xml_node player = playerData.child("player");

	if (player == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		playerData.position.x = player.attribute("x").as_int(0);
		playerData.position.y = player.attribute("y").as_int(0);
	}

	return ret;

}


bool Player::PreUpdate() {

	return true;
}

bool Player::Update(float dt) {
	Fallings();

	playerData.currentAnimation->Update();
	int followPositionPalyerX = (WINDOW_W / 2) + (playerData.position.x * -1);
	int followPositionPalyerY = (WINDOW_H / 2) + (playerData.position.y * -1)+200;

	if (app->render->camera.x <= 0 && app->render->camera.x >= -1694)
		app->render->camera.x = followPositionPalyerX;
	else if (followPositionPalyerX<0 && followPositionPalyerX>-1694)  
		app->render->camera.x = followPositionPalyerX;

	if (app->render->camera.y <= -48 && app->render->camera.y >= -910)
		app->render->camera.y = followPositionPalyerY;
	else if (followPositionPalyerY<-48 && followPositionPalyerY>-910)
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
		playerData.currentAnimation = &idleAnim;
		break;

	case WALK:
		playerData.currentAnimation = walkAnim;
		break;

	case JUMP:
		playerData.currentAnimation = jumpAnim;
		break;

	case RUN:
		playerData.currentAnimation = jumpAnim;
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
		if (playerData.state != State::JUMP)playerData.state = State::WALK;
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)MovePlayer(MoveDirection::WALK_R);
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)MovePlayer(MoveDirection::WALK_L);
	}
	else if(playerData.state!= State::JUMP) playerData.state = State::IDLE;

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
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

	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) playerData.state=State::RUN;
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP) 
	{

		if (playerData.state != State::JUMP)playerData.state = State::RUN;
		if (playerData.state == MoveDirection::WALK_R) {
			playerData.position.x += playerData.velocity;
		}
		else
			playerData.position.x -= playerData.velocity;
	}


	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)MovePlayer(MoveDirection::WALK_UP);
	
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)MovePlayer(MoveDirection::WALK_DOWN);

	PlayerMoveAnimation();


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
		MoveToDirection(playerData.velocity);
		break;

	case WALK:
		MoveToDirection(playerData.velocity);
		break;

	case RUN:
		MoveToDirection(playerData.velocity * 2);
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

	case WALK_UP:
		playerData.position.y -= velocity;
		break;
	case WALK_DOWN:
		playerData.position.y += velocity;
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

	app->tex->UnLoad(playerData.texture);

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

	//positionMapPlayer= app->map->WorldToMap((int)nextPosition.x, y);
	//if (CheckCollision(positionMapPlayer)) return true;
	//positionMapPlayer = app->map->WorldToMap((int)nextPosition.x+48, y);
	//if (CheckCollision(positionMapPlayer)) return true;
	//positionMapPlayer = app->map->WorldToMap((int)nextPosition.x, y-54);
	//if (CheckCollision(positionMapPlayer)) return true;
	//positionMapPlayer = app->map->WorldToMap((int)nextPosition.x + 48, y-54);
	//if (CheckCollision(positionMapPlayer)) return true;

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
	if (app->map->data.layers.At(2)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0)
	{
		return true;
	}
	return false;
}
