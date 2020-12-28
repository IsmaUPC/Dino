#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Module.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class Audio : public Module
{
public:

	Audio();

	// Destructor
	virtual ~Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	bool Update(float dt);

	// Play a music file
	bool PlayMusic(const char* path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	bool UnloadFxs();

	// Volume modify
	void ChangeVolumeMusic(int num);

	// Save and Load volume
	bool LoadState(pugi::xml_node& node);
	bool SaveState(pugi::xml_node& node)const;
	// Return volume music
	int GetVolumeMusic() { return volumeMusic; }
	// Return volume Fx
	int GetVolumeFx() { return volumeFx; }

	// Set volume Music
		int SetVolumeMusic(int volume) { volumeMusic=volume; }
	// Set volume Fx
	int SetVolumeFx(int volume) { volumeFx=volume; }
private:

	_Mix_Music* music;
	List<Mix_Chunk *>	fx;


	uint volumeMusic=0;
	uint volumeFx=0;
};

#endif // __AUDIO_H__