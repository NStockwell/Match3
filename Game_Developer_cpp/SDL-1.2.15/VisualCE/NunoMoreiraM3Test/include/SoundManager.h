
#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include "SDL_audio.h"
#include <vector>

#define NUM_SOUNDS 2


	struct sample {
    Uint8 *data;
    Uint32 dpos;
    Uint32 dlen;
} sounds[NUM_SOUNDS];

extern void mixaudio(void *unused, Uint8 *stream, int len)
{
    int i;
    Uint32 amount;

    for ( i=0; i<NUM_SOUNDS; ++i ) {
        amount = (sounds[i].dlen-sounds[i].dpos);
        if ( amount > len ) {
            amount = len;
        }
        SDL_MixAudio(stream, &sounds[i].data[sounds[i].dpos], amount, SDL_MIX_MAXVOLUME);
        sounds[i].dpos += amount;
    }
}

class SoundManager
{

public:
	static SoundManager& getInstance()
	{
		static SoundManager instance;
		return instance;
	}
	void PlaySound(char *file);
	//std::vector<sample> sounds;

protected:
private:
	SDL_AudioSpec fmt;
	SoundManager();
	SoundManager(SoundManager const&);
	void operator=(SoundManager const&);

	void mixaudio(void *unused, Uint8 *stream, int len);

};

#endif