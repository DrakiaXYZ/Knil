#include "Game.Audio.h"

Mix_Music* GameAudio::music = NULL;
Mix_Chunk* GameAudio::chunk = NULL;
const int GameAudio::audio_rate;
const Uint16 GameAudio::audio_format;
const int GameAudio::audio_channels;
const int GameAudio::audio_buffers;
bool GameAudio::loaded = false;
int GameAudio::channel = -1;

bool GameAudio::Init() {
    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) == -1) {
        return false;
    }
    loaded = true;
    return true;
}

void GameAudio::PlayMusic(const char* file) {
    if (!loaded) return;
    if (music) {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        music = NULL;
    }
    music = Mix_LoadMUS(file);
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(32);
}
void GameAudio::StopMusic() {
    if (!loaded) return;
    if (music) {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        music = NULL;
    }
}
void GameAudio::PlaySound(const char* file) {
    if (!loaded) return;
    if (chunk) {
        Mix_HaltChannel(channel);
        channel = -1;
        Mix_FreeChunk(chunk);
        chunk = NULL;
    }
    chunk = Mix_LoadWAV(file);
    channel = Mix_PlayChannel(-1, chunk, 0);

}

void GameAudio::Destroy() {
    if (!loaded) return;
    if (music) {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
    }
    if (chunk) {
        Mix_HaltChannel(channel);
        Mix_FreeChunk(chunk);
        chunk = NULL;
    }
    Mix_CloseAudio();
}
