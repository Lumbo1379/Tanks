#include "Mixer.h"
#include <SDL.h>

Mix_Music* Mixer::backgroundMusic;
std::vector<Mix_Chunk*> Mixer::SFX;
int Mixer::volume;
bool Mixer::muted;

bool Mixer::init()
{
	volume = 128;
	muted = false;
	
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		printf("Warning: Audio has not been found!\n");
		return false;
	}

	return true;
}

void Mixer::addSFX(std::string path)
{
	SDL_Log("Sound effect added from %s", path.c_str());
	
	SFX.emplace_back(Mix_LoadWAV(path.c_str()));
}

void Mixer::addMusic(std::string path)
{
	SDL_Log("Background music added from %s", path.c_str());
	
	backgroundMusic = Mix_LoadMUS(path.c_str());
}

void Mixer::playSFX(int id)
{
	Mix_PlayChannel(-1, SFX[id], 0);
}

void Mixer::playMusic()
{
	if (Mix_PlayingMusic() == 0 && !muted)
		Mix_PlayMusic(backgroundMusic, -1);
}

void Mixer::onClose()
{
	Mix_FreeMusic(backgroundMusic);

	for (auto& sfx : SFX)
		Mix_FreeChunk(sfx);

	Mix_CloseAudio();
}

void Mixer::mute()
{
	if (muted)
	{
		SDL_Log("Sound un-muted");
		
		muted = false;
		Mix_Volume(-1, volume);
		Mix_VolumeMusic(volume);
	}
	else
	{
		SDL_Log("Sound muted");
		
		muted = true;
		Mix_Volume(-1, 0);
		Mix_VolumeMusic(0);
	}
}

void Mixer::volumeUp()
{
	if (volume < 128)
	{
		volume++;
		Mix_Volume(-1, volume);
		Mix_VolumeMusic(volume);

		SDL_Log("Volume increased to %i", volume);
	}
	else
		SDL_Log("Volume at max");
}

void Mixer::volumeDown()
{
	if (volume > 0)
	{
		volume--;
		Mix_Volume(-1, volume);
		Mix_VolumeMusic(volume);

		SDL_Log("Volume decreased to %i", volume);
	}
	else
		SDL_Log("Volume at minimum");
}

Mixer::Mixer() = default;

