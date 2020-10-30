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

	idleAnim.loop = true;
	idleAnim.speed = 0.025;
	walkAnim->loop = true;
	walkAnim->speed = 0.04;
	damageAnim->loop = true;
	damageAnim->speed = 0.025;
	runAnim->loop = true;
	runAnim->speed = 0.025;

	

	for (int i = 0; i < 4; i++)
		idleAnim.PushBack({ 78 * i,0, 78, 78 });

	for (int i = 0; i < 6; i++)
		walkAnim->PushBack({ 312 + (78 * i),0, 78, 78 });

	for (int i = 0; i < 3; i++)
		atakAnim->PushBack({ 480 + (78 * i),0, 78, 78 });

	for (int i = 0; i < 4; i++)
		damageAnim->PushBack({ 576 + (78 * i),0, 78, 78 });

	for (int i = 0; i < 7; i++)
		runAnim->PushBack({ 768 + (78 * i),0, 78, 78 });

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

	playerData.currentAnimation->Update();
	int followPositionPalyerX = (WINDOW_W / 2) + (playerData.position.x * -1);
	int followPositionPalyerY = (WINDOW_H / 2) + (playerData.position.y * -1)+200;

	if (app->render->camera.x <= 0 && app->render->camera.x >= -1694)
		app->render->camera.x = followPositionPalyerX;
	else if (followPositionPalyerX<0 && followPositionPalyerX>-1694)  
		app->render->camera.x = followPositionPalyerX;

	if (app->render->camera.y <= -50 && app->render->camera.y >= -910)
		app->render->camera.y = followPositionPalyerY;
	else if (followPositionPalyerY<-50 && followPositionPalyerY>-910)
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

	case WALK_L:
		playerData.currentAnimation = walkAnim;

		break;

	case WALK_R:
		playerData.currentAnimation = walkAnim;
		break;

	default:
		break;
	}
}

void Player::PlayerControls()
{
	MovePlayer(playerData.state);
	
		// Comprobamos si las tecas están pulsadas al mismo tiempo
	if (!(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		&&(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT))
	{
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)MovePlayer(State::WALK_R);
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)MovePlayer(State::WALK_L);
	}
	else if(playerData.state!= State::JUMP) playerData.state = State::IDLE;

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) Jump();

	if (playerData.state == State::JUMP) MovePlayer(State::JUMP);

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)MovePlayer(State::WALK_UP);
	
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)MovePlayer(State::WALK_DOWN);

	PlayerMoveAnimation();


}

void Player::MovePlayer(State playerState)
{
	tmp =(iPoint) playerData.position;
	bool isJumping=(playerData.state==State::JUMP);


	switch (playerState)
	{
	case IDLE:
		if(!isJumping)playerData.state = State::IDLE;
		break;
	case WALK_L:
		if(!isJumping)playerData.state = State::WALK_L;
		playerData.rightDirection = false;
		playerData.position.x -= playerData.velocity;
		break;
	case WALK_R:
		if(!isJumping)playerData.state = State::WALK_R;
		playerData.rightDirection = true;
		playerData.position.x += playerData.velocity;
		break;
	case WALK_UP:
		playerData.state = State::IDLE;
		playerData.position.y -= playerData.velocity;
		break;
	case WALK_DOWN:
		playerData.state = State::IDLE;
		playerData.position.y += playerData.velocity;
		break;
	case JUMP:
		if (velY == 0)playerData.state = State::IDLE;
		break;
	default:
		break;
	}
	nextY = (playerData.position.y + velY);

	if (!CollisionPlayer({ playerData.position.x+playerData.velocity,  nextY})|| !CollisionPlayer({ playerData.position.x - playerData.velocity,  nextY }))
	{
		playerData.position.y += velY;
		velY += 0.02f;
	}
	else {
		velY = 0;
	}

	if (CollisionPlayer(playerData.position))playerData.position = tmp;
	if (CollisionPlayer(tmp))playerData.position = tmp;

}

bool Player::PostUpdate() {

	SDL_Rect rectPlayer;
	rectPlayer = playerData.currentAnimation->GetCurrentFrame();
	if (playerData.rightDirection) {
	app->render->DrawTexture(playerData.texture, playerData.position.x -15, playerData.position.y - (rectPlayer.h - 10), &rectPlayer);
	}else
	app->render->DrawTextureFlip(playerData.texture, (int) playerData.position.x -15, playerData.position.y - (rectPlayer.h-10), &rectPlayer);

	return true;
}

bool Player::CleanUp() {

	app->tex->UnLoad(playerData.texture);

	return true;
}

void Player::Jump() {

	if (playerData.state != State::JUMP) {
		velY = -2.5;
		playerData.state = State::JUMP;
	}


}

bool Player::CollisionPlayer(iPoint nextPosition) {

	iPoint positionMapPlayer;
	int y = (int)nextPosition.y;
	positionMapPlayer= app->map->WorldToMap((int)nextPosition.x, y);
	if (app->map->data.layers.At(2)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0)
	{
		return true;
	}
	return false;
}
