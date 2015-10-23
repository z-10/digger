#include "def.h"
#include "main.h"
#include "sound.h"
#include "hardware.h"
#include "record.h"
#include "digger.h"
#include <SDL.h>

bool escape=FALSE,firepflag=FALSE,aleftpressed=FALSE,arightpressed=FALSE,
     auppressed=FALSE,adownpressed=FALSE,start=FALSE,af1pressed=FALSE;
bool fire2pflag=FALSE,aleft2pressed=FALSE,aright2pressed=FALSE,
     aup2pressed=FALSE,adown2pressed=FALSE,af12pressed=FALSE;

Sint4 akeypressed;

Sint4 dynamicdir=-1,dynamicdir2=-1,staticdir=-1,staticdir2=-1,joyx=0,joyy=0;

bool joybut1=FALSE,joybut2=FALSE;

Sint4 keydir=0,keydir2=0,jleftthresh=0,jupthresh=0,jrightthresh=0,
      jdownthresh=0,joyanax=0,joyanay=0;

bool joyflag=FALSE,pausef=FALSE;

bool krdf[17]={FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,
               FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE};


bool GetAsyncKeyState(int);

int keycodes[17][5]={{SDL_SCANCODE_RIGHT,-2,-2,-2,-2},		/* 1 Right */
                     {SDL_SCANCODE_UP,-2,-2,-2,-2},		/* 1 Up */
                     {SDL_SCANCODE_LEFT,-2,-2,-2,-2},		/* 1 Left */
                     {SDL_SCANCODE_DOWN,-2,-2,-2,-2},		/* 1 Down */
                     {SDL_SCANCODE_F1,-2,-2,-2,-2},		/* 1 Fire */
                     {SDL_SCANCODE_A,-2,-2,-2,-2},		/* 2 Right */
                     {SDL_SCANCODE_W,-2,-2,-2,-2},		/* 2 Up */
                     {SDL_SCANCODE_D,-2,-2,-2,-2},		/* 2 Left */
                     {SDL_SCANCODE_S,-2,-2,-2,-2},		/* 2 Down */
                     {SDL_SCANCODE_TAB,-2,-2,-2,-2},		/* 2 Fire */
                     {SDL_SCANCODE_T,-2,-2,-2,-2},		/* Cheat */
                     {SDL_SCANCODE_KP_PLUS,-2,-2,-2,-2},	/* Accelerate */
                     {SDL_SCANCODE_KP_MINUS,-2,-2,-2,-2},	/* Brake */
                     {SDL_SCANCODE_F7,-2,-2,-2,-2},		/* Music */
                     {SDL_SCANCODE_F9,-2,-2,-2,-2},		/* Sound */
                     {SDL_SCANCODE_F10,-2,-2,-2,-2},		/* Exit */
                     {SDL_SCANCODE_SPACE,-2,-2,-2,-2}};		/* Pause */

#define ASCIIF8 SDL_SCANCODE_F8

#define rightpressed  (GetAsyncKeyState(keycodes[0][0]))
#define uppressed     (GetAsyncKeyState(keycodes[1][0]))
#define leftpressed   (GetAsyncKeyState(keycodes[2][0]))
#define downpressed   (GetAsyncKeyState(keycodes[3][0]))
#define f1pressed     (GetAsyncKeyState(keycodes[4][0]))
#define right2pressed (GetAsyncKeyState(keycodes[5][0]))
#define up2pressed    (GetAsyncKeyState(keycodes[6][0]))
#define left2pressed  (GetAsyncKeyState(keycodes[7][0]))
#define down2pressed  (GetAsyncKeyState(keycodes[8][0]))
#define f12pressed    (GetAsyncKeyState(keycodes[9][0]))


Uint4 scancode;

int pki;

void findkey(int kn) { keycodes[kn][0] = getkey(); }

void readjoy(void);

/* The standard ASCII keyboard is also checked so that very short keypresses
   are not overlooked. The functions kbhit() (returns bool denoting whether or
   not there is a key in the buffer) and getkey() (wait until a key is in the
   buffer, then return it) are used. These functions are emulated on platforms
   which only provide an inkey() function (return the key in the buffer, unless
   there is none, in which case return -1. It is done this way around for
   historical reasons, there is no fundamental reason why it shouldn't be the
   other way around. */
void checkkeyb(void)
{
  int i,j,k=0;
  bool *aflagp[10]={&arightpressed,&auppressed,&aleftpressed,&adownpressed,
                    &af1pressed,&aright2pressed,&aup2pressed,&aleft2pressed,
                    &adown2pressed,&af12pressed};
  if (leftpressed)
    aleftpressed=TRUE;
  if (rightpressed)
    arightpressed=TRUE;
  if (uppressed)
    auppressed=TRUE;
  if (downpressed)
    adownpressed=TRUE;
  if (f1pressed)
    af1pressed=TRUE;
  if (left2pressed)
    aleft2pressed=TRUE;
  if (right2pressed)
    aright2pressed=TRUE;
  if (up2pressed)
    aup2pressed=TRUE;
  if (down2pressed)
    adown2pressed=TRUE;
  if (f12pressed)
    af12pressed=TRUE;

  while (kbhit()) {
    akeypressed=getkey();
    for (i=0;i<10;i++)
      for (j=2;j<5;j++)
        if (akeypressed==keycodes[i][j])
          *aflagp[i]=TRUE;
    for (i=10;i<17;i++)
      for (j=0;j<5;j++)
        if (akeypressed==keycodes[i][j])
          k=i;
    switch (k) {
      case 10: /* Cheat! */
        if (!gauntlet) {
          playing=FALSE;
          drfvalid=FALSE;
        }
        break;
      case 11: /* Increase speed */
        if (ftime>10000l)
          ftime-=10000l;
        break;
      case 12: /* Decrease speed */
        ftime+=10000l;
        break;
      case 13: /* Toggle music */
        musicflag=!musicflag;
        break;
      case 14: /* Toggle sound */
        soundflag=!soundflag;
        break;
      case 15: /* Exit */
        escape=TRUE;
        break;
      case 16: /* Pause */
        pausef=TRUE;
    }
    if (akeypressed==ASCIIF8) /* Save DRF */
      savedrf=TRUE;
    if (akeypressed!=27 && akeypressed!='n' && akeypressed!='N')
      start=TRUE;                                /* Change number of players */
  }
}

/* Joystick not yet implemented. It will be, though, using gethrt on platform
   DOSPC. */
void readjoy(void)
{
}

void detectjoy(void)
{
  joyflag=FALSE;
  staticdir=dynamicdir=DIR_NONE;
}

/* Contrary to some beliefs, you don't need a separate OS call to flush the
   keyboard buffer. */
void flushkeybuf(void)
{
  while (kbhit())
    getkey();
  aleftpressed=arightpressed=auppressed=adownpressed=af1pressed=FALSE;
  aleft2pressed=aright2pressed=aup2pressed=adown2pressed=af12pressed=FALSE;
}

void clearfire(int n)
{
  if (n==0)
    af1pressed=FALSE;
  else
    af12pressed=FALSE;
}

bool oupressed=FALSE,odpressed=FALSE,olpressed=FALSE,orpressed=FALSE;
bool ou2pressed=FALSE,od2pressed=FALSE,ol2pressed=FALSE,or2pressed=FALSE;

void digger_readdir(int n)
{
  Sint4 j;
  bool u=FALSE,d=FALSE,l=FALSE,r=FALSE;
  bool u2=FALSE,d2=FALSE,l2=FALSE,r2=FALSE;

  if (n==0) {
    if (auppressed || uppressed) { u=TRUE; auppressed=FALSE; }
    if (adownpressed || downpressed) { d=TRUE; adownpressed=FALSE; }
    if (aleftpressed || leftpressed) { l=TRUE; aleftpressed=FALSE; }
    if (arightpressed || rightpressed) { r=TRUE; arightpressed=FALSE; }
    if (f1pressed || af1pressed) {
      firepflag=TRUE;
      af1pressed=FALSE;
    }
    else
      firepflag=FALSE;
    if (u && !oupressed)
      staticdir=dynamicdir=DIR_UP;
    if (d && !odpressed)
      staticdir=dynamicdir=DIR_DOWN;
    if (l && !olpressed)
      staticdir=dynamicdir=DIR_LEFT;
    if (r && !orpressed)
      staticdir=dynamicdir=DIR_RIGHT;
    if ((oupressed && !u && dynamicdir==DIR_UP) ||
        (odpressed && !d && dynamicdir==DIR_DOWN) ||
        (olpressed && !l && dynamicdir==DIR_LEFT) ||
        (orpressed && !r && dynamicdir==DIR_RIGHT)) {
      dynamicdir=DIR_NONE;
      if (u) dynamicdir=staticdir=2;
      if (d) dynamicdir=staticdir=6;
      if (l) dynamicdir=staticdir=4;
      if (r) dynamicdir=staticdir=0;
    }
    oupressed=u;
    odpressed=d;
    olpressed=l;
    orpressed=r;
    keydir=staticdir;
    if (dynamicdir!=DIR_NONE)
      keydir=dynamicdir;
    staticdir=DIR_NONE;
  }
  else {
    if (aup2pressed || up2pressed) { u2=TRUE; aup2pressed=FALSE; }
    if (adown2pressed || down2pressed) { d2=TRUE; adown2pressed=FALSE; }
    if (aleft2pressed || left2pressed) { l2=TRUE; aleft2pressed=FALSE; }
    if (aright2pressed || right2pressed) { r2=TRUE; aright2pressed=FALSE; }
    if (f12pressed || af12pressed) {
      fire2pflag=TRUE;
      af12pressed=FALSE;
    }
    else
      fire2pflag=FALSE;
    if (u2 && !ou2pressed)
      staticdir2=dynamicdir2=DIR_UP;
    if (d2 && !od2pressed)
      staticdir2=dynamicdir2=DIR_DOWN;
    if (l2 && !ol2pressed)
      staticdir2=dynamicdir2=DIR_LEFT;
    if (r2 && !or2pressed)
      staticdir2=dynamicdir2=DIR_RIGHT;
    if ((ou2pressed && !u2 && dynamicdir2==DIR_UP) ||
        (od2pressed && !d2 && dynamicdir2==DIR_DOWN) ||
        (ol2pressed && !l2 && dynamicdir2==DIR_LEFT) ||
        (or2pressed && !r2 && dynamicdir2==DIR_RIGHT)) {
      dynamicdir2=DIR_NONE;
      if (u2) dynamicdir2=staticdir2=2;
      if (d2) dynamicdir2=staticdir2=6;
      if (l2) dynamicdir2=staticdir2=4;
      if (r2) dynamicdir2=staticdir2=0;
    }
    ou2pressed=u2;
    od2pressed=d2;
    ol2pressed=l2;
    or2pressed=r2;
    keydir2=staticdir2;
    if (dynamicdir2!=DIR_NONE)
      keydir2=dynamicdir2;
    staticdir2=DIR_NONE;
  }

  if (joyflag) {
    incpenalty();
    incpenalty();
    joyanay=0;
    joyanax=0;
    for (j=0;j<4;j++) {
      readjoy();
      joyanax+=joyx;
      joyanay+=joyy;
    }
    joyx=joyanax>>2;
    joyy=joyanay>>2;
    if (joybut1)
      firepflag=TRUE;
    else
      firepflag=FALSE;
  }
}

/* Calibrate joystick while waiting at title screen. This works more
   effectively if the user waggles the joystick in the title screen. */
bool teststart(void)
{
  Sint4 j;
  bool startf=FALSE;
  if (joyflag) {
    readjoy();
    if (joybut1)
      startf=TRUE;
  }
  if (start) {
    start=FALSE;
    startf=TRUE;
    joyflag=FALSE;
  }
  if (!startf)
    return FALSE;
  if (joyflag) {
    joyanay=0;
    joyanax=0;
    for (j=0;j<50;j++) {
      readjoy();
      joyanax+=joyx;
      joyanay+=joyy;
    }
    joyx=joyanax/50;
    joyy=joyanay/50;
    jleftthresh=joyx-35;
    if (jleftthresh<0)
      jleftthresh=0;
    jleftthresh+=10;
    jupthresh=joyy-35;
    if (jupthresh<0)
      jupthresh=0;
    jupthresh+=10;
    jrightthresh=joyx+35;
    if (jrightthresh>255)
      jrightthresh=255;
    jrightthresh-=10;
    jdownthresh=joyy+35;
    if (jdownthresh>255)
      jdownthresh=255;
    jdownthresh-=10;
    joyanax=joyx;
    joyanay=joyy;
  }
  return TRUE;
}

/* Why the joystick reading is split between readdir and getdir like this is a
   mystery to me. */
Sint4 getdir(int n)
{
  Sint4 dir=((n==0) ? keydir : keydir2);
  if (joyflag) {
    dir=DIR_NONE;
    if (joyx<jleftthresh)
      dir=DIR_LEFT;
    if (joyx>jrightthresh)
      dir=DIR_RIGHT;
    if (joyx>=jleftthresh && joyx<=jrightthresh) {
      if (joyy<jupthresh)
        dir=DIR_UP;
      if (joyy>jdownthresh)
        dir=DIR_DOWN;
    }
  }
  if (n==0) {
    if (playing)
      playgetdir(&dir,&firepflag);
    recputdir(dir,firepflag);
  }
  else {
    if (playing)
      playgetdir(&dir,&fire2pflag);
    recputdir(dir,fire2pflag);
  }
  return dir;
}
