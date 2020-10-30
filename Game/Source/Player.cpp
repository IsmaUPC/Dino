#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Player.h"
#include "Input.h"


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
	
	playerData.velocity = 2;
	playerData.position = { 432,1178 };

	idleAnim.loop = true;
	idleAnim.speed = 0.025;
	walkAnim->loop = true;
	walkAnim->speed = 0.08;
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
	app->render->camera.x= (WINDOW_W/2) + (playerData.position.x*-1)  ;
	app->render->camera.y= 	((WINDOW_H/2)+(playerData.position.y*-1))+100;


	// Comprobamos si las tecas están pulsadas al mismo tiempo
	if( !(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT))
	{
		
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				playerData.state = State::WALK_R;
				playerData.rightDirection = true;
				playerData.position.x += playerData.velocity;
			}

			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				playerData.state = State::WALK_L;
				playerData.rightDirection = false;
				playerData.position.x -= playerData.velocity;
			}
		}
		else
		{
			playerData.state = State::IDLE;

		}

	}
	else {
		playerData.state = State::IDLE;
	}


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

	return true;
}

bool Player::PostUpdate() {

	SDL_Rect rectPlayer;
	rectPlayer = playerData.currentAnimation->GetCurrentFrame();
	if (playerData.rightDirection) {
	app->render->DrawTexture(playerData.texture, playerData.position.x, playerData.position.y, &rectPlayer);
	}else
	app->render->DrawTextureFlip(playerData.texture, playerData.position.x, playerData.position.y, &rectPlayer);

	return true;
}

bool Player::CleanUp() {

	app->tex->UnLoad(playerData.texture);

	return true;
}
