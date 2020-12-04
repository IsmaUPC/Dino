#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "SceneLogo.h"
#include "Scene.h"
#include "SceneIntro.h"
#include "SceneWin.h"
#include "SceneLose.h"
#include "SceneLevel2.h"
#include "Map.h"
#include "Player.h"
//#include "Enemy.h"
//#include "Coins.h"
#include "Entity.h"
#include "EntityManager.h"
#include "ModuleFadeToBlack.h"
#include "Pathfinding.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;

	win = new Window();
	input = new Input();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	map = new Map();
	player = new Player();
	//enemy = new Enemy();
	entityManager = new EntityManager();
	//coins = new Coins();
	sceneLogo = new SceneLogo();
	scene = new Scene();
	sceneIntro = new SceneIntro();
	sceneWin = new SceneWin();
	sceneLose = new SceneLose();
	sceneLevel2 = new SceneLevel2();
	fade = new ModuleFadeToBlack(); 
	pathfinding = new PathFinding();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	AddModule(map);
	AddModule(player);
	//AddModule(enemy);
	AddModule(entityManager);
	//AddModule(coins);
	AddModule(sceneLogo);
	AddModule(scene);
	AddModule(sceneIntro);
	AddModule(sceneWin);
	AddModule(sceneLose);
	AddModule(sceneLevel2);
	AddModule(fade);
	AddModule(pathfinding);

	//actives
	sceneIntro->active = false;
	scene->active = false;
	sceneWin->active = false;
	sceneLose->active = false;
	sceneLevel2->active = false;
	player->active = false;
	//enemy->active = false;
	map->active = false;

	
	// Render last to swap buffer
	AddModule(render);
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.Add(module);
}

// Called before render is available
bool App::Awake()
{
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	bool ret = false;

	config = LoadConfig(configFile);

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		title.Create(configApp.child("title").child_value());
		organization.Create(configApp.child("organization").child_value());
		
		// Obtenemos el Frame Rate
		framerate = configApp.attribute("framerate_cap").as_int(0);
		//if (framerate > 0)	cappedMs = framerate *0.001 ;
		if (framerate > 0)	cappedMs = 1000/framerate  ;
		
	}

	if (ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while ((item != NULL) && (ret == true))
		{

			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	return ret;
}

// Called before the first frame
bool App::Start()
{
	startupTime.Start();
	lastSecFrameTime.Start();
	frameTime.Start();

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		if (item->data->active == true)
			ret = item->data->Start();
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();
	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// Load config from XML file
// NOTE: Function has been redesigned to avoid storing additional variables on the class
pugi::xml_node App::LoadConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description());
	else ret = configFile.child("config");

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	frameCount++;
	lastSecFrameCount++;

	// Calculate the dt: differential time since last frame
	// dt = (oldLastFrame - lastFrameMs) / 1000;
	dt = frameTime.ReadSec();
	frameTime.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	if (saveGameRequested == true) SaveGame(filenameGame.GetString());
	if (loadGameRequested == true) LoadGame(filenameGame.GetString());

	if (loadConfigRequested == true) LoadGame(filenameConfig.GetString());
	if (saveConfigRequested == true) SaveGame(filenameConfig.GetString());

	float secondsSinceStartup = startupTime.ReadSec();
	float averageFps = frameCount / secondsSinceStartup;

	timeFramesSecond += dt;
	if (timeFramesSecond > 1.0f)
	{
		framesOnLastSecond = lastSecFrameCount;
		lastSecFrameCount = 0;
		timeFramesSecond = 0;
		//frameTime.Start();
	}

	oldLastFrame = lastFrameMs;
	lastFrameMs = lastSecFrameTime.Read();
	lastSecFrameTime.Start();
	
	if ((cappedMs > 0.000f) && (lastFrameMs < cappedMs))
	{
		//ptimer.Start();
		// Delay delimitation
		SDL_Delay(cappedMs);

		//perfTime = ptimer.ReadMs();
		//LOG("We waited for %f milliseconds and got back in %f milliseconds", cappedMs, perfTime);

	}

	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %02u Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %I64u ",
		averageFps, lastFrameMs, framesOnLastSecond, dt, secondsSinceStartup, frameCount);

	app->win->SetTitle(title);
	//Use SDL_Delay to make sure you get your capped framerate

}


// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;

	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		app->fade->FadeToBlack(lastScene,(Module*)app->scene,0.f);
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		app->fade->FadeToBlack(lastScene, (Module*)app->sceneLevel2, 0.f);

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		if (item->data->active == true)
			ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void App::LoadGameRequest()
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist
	loadGameRequested = true;
}

// ---------------------------------------
void App::SaveGameRequest() const
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist and... should we overwriten
	saveGameRequested = true;
}

void App::LoadConfigRequested()
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist
	loadConfigRequested = true;
}

// ---------------------------------------
void App::SaveConfigRequested() const
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist and... should we overwriten
	saveConfigRequested = true;
}

// then call all the modules to load themselves
bool App::LoadGame(SString filename)
{
	bool ret = false;
	//  Load savegame.xml file using load_file() method from the xml_document class
	pugi::xml_parse_result result = stateFile.load_file(filename.GetString());

	// Check result for loading errors
	if (result == NULL)
	{
		LOG("Could not load map xml file savegame.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		rootStateFile = stateFile.first_child();

		ListItem<Module*>* currentModule = modules.start;
		ret = true;
		while (currentModule != NULL && ret)
		{
			// Recorremos la lista de modulos, en caso de que falle la carga del nodo perteneciente al modulo correspondiente, ret sera false 
			ret = currentModule->data->LoadState(rootStateFile.child(currentModule->data->name.GetString()));
			currentModule = currentModule->next;
		}
		stateFile.reset();
		(ret == true) ? LOG("Carga de modulos existosa") : LOG("Fallo en la carga del modulo %s", currentModule->prev->data->name.GetString());

	}
	loadGameRequested = false;
	loadConfigRequested = false;

	return ret;
}

bool App::SaveGame(SString filename) const
{
	bool ret = true;

	pugi::xml_document saveFile;
	pugi::xml_node rootSaveFile;


	//  Load savegame.xml file using load_file() method from the xml_document class
	pugi::xml_parse_result result = saveFile.load_file(filename.GetString());

	// Check result for loading errors
	if (result == NULL )
	{
		LOG("Could not load map xml file savegame.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		rootSaveFile = saveFile.first_child();
		ListItem<Module*>* currentModule = modules.start;
		ret = true;
		while (currentModule != NULL && ret)
		{
			// Recorremos la lista de modulos, en caso de que falle la carga del nodo perteneciente al modulo correspondiente, ret sera false 
			ret = currentModule->data->SaveState(rootSaveFile.child(currentModule->data->name.GetString()));
			currentModule = currentModule->next;
		}
		saveFile.save_file(filename.GetString());
		saveFile.reset();
		(ret == true) ? LOG("Guardado de modulos existoso") : LOG("Fallo en el guardado del modulo %s", currentModule->prev->data->name.GetString());

	}
	saveGameRequested = false;
	saveConfigRequested = false;

	return ret;
}



float App::GetCapMs() {
	return cappedMs;
}