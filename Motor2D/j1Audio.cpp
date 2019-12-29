#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "j1Audio.h"
#include "j1FadeToBlack.h"
#include "SDL/include/SDL.h"
#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

j1Audio::j1Audio() : j1Module()
{
	main_menu_music = NULL;
	game_music = NULL;
	name.create("audio");
}

// Destructor
j1Audio::~j1Audio()
{}


// Called before render is available
bool j1Audio::Awake(const pugi::xml_node& node)
{
	if (node != nullptr) {
		volume_music = node.child("music").attribute("music_volume").as_int();
		volume_fx = node.child("music").attribute("fx_volume").as_int();
	}
	else {
		LOG("Error in Awake %s", name);
	}
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}
	if (ret == true) {
		Mix_VolumeMusic(volume_music);
		Mix_Volume(1, volume_fx);
	}
	return ret;
}

bool j1Audio::Start()
{
	main_menu_music = Mix_LoadMUS("audio/music/music2.ogg");
	game_music = Mix_LoadMUS("audio/music/music.ogg");

	return true;
}

bool j1Audio::Update(float dt)
{

	return true;
}

// Called before quitting
bool j1Audio::CleanUp()
{
	if (!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if (main_menu_music != NULL || game_music != NULL)
	{
		Mix_FreeMusic(main_menu_music);
		Mix_FreeMusic(game_music);
	}

	p2List_item<Mix_Chunk*>* item;
	for (item = fx.start; item != NULL; item = item->next)
		Mix_FreeChunk(item->data);

	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

bool j1Audio::PlayMusic(MusicType type, float fade_time)
{
	if (type == MAIN_MENU_MUSIC)
	{
		PlayMusic(main_menu_music, fade_time);
	}
	else
	{
		PlayMusic(game_music, fade_time);
	}
	return true;
}

// Play a music file
bool j1Audio::PlayMusic(_Mix_Music* music, float fade_time)
{
	bool ret = true;
	if (!active)
		return false;

	if (current_music != NULL)
	{
		if (fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		//Mix_FreeMusic(game_music);
	}

	current_music = music;

	if (music == NULL)
	{
		ret = false;
	}
	else
	{
		if (fade_time > 0.0f)
		{
			if (Mix_FadeInMusic(music, -1, (int)(fade_time * 1000.0f)) < 0)
			{
				ret = false;
			}
		}
		else
		{
			if (Mix_PlayMusic(music, -1) < 0)
			{
				ret = false;
			}
		}
	}


	//LOG("Successfully playing %s", path);
	return ret;
}

// Load WAV
unsigned int j1Audio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if (!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if (chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.add(chunk);
		ret = fx.count();
	}

	return ret;
}

void j1Audio::SetVolume(int volume)
{
	LOG("%i", volume_music);

	if (volume_music > 128 || volume_music < 0) {
		return;
	}
	volume_music = volume;
	Mix_VolumeMusic(volume_music);

}

int j1Audio::GetVolume() const
{
	return volume_music;
}

void j1Audio::SetVolumeFX(int volume)
{
	if (volume_fx > 128 || volume_fx < 0) {
		return;
	}
	volume_fx = volume;
	Mix_Volume(1, volume_fx);
}

int j1Audio::GetVolumeFX() const
{
	return volume_fx;
}

// Play WAV
bool j1Audio::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;

	if (!active)
		return false;

	if (id > 0 && id <= fx.count())
	{
		Mix_PlayChannel(1, fx[id - 1], repeat);
	}

	return ret;
}

bool j1Audio::Load(const pugi::xml_node& node)
{
	volume_music = node.child("audio").attribute("volume_music").as_int();
	return true;
}
bool j1Audio::Save(pugi::xml_node& node)
{
	pugi::xml_node audio_node = node.append_child("audio");
	audio_node.append_attribute("volume_music");
	audio_node.attribute("volume_music").set_value(volume_music);

	return true;
}