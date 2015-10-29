#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

#include "def.h"
#include "device.h"
#include "hardware.h"

void fill_audio(void *udata, Uint8 *stream, int len);
samp getsample(void);

samp *buf;
Uint4 bsize;

bool wave_device_available = false;

bool initsounddevice(void)
{
	return(true);
}

bool setsounddevice(int base, int irq, int dma, Uint4 samprate, Uint4 bufsize)
{
	SDL_AudioSpec wanted;
	bool result = false;

	wanted.freq = samprate;
	wanted.samples = bufsize;
	wanted.channels = 1;
	wanted.format = AUDIO_U8;
	wanted.userdata = NULL;
	wanted.callback = fill_audio;

	if ((SDL_Init(SDL_INIT_AUDIO)) >= 0)
		if ((SDL_OpenAudio(&wanted, NULL)) >= 0)
			result = true;
	if (result == false)
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
	else {
		buf = new samp[bufsize];
		bsize = bufsize;
		wave_device_available = true;
	}

	return(result);
}

void fill_audio(void *udata, Uint8 *stream, int len)
{
	int i;

	if (len > bsize)
		len = bsize;
	for(i = 0; i<len; i++)
		buf[i] = getsample();

	SDL_MixAudio(stream, buf, len, SDL_MIX_MAXVOLUME);
}


void killsounddevice(void)
{
	SDL_PauseAudio(1);
}
