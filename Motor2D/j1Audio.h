#ifndef __j1AUDIO_H__
#define __j1AUDIO_H__

#include "j1Module.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

enum MusicType {
	MAIN_MENU_MUSIC,
	GAME_MUSIC
};
class j1Audio : public j1Module
{
public:

	j1Audio();

	// Destructor
	virtual ~j1Audio();
	// Called before render is available
	bool Awake(const pugi::xml_node& node) override;

	bool Start();

	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(MusicType type, float fade_time = DEFAULT_MUSIC_FADE_TIME);
	bool PlayMusic(_Mix_Music* music, float fade_time = DEFAULT_MUSIC_FADE_TIME);

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
	
	_Mix_Music*			main_menu_music;
	_Mix_Music*			game_music;
	_Mix_Music*			current_music;

	p2List<Mix_Chunk*>	fx;

	char* pending_play_path = nullptr;
	float pending_play_fade = 0.0f;
	float pending_play_delay = 0.0f;
};

#endif // __j1AUDIO_H__