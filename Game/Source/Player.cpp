#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Player.h"

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
	idleAnim.loop = true;
	idleAnim.speed = 0.025;

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


	return true;
}

bool Player::PostUpdate() {

	SDL_Rect rectPlayer;
	rectPlayer = playerData.currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(playerData.texture, playerData.position.x, playerData.position.y, &rectPlayer);

	return true;
}

bool Player::CleanUp() {

	app->tex->UnLoad(playerData.texture);

	return true;
}
