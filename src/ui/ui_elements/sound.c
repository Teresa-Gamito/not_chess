#include "include/ui/ui_elements/sound.h"

struct Sound
{
    Uint8 *wav_data;
    Uint32 wav_data_lenght;
    SDL_AudioStream *stream;
};

Sound* sound_load(const char* path)
{
    Sound* sound = SDL_malloc(sizeof(Sound));
    verify(sound == NULL, "Sound could not be created: malloc");

    SDL_AudioSpec spec;
    SDL_LoadWAV(path, &spec, &sound->wav_data, &sound->wav_data_lenght);
    sound->stream = SDL_CreateAudioStream(&spec, NULL);
    SDL_BindAudioStream(g_audio_device, sound->stream);

    return sound;
}

void sound_unload(Sound* sound)
{
    SDL_free(sound);
}

void sound_play(const Sound* sound)
{
    if (!g_volume_on)
    {
        return;
    }
    SDL_ClearAudioStream(sound->stream);

    SDL_PutAudioStreamData(
        sound->stream, 
        sound->wav_data, 
        (int) sound->wav_data_lenght
    );
}


