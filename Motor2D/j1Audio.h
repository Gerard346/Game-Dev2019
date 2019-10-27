#ifndef __j1AUDIO_H__
#define __j1AUDIO_H__

#include "j1Module.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class j1Audio : public j1Module
{
public:

	j1Audio();

	// Destructor
	virtual ~j1Audio();

	// Called before render is available
	bool Awake(const pugi::xml_node& node) override;

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);
	void SetVolume(int volume);
	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	bool Load(const pugi::xml_node&);

	bool Save(pugi::xml_node&);

private:
	int volume_music = 0;
	int volume_fx= 0;
	_Mix_Music*			music;
	p2List<Mix_Chunk*>	fx;
};

#endif // __j1AUDIO_H__