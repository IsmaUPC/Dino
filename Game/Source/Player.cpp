#include "Player.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Audio.h"

Player::Player() : Entity()
{
    name.Create("player");
}

Player::Player(TypeEntity pTypeEntity, iPoint pPosition, float pVelocity, SDL_Texture* pTexture) 
	: Entity(pTypeEntity, pPosition, pVelocity, pTexture)
{
	playerData.state = IDLE;
    name.Create("player");

}

Player::~Player()
{}

bool Player::Start() 
{
	iPoint pathInit =  app->map->WorldToMap(positionInitial.x ,positionInitial.y) ;
	app->map->ResetPath(pathInit);

	playerData.texture = app->tex->Load("Assets/Textures/dino_green.png");
	playerData.position = positionInitial;
	playerData.state = IDLE;

	//FX
	bonfireFx = app->audio->LoadFx("Assets/Audio/Fx/bonfire.wav");

	playerData.respawns = 3;
	playerData.coins = 0;
	playerData.lives = 3;

	inCheckPoint = false;
	checkpointMove = false;
	endUpdate = true;
	win = false;

	playerData.velocity = 1;
	playerData.isJumped = false;
	playerData.state = IDLE;
	playerData.direction = WALK_R;

	idleAnim->loop = true;
	idleAnim->speed = 0.05f;
	walkAnim->loop = true;
	walkAnim->speed = 0.08f;
	damageAnim->loop = true;
	damageAnim->speed = 0.05f;
	runAnim->loop = true;
	runAnim->speed = 0.10f;

	jumpAnim->loop = true;
	jumpAnim->speed = 0.24f;

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
	velX = playerData.velocity;

	app->entityManager->AddEntity(FIREBALL, 0, 0);
	app->entityManager->AddEntity(HUD, 0, 0);



	return true;
}

bool Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player Parser");
	bool ret = true;
	
	return true;
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

bool Player::PreUpdate() 
{
	
	return true;
}

bool Player::Update(float dt) 
{
	SpeedAnimationCheck(dt);
	playerData.velocity = (1000 * dt) / 3;
	gravity = ceil(600 * dt);


	if (godMode == false)
	{
		playerData.currentAnimation->Update();
		GravityDown(dt);
	}
	else playerData.currentAnimation = idleAnim;

	CameraPlayer();
	// Move player inputs control
	if (!checkpointMove)PlayerControls(dt);
	//Move Between CheckPoints
	else MoveBetweenCheckPoints();

	return true;
}

void Player::SpeedAnimationCheck(float dt)
{
	if (CheckChangeFPS(app->GetFramerate()))
	{
		idleAnim->speed = (dt * 100) * 0.0025f;
		walkAnim->speed = (dt * 100) * 0.004f;
		atakAnim->speed = (dt * 100) * 0.008f;
		damageAnim->speed = (dt * 100) * 0.008f;
		runAnim->speed = (dt * 100) * 0.008f;
		jumpAnim->speed = (dt * 100) * 0.008f;
		
	}
}

void Player::MoveBetweenCheckPoints()
{
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		if ((lastCP + 1) >= checkPoints.Count()) lastCP = 0;
		else lastCP++;
		playerData.position = IPointMapToWorld(checkPoints.At(lastCP)->data);
		app->render->camera.x = cameraPosCP.At(lastCP)->data.x;
		app->render->camera.y = cameraPosCP.At(lastCP)->data.y;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		if (lastCP == 0) lastCP = checkPoints.Count() - 1;
		else lastCP--;
		playerData.position = IPointMapToWorld(checkPoints.At(lastCP)->data);
		app->render->camera.x = cameraPosCP.At(lastCP)->data.x;
		app->render->camera.y = cameraPosCP.At(lastCP)->data.y;
	}
}

