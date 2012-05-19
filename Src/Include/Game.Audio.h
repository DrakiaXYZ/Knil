#ifndef GAMEAUDIO_H
#define GAMEAUDIO_H

#include <SDL/SDL.h>
#include <SDL/SDL_Mixer.h>

class GameAudio
{
	public:
        static bool Init();
		static void Destroy();
		static void PlayMusic(const char* file);
		static void StopMusic();
		static void PlaySound(const char* file);

		static const int audio_rate = 22050;
		static const Uint16 audio_format = AUDIO_S16;
		static const int audio_channels = 2;
		static const int audio_buffers = 4096;
		static bool loaded;
		static int channel;

		static Mix_Music* music;
		static Mix_Chunk* chunk;

	private:

};


#endif // GAMEAUDIO_H

