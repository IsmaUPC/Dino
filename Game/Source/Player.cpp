#include "Player.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Audio.h"
#include "ModuleFadeToBlack.h"
#include "cmath"
//#include "External/SDL_draw-1.2.13/include/SDL_draw.h"

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

	
	playerData.texture = app->tex->Load("Assets/Textures/Dino_GreenUnit.png");
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

	playerData.isJumped = false;
	playerData.state = IDLE;
	playerData.direction = WALK_R;

	idleAnim->loop = true;
	idleAnim->speed = 0.05f;
	/*
	walkAnim->loop = true;
	walkAnim->speed = 0.08f;
	damageAnim->loop = false;
	damageAnim->speed = 0.005f;
	runAnim->loop = true;
	runAnim->speed = 0.10f;
	
	atakAnim->loop = false;
	atakAnim->speed = 0.10f;

	jumpAnim->loop = true;
	jumpAnim->speed = 0.24f;
	
	for (int i = 0; i < 4; i++)
		idleAnim->PushBack({ 78 * i,0, 78, 78 });
	
	for (int i = 0; i < 6; i++)
		jumpAnim->PushBack({ 312 + (78 * i),0, 78, 78 });

	for (int i = 0; i < 6; i++)
		walkAnim->PushBack({ 312 + (78 * i),0, 78, 78 });

	for (int i = 0; i < 3; i++)
		atakAnim->PushBack({ 858 + (78 * i),0, 78, 78 });

	for (int i = 0; i < 4; i++)
		damageAnim->PushBack({ 1008 + (78 * i),0, 78, 78 });

	//for (int j = 0; j < 1; j++)
		for (int i = 2; i < 4; i++)
			damageAnim->PushBack({ 1008 + (78 * i),0, 78, 78 });

	for (int i = 0; i < 4; i++)
		runAnim->PushBack({ 1319 + (78 * i),0, 78, 78 });
	*/  
	idleAnim->PushBack({ 0,0, 48, 54 });
	playerData.currentAnimation = idleAnim;
	//velX = playerData.velocity;

	app->entityManager->AddEntity(FIREBALL, 0, 0);
	app->entityManager->AddEntity(HUD, 0, 0);

	//playerData.ship.acceleration = { 0,0 };
	playerData.ship.velocity = { 0,0 };
	playerData.ship.angularVelocity = 0;
	playerData.ship.position = positionInitial;
	playerData.ship.angularPosition = 0;
	playerData.ship.mass = 200;
	//playerData.pointsCollision[5] = { {0,0},{0,0},{0,0},{0,0},{0,0} };
	fPoint positionP = playerData.ship.position;
	playerData.pointsCollision[0] = { positionP.x,positionP.y };
	playerData.pointsCollision[1] = { positionP.x+24,positionP.y };
	playerData.pointsCollision[2] = { positionP.x+48,positionP.y };
	playerData.pointsCollision[3] = { positionP.x+48,positionP.y-54 };
	playerData.pointsCollision[4] = { positionP.x,positionP.y-54 };
	//levelScene = app->fade->GetLastLevel()->GetNumThisScene();
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
		playerData.ship.position.x = player.child("position").attribute("x").as_int(playerData.ship.position.x);
		playerData.ship.position.y = player.child("position").attribute("y").as_int(playerData.ship.position.y);
	return ret;
}

bool Player::SaveState(pugi::xml_node& player) const
{
	pugi::xml_node positionPlayer = player.child("position");

	positionPlayer.attribute("x").set_value(playerData.ship.position.x) ;
	positionPlayer.attribute("y").set_value( playerData.ship.position.y) ;
	player.child("level").attribute("lvl").set_value(levelScene);
	return true;
}

bool Player::PreUpdate() 
{
	PlayerMoveAnimation();

	return true;
}

