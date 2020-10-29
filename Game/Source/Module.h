#ifndef __MODULE_H__
#define __MODULE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"
#include "Log.h"

class App;

class Module
{
public:

	Module() : active(false)
	{}

	void Init()
	{
		active = true;
	}

	// Called before render is available
	// L01: DONE 5: Sending config file to all modules
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}


	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

    // L02: TODO 2: Create new virtual methods to Load / Save
	virtual bool LoadModule(pugi::xml_node&) 
	{
		LOG("Load Clase base");
		return true;
	}
	virtual bool SaveModule(pugi::xml_node&)
	{
		LOG("save Clase base");
		return true;
	}


public:

	SString name;
	bool active;

};

#endif // __MODULE_H__