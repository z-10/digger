#ifndef DEF_H
#define DEF_H

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

#define TRUE -1
#define FALSE 0
typedef int bool;

typedef signed char Sint3;
typedef unsigned char Uint3;
typedef signed short int Sint4;
typedef unsigned short int Uint4;
typedef signed int Sint;
typedef unsigned int Uint;
typedef signed long int Sint5;
typedef unsigned long int Uint5;

#define DIR_NONE -1
#define DIR_RIGHT 0
#define DIR_UP 2
#define DIR_LEFT 4
#define DIR_DOWN 6

#define TYPES 5

#define BONUSES 1
#define BAGS 7
#define MONSTERS 6
#define FIREBALLS DIGGERS
#define DIGGERS 2
#define SPRITES (BONUSES+BAGS+MONSTERS+FIREBALLS+DIGGERS)

/* Sprite order is figured out here. By LAST I mean last+1. */

#define FIRSTBONUS 0
#define LASTBONUS (FIRSTBONUS+BONUSES)
#define FIRSTBAG LASTBONUS
#define LASTBAG (FIRSTBAG+BAGS)
#define FIRSTMONSTER LASTBAG
#define LASTMONSTER (FIRSTMONSTER+MONSTERS)
#define FIRSTFIREBALL LASTMONSTER
#define LASTFIREBALL (FIRSTFIREBALL+FIREBALLS)
#define FIRSTDIGGER LASTFIREBALL
#define LASTDIGGER (FIRSTDIGGER+DIGGERS)

#define MWIDTH 15
#define MHEIGHT 10
#define MSIZE MWIDTH*MHEIGHT

#define MAX_REC_BUFFER 262144l
           /* I reckon this is enough for about 36 hours of continuous play. */

#define INI_GAME_SETTINGS "Game"
#define INI_GRAPHICS_SETTINGS "Graphics"
#define INI_SOUND_SETTINGS "Sound"
#define INI_KEY_SETTINGS "Keys"

#define DEFAULT_BUFFER 128
#define DEF_SND_DEV 0
#define ININAME "DIGGER.INI"

#define DIGGER_VERSION "SDL2 0.0.1 RS"

#include "fbsd_sup.h"

#endif /* DEF_H */
