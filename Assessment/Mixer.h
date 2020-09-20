#pragma once
#include <string>
#include <SDL_mixer.h>
#include <vector>

static class Mixer
{
	public:
		static bool init();
		static void addSFX(std::string path);
		static void addMusic(std::string path);
		static void playSFX(int id);
		static void playMusic();
		static void onClose();
		static void mute();
		static void volumeUp();
		static void volumeDown();

	private:
		Mixer(); // Reject creating an instance
	
		static Mix_Music* backgroundMusic;
		static std::vector<Mix_Chunk*> SFX;
		static int volume;
		static bool muted;
};