bool Player::Update(float dt) 
{
	SpeedAnimationCheck(dt);
	//playerData.velocity = floor(1000 * dt) / 4;
	gravity = ceil(600 * dt);

	

	//Linear forces
	//Call all forces
	//playerData.ship.AddForces(playerData.ForceGrav(playerData.ship.mass));
	fPoint velAir = { 13,-7.5 };//15m/s con un angulo de -30º
	fPoint velRelative = playerData.ship.velocity - velAir;
	float velRelativeM = sqrt((velRelative.x * velRelative.x) + (velRelative.y * velRelative.y));
	//16m^2, intentar cambiar a surperficie variable
	playerData.ship.AddForces(playerData.ForceAeroDrag(playerData.ship.velocity,1.225,velRelativeM,16,0.82));

	//Calculate Acceleration
	playerData.ship.CalculateAcceleration();

	//Integrate
	playerData.ship.position.x=PIXEL_TO_METERS(playerData.ship.position.x);
	playerData.ship.position.y=PIXEL_TO_METERS(playerData.ship.position.y);
	//cambiar el 0.016 a dt de la función VelocityVerletLinear
	playerData.VelocityVerletLinear(playerData.ship.position, playerData.ship.velocity, playerData.ship.acceleration, 0.016);
	playerData.ship.position.x = METERS_TO_PIXELS(playerData.ship.position.x);
	playerData.ship.position.y = METERS_TO_PIXELS(playerData.ship.position.y);

	//Reset forces
	playerData.ship.ResetForces();

	//Angular Forces
	//CalculateAcceleration
	fPoint distance;
	distance.x = PIXEL_TO_METERS(playerData.pointsCollision[3].x) * 10;//pasamos de pixeles a metros
	distance.y = PIXEL_TO_METERS(playerData.pointsCollision[3].y) * 10;//ajustar el 10 a la distancia que se quiere aplicar el torque
	playerData.ship.CalculateAngularAcceleration(sqrt((distance.x * distance.x) + (distance.y * distance.y)));

	//Integrate
	//cambiar el 0.016 por dt
	playerData.VelocityVerletAngular(playerData.ship.angularPosition, playerData.ship.angularVelocity, playerData.ship.angularAcceleration, 0.016);
	
	//RotateBody
	fPoint axis = { playerData.ship.position.x+24, playerData.ship.position.y +27};
	playerData.ship.RotateBody(playerData.pointsCollision, playerData.numPoints, axis);

	//Reset torque
	playerData.ship.ResetTorque();

	MoveHit();
	//GravityDown(dt);	
	CameraPlayer();
	if (playerData.state == ATTACK && playerData.currentAnimation->HasFinished())
	{
		playerData.state = IDLE;
		//atakAnim->Reset();
	}
	if (playerData.state!=HIT && playerData.state != DEAD && playerData.state != ATTACK)
	{
		// Move player inputs control
		if (!checkpointMove)PlayerControls(dt);
		//Move Between CheckPoints
		else MoveBetweenCheckPoints();
	}
	return true;
}

void Player::MoveHit()
{
	if (app->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)SetHit();

	//if (playerData.currentAnimation == damageAnim) {
	//	if (!playerData.currentAnimation->HasFinished())
	//	{
	//		tmp = playerData.ship.position;
	//		playerData.ship.position.x -= 2 * playerData.direction;
	//		if (!jumpHit)
	//		{
	//			jumpHit = true;
	//			if (app->GetFramerate() == 60)	velY = -10.5f;
	//			else if (app->GetFramerate() == 30) velY = -21;
	//		}
	//		if (CollisionPlayer(playerData.ship.position)) playerData.ship.position = tmp;
	//	}
	//	else
	//	{			
	//		//playerData.ship.position = IPointMapToWorld(checkPoints.end->data);
	//		playerData.currentAnimation->Reset();
	//		playerData.state = IDLE;
	//		if(playerData.respawns <1)playerData.state = DEAD;
	//	}
	//}
}

void Player::GravityDown(float dt)
{
	if (godMode == false)
	{
		playerData.currentAnimation->Update();
		GravityDownCollision(dt);
	}
	else playerData.currentAnimation = idleAnim;
}

void Player::SpeedAnimationCheck(float dt)
{

		idleAnim->speed = (dt * 5) ;
		/*walkAnim->speed = (dt * 9) ;
		jumpAnim->speed = (dt * 18) ;
		atakAnim->speed = (dt * 5) ;
		damageAnim->speed = (dt * 10) ;
		runAnim->speed = (dt * 9) ;*/
	
}

void Player::MoveBetweenCheckPoints()
{
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		if ((lastCP + 1) >= checkPoints.Count()) lastCP = 0;
		else lastCP++;
		//playerData.ship.position = IPointMapToWorld(checkPoints.At(lastCP)->data);
		app->render->camera.x = cameraPosCP.At(lastCP)->data.x;
		app->render->camera.y = cameraPosCP.At(lastCP)->data.y;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		if (lastCP == 0) lastCP = checkPoints.Count() - 1;
		else lastCP--;
		//playerData.ship.position = IPointMapToWorld(checkPoints.At(lastCP)->data);
		app->render->camera.x = cameraPosCP.At(lastCP)->data.x;
		app->render->camera.y = cameraPosCP.At(lastCP)->data.y;
	}
}

