/*
 * ---------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42, (c) Poul-Henning Kamp): Maxim
 * Sobolev <sobomax@altavista.net> wrote this file. As long as you retain
 * this  notice you can  do whatever you  want with this stuff. If we meet
 * some day, and you think this stuff is worth it, you can buy me a beer in
 * return.
 *
 * Maxim Sobolev
 * ---------------------------------------------------------------------------
 */

#include <stdlib.h>
#include <string.h>
#include <SDL.h>

#include "def.h"
#include "hardware.h"
#include "sdl_vid.h"

#define KBLEN		30
Sint4 kbuffer[KBLEN];
Sint4 klen=0;

int Handler(void* userdata, SDL_Event *event)
{
	if(event->type == SDL_KEYDOWN) {
		if(klen == KBLEN) /* Buffer is full, drop some pieces */
			memcpy(kbuffer, kbuffer + 1, --klen);
		kbuffer[klen++] = event->key.keysym.sym;
}
if(event->type == SDL_QUIT)
{
	exit(0);
}

	return(1);
}

bool GetAsyncKeyState(int key)
{
	const Uint8 *keys;

	SDL_PumpEvents();
	keys = SDL_GetKeyboardState(NULL);
	if (keys[key])
		return(true);
	else
		return(false);
}

void initkeyb(void)
{
	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
	SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
	SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);

	SDL_SetEventFilter(Handler, NULL);
}

void restorekeyb(void)
{
}

Sint4 getkey(void)
{
	Sint4 result;

	while(kbhit() != true)
		gethrt();
	result = kbuffer[0];
	memcpy(kbuffer, kbuffer + 1, --klen);

	return(result);
}

bool kbhit(void)
{
	SDL_PumpEvents();
	doscreenupdate();

	if (klen > 0)
		return(true);
	else
		return(false);

}