void Player::CameraPlayer()
{
	//Camera follow the player
	int followPositionPalyerX = (WINDOW_W / 2) + (playerData.position.x * -1);
	int followPositionPalyerY = (WINDOW_H / 2) + (playerData.position.y * -1) + 200;


	// Camera delimitation x
	if (app->render->camera.x <= -1 && app->render->camera.x >= -((app->map->data.width * app->map->data.tileWidth) - WINDOW_W))
		app->render->camera.x = followPositionPalyerX;
	else if (followPositionPalyerX<-1 && followPositionPalyerX>-((app->map->data.width * app->map->data.tileWidth) - WINDOW_W))
		app->render->camera.x = followPositionPalyerX;

	// Camera delimitation x
	if (app->render->camera.y <= -48 && app->render->camera.y >= -((app->map->data.height * app->map->data.tileHeight) - (WINDOW_H + (4 * app->map->data.tileHeight))))
		app->render->camera.y = followPositionPalyerY;
	else if (followPositionPalyerY<-48 && followPositionPalyerY>-((app->map->data.height * app->map->data.tileHeight) - (WINDOW_H + (4 * app->map->data.tileHeight))))
		app->render->camera.y = followPositionPalyerY;
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

void Player::PlayerControls(float dt)
{
		// Player Run
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT
		&& (playerData.state == State::WALK || playerData.state == State::RUN))
	{
		velX = playerData.velocity * 2;
		playerData.state = State::RUN;
	}
		// Comprobamos si las tecas est�n pulsadas al mismo tiempo
	if (!(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		&& (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT))
	{
		if (playerData.state == State::IDLE || playerData.state == State::WALK) {
			playerData.state = State::WALK;
			velX = playerData.velocity;
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)MovePlayer(MoveDirection::WALK_R, dt);
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)MovePlayer(MoveDirection::WALK_L, dt);

	}	// Any key is pressed or A and D pressed in same time, set player in IDLE state
	else playerData.state = State::IDLE;

	// Player Jump
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) 
	{
		Jump(dt);
	}

	if (godMode == true)
	{
		velX = playerData.velocity * 2;
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)playerData.position.y -= velX;
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)playerData.position.y += velX;
	}

	PlayerMoveAnimation();
}


void Player::MovePlayer(MoveDirection playerDirection, float dt)
{
	tmp = playerData.position;
	playerData.direction = playerDirection;

	switch (playerData.state)
	{
		
	case IDLE:
		// Future conditions in state IDLE...
		break;	

	case JUMP:
		// Move in state JUMP 
		MoveToDirection(velX);
		// Future conditions in state JUMP...
		break;

	case WALK:
		// Move in state WALK 
		MoveToDirection(velX);
		// Future conditions in state WALK...
		break;

	case RUN:
		// Move in state RUN 
		MoveToDirection(velX);
		// Future conditions in state RUN...
		break;

	default:
		break;
	}
	if (CollisionPlayer(playerData.position))playerData.position = tmp;
	
}