void Player::CameraPlayer()
{
	//Camera follow the player
	int followPositionPalyerX = (WINDOW_W / 2) + (playerData.ship.position.x * -1);
	int followPositionPalyerY = (WINDOW_H / 2) + (playerData.ship.position.y * -1) + 200;


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
		//playerData.currentAnimation = walkAnim;
		break;

	case JUMP:
		//playerData.currentAnimation = jumpAnim;
		break;

	case RUN:
		//playerData.currentAnimation = runAnim;
		break;
	
	case HIT:
		//playerData.currentAnimation = damageAnim;
		break;	
	
	case ATTACK:
		//playerData.currentAnimation = atakAnim;
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
		//velX = playerData.velocity * 2;
		playerData.state = State::RUN;
	}
		// Comprobamos si las teclas están pulsadas al mismo tiempo
	if (!(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		&& (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT))
	{
		if (playerData.state == State::IDLE || playerData.state == State::WALK) {
			playerData.state = State::WALK;
			//velX = playerData.velocity;
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			//MovePlayer(MoveDirection::WALK_R, dt);
			fPoint distance;
			distance.x = PIXEL_TO_METERS(playerData.pointsCollision[3].x)*10;//pasamos de pixeles a metros
			distance.y = PIXEL_TO_METERS(playerData.pointsCollision[3].y)*10;//ajustar el 10 a la distancia que se quiere aplicar el torque
			
			playerData.ship.AddTorque({ distance.y*100,-distance.x*100}, {distance.x,distance.y});
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			//MovePlayer(MoveDirection::WALK_L, dt);
			fPoint distance;
			distance.x = PIXEL_TO_METERS(playerData.pointsCollision[4].x) * 10;//pasamos de pixeles a metros
			distance.y = PIXEL_TO_METERS(playerData.pointsCollision[4].y) * 10;//ajustar el 10 a la distancia que se quiere aplicar el torque

			playerData.ship.AddTorque({ -distance.y * 100,distance.x * 100 }, { distance.x,distance.y });
			
		}

	}	// Any key is pressed or A and D pressed in same time, set player in IDLE state
	else if(playerData.state != JUMP) playerData.state = State::IDLE;

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		//Nothing
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		fPoint impuls;
		impuls.x = -PIXEL_TO_METERS(playerData.pointsCollision[1].x);
		impuls.y = -PIXEL_TO_METERS(playerData.pointsCollision[1].y);
		playerData.ship.AddForces({ 2000 * impuls.x,2000 * impuls.y });
	}

	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) playerData.state= ATTACK;


	// Player Jump
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) Jump();

	if (godMode == true)
	{
		//velX = playerData.velocity * 2;
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)playerData.ship.position.y -= velX;
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)playerData.ship.position.y += velX;
	}

}


void Player::MovePlayer(MoveDirection playerDirection, float dt)
{
	tmp = playerData.ship.position;
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

	if (CollisionPlayer(playerData.ship.position))playerData.ship.position = tmp;
	
}

void Player::MoveToDirection(int velocity)
{
	switch (playerData.direction)
	{	
	// Move in to correct direction
	case WALK_L:
		playerData.ship.position.x -= velocity;
		break;

	case WALK_R:
		playerData.ship.position.x += velocity;
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
		app->render->DrawTexture(playerData.texture, playerData.ship.position.x, playerData.ship.position.y, &rectPlayer,1.0f,playerData.ship.GetRotation());
	else if (playerData.direction == MoveDirection::WALK_L)
		app->render->DrawTextureFlip(playerData.texture, playerData.ship.position.x, playerData.ship.position.y , &rectPlayer,1.0f, playerData.ship.GetRotation());

	int x1 = playerData.pointsCollision[0].x;
	int y1 = playerData.pointsCollision[0].y;
	int x2 = playerData.pointsCollision[2].x;
	int y2 = playerData.pointsCollision[2].y;

	app->render->DrawLine(x1,y1,x2,y2,255,0,0);

	endUpdate = true;
	return true;
}

// Implements to gravity fall down
void Player::GravityDownCollision(float dt)
{
	//velY += gravity/10;

	tmp = playerData.ship.position;	
	lastState = playerData.state;

	//playerData.ship.position.y += velY;

	fPoint PlayerCurrPos = { playerData.ship.position.x  ,playerData.ship.position.y};
	bool fallingCollision = false;
	bool feedCollision = false;
	for (int i = 0; i < playerData.numPoints; i++)
	{
		//if (CollisionJumping(iPoint{ PlayerCurrPos.x + playerData.pointsCollision[i].x ,PlayerCurrPos.y + playerData.pointsCollision[i].y }))
			//fallingCollision = true;		
		if (fallingCollision && (i == 0 || i == 1))feedCollision = true;
	}
	if (fallingCollision)
	{	
		if (feedCollision)
		{
			jumpHit = false;
			playerData.isJumped = false;
			playerData.isJumpedAgain = false;
			if (playerData.state != JUMP)playerData.state = lastState;
			else playerData.state = IDLE;
		}
		playerData.ship.position = tmp;
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

bool Player::CollisionPlayer(fPoint nextPosition) 
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

void Player::Jump()
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
		if (playerData.ship.position.y > 1720)
		{
			//isDead = true;
			playerData.respawns--;
			//if (playerData.respawns < 1)
			playerData.state = DEAD;

			if (playerData.state != DEAD) {
				//playerData.ship.position = IPointMapToWorld(checkPoints.end->data);
				app->render->camera.x = cameraPosCP.end->data.x;
				app->render->camera.y = cameraPosCP.end->data.y;
			}
			//return true;
		}
	}
	if (level == 2)
	{
		if (playerData.ship.position.y > 1968)
		{
			playerData.respawns--;
			if (playerData.respawns < 1)playerData.state = DEAD;

			if (playerData.state != DEAD) {
				//playerData.ship.position = IPointMapToWorld(checkPoints.end->data);
				app->render->camera.x = cameraPosCP.end->data.x;
				app->render->camera.y = cameraPosCP.end->data.y;
			}
		}
	}
	return false;
}

void Player::SetHit()
{
	if (playerData.respawns > 0 && playerData.state != HIT) {
		playerData.respawns--;
		playerData.state = HIT;
		hitDirection = playerData.direction;
	}
	
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