void Player::MoveToDirection(int velocity)
{
	switch (playerData.direction)
	{	
	// Move in to correct direction
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


iPoint Player::IPointMapToWorld(iPoint ipoint)
{
	iPoint CPos = app->map->MapToWorld(ipoint.x, ipoint.y);
	return CPos;
}

bool Player::PostUpdate() 
{
	SDL_Rect rectPlayer;
	rectPlayer = playerData.currentAnimation->GetCurrentFrame();
	// Draw player in correct direction
	if (playerData.direction == MoveDirection::WALK_R )
		app->render->DrawTexture(playerData.texture, playerData.position.x -15, playerData.position.y - (rectPlayer.h - 10), &rectPlayer);
	if (playerData.direction == MoveDirection::WALK_L)
		app->render->DrawTextureFlip(playerData.texture, playerData.position.x -15, playerData.position.y - (rectPlayer.h - 10), &rectPlayer);
	
	endUpdate = true;
	return true;
}

// Implements to gravity fall down
void Player::GravityDown(float dt)
{
	//velY += gravity/10;

	tmp = playerData.position;	
	lastState = playerData.state;

	playerData.position.y += velY;

	iPoint PlayerCurrPos = { playerData.position.x  ,playerData.position.y};
	bool fallingCollision = false;
	bool feedCollision = false;
	for (int i = 0; i < playerData.numPoints; i++)
	{
		if (CollisionJumping({ PlayerCurrPos.x + playerData.pointsCollision[i].x ,PlayerCurrPos.y + playerData.pointsCollision[i].y }))
			fallingCollision = true;		
		if (fallingCollision && (i == 0 || i == 1))feedCollision = true;
	}
	if (fallingCollision)
	{	
		if (feedCollision)
		{
			playerData.isJumped = false;
			playerData.isJumpedAgain = false;
			playerData.state = lastState;
		}
		playerData.position = tmp;
		velY = 0.0f;
		fallingCollision = false;
	}
	else{
		if(app->GetFramerate()==60)velY += 0.6f;
		else if(app->GetFramerate()==30) velY += 2.2f;
	}
}

bool Player::CleanUp() 
{
	if (!active)
		return true;

	app->tex->UnLoad(playerData.texture);
	active = false;

	checkPoints.Clear();
	cameraPosCP.Clear();
	return true;
}

bool Player::CollisionPlayer(iPoint nextPosition) 
{
	iPoint positionMapPlayer;
	int y = nextPosition.y;
	int x = nextPosition.x ;

	for (int i = 0; i < playerData.numPoints; i++)
	{	
		inCheckPoint = false;
		// Concvert position player WorldToMap 
		positionMapPlayer = app->map->WorldToMap(x+playerData.pointsCollision[i].x, y+playerData.pointsCollision[i].y);
		if (CheckCollision(positionMapPlayer)== COLLISION) return true;
	}
	return false;
}

bool Player::CollisionJumping(iPoint nextPosition)
{
	iPoint positionMapPlayer;
	int y = nextPosition.y;
	positionMapPlayer = app->map->WorldToMap(nextPosition.x, nextPosition.y);
	if (CheckCollision(positionMapPlayer)== COLLISION) return true;
	return false;
}

void Player::Jump(float dt)
{
	lastState = playerData.state;
	if (playerData.isJumped && !playerData.isJumpedAgain)
	{
		// Generate second impulse
		if (app->GetFramerate() == 60)velY = -9.75f;
		else if (app->GetFramerate() == 30) velY = -19.50;
		playerData.isJumpedAgain = true;
	}
	if (!playerData.isJumped)
	{
		// Generate first impulse
		if (app->GetFramerate() == 60)	velY = -10.5f;
		else if (app->GetFramerate() == 30) velY = -21;
		playerData.isJumped = true;
	}
	playerData.state = State::JUMP;
}


bool Player::CheckGameOver(int level)
{
	if (playerData.state==DEAD)
	{
		return true;
	}
	if (level == 1)
	{
		if (playerData.position.y > 1720)
		{
			//isDead = true;
			playerData.position = IPointMapToWorld(checkPoints.end->data);
			app->render->camera.x = cameraPosCP.end->data.x;
			app->render->camera.y = cameraPosCP.end->data.y;
			return true;
		}
	}
	if (level == 2)
	{
		if (playerData.position.y > 1968)
		{
			return true;
		}
	}
	return false;
}

void Player::SetHit()
{
	if (playerData.lives > 0) {
		playerData.respawns--;
		playerData.position = IPointMapToWorld(checkPoints.end->data);
	}
	else playerData.state = DEAD;
}

void Player::ActiveCheckpoint(iPoint positionMapPlayer)

{
	if (app->map->data.layers.At(2)->data->Get(positionMapPlayer.x, positionMapPlayer.y) == app->map->data.tilesets.At(2)->data->firstgid + 2)
	{
		for (int i = 0; i < checkPoints.Count(); i++)
		{
			if (checkPoints.At(i)->data == positionMapPlayer) {
				lastCP = i;
				if (checkPoints.Count() > 1){
					inCheckPoint = true;
					if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && endUpdate) {
						endUpdate = false;
						checkpointMove = !checkpointMove;
					}
				}
				return;
			}
		}
		//app->SaveGameRequest();
		checkPoints.Add(positionMapPlayer);
		iPoint cam(app->render->camera.x, app->render->camera.y);
		cameraPosCP.Add(cam);
		LOG("CHECKPOINT pos:%d,%d", positionMapPlayer.x, positionMapPlayer.y);
		app->map->CheckPointActive(positionMapPlayer);
		//FX
		app->audio->PlayFx(bonfireFx);
	}
}

