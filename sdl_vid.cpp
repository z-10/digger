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

/* malloc() and friends */
#include <stdlib.h>
/* Lovely SDL */
#include <SDL.h>

#include "def.h"
#include "hardware.h"
#include "title_gz.h"
#include "icon.h"
extern Uint3 *vgatable[];
extern Uint3 *ascii2vga[];

Uint3 **sprites = vgatable;
Uint3 **alphas = ascii2vga;

Sint4 xratio = 2;
Sint4 yratio = 2;
Sint4 yoffset = 0;
Sint4 hratio = 2;
Sint4 wratio = 2;
#define virt2scrx(x) (x*xratio)
#define virt2scry(y) (y*yratio+yoffset)
#define virt2scrw(w) (w*wratio)
#define virt2scrh(h) (h*hratio)

/* palette1, normal intensity */
SDL_Color vga16_pal1[] = \
{{0,0,0,255},{0,0,128,255},{0,128,0,255},{0,128,128,255},{128,0,0,255},{128,0,128,255} \
,{128,64,0,255},{128,128,128,255},{64,64,64,255},{0,0,255,255},{0,255,0,255} \
,{0,255,255,255},{255,0,0,255},{255,0,255,255},{255,255,0,255},{255,255,255,255}};
/* palette1, high intensity */
SDL_Color vga16_pal1i[] = \
{{0,0,0,255},{0,0,255,255},{0,255,0,255},{0,255,255,255},{255,0,0,255},{255,0,255,255} \
,{255,128,0,255},{196,196,196,255},{128,128,128,255},{128,128,255,255},{128,255,128,255} \
,{128,255,255,255},{255,128,128,255},{255,128,255,255},{255,255,128,255},{255,255,255,255}};
/* palette2, normal intensity */
SDL_Color vga16_pal2[] = \
{{0,0,0,255},{0,128,0,255},{128,0,0,255},{128,64,0,255},{0,0,128,255},{0,128,128,255} \
,{128,0,128,255},{128,128,128,255},{64,64,64,255},{0,255,0,255},{255,0,0,255} \
,{255,255,0,255},{0,0,255,255},{0,255,255,255},{255,0,255,255},{255,255,255,255}};
/* palette2, high intensity */
SDL_Color vga16_pal2i[] = \
{{0,0,0,255},{0,255,0,255},{255,0,0,255},{255,128,0,255},{0,0,255,255},{0,255,255,255} \
,{255,0,255,255},{196,196,196,255},{128,128,128,255},{128,255,128,255},{255,128,128,255} \
,{255,255,128,255},{128,128,255,255},{128,255,255,255},{255,128,255,255},{255,255,255,255}};

SDL_Color *npalettes[] = {vga16_pal1, vga16_pal2};
SDL_Color *ipalettes[] = {vga16_pal1i, vga16_pal2i};
Sint4	currpal=0;

Uint32	addflag=0;

SDL_Surface *screen, *helper = NULL;
SDL_Window *sdlWindow = NULL;
SDL_Renderer *sdlRenderer = NULL;
SDL_Texture *sdlTexture = NULL;

/* Data structure holding pending updates */
struct PendNode {
	PendNode *prevnode;
	PendNode *nextnode;
	SDL_Rect rect;
};

struct PendNode *First=NULL, *Last=NULL;

int pendnum = 0;

SDL_Surface *ch2bmap(Uint3 *sprite, Sint4 w, Sint4 h)
{
	Sint4 realw, realh;
	SDL_Surface *tmp;

	realw = virt2scrw(w*4);
	realh = virt2scrh(h);
	tmp = SDL_CreateRGBSurfaceFrom(sprite, realw, realh, 8, realw, 0, 0, 0, 0);
	SDL_SetPaletteColors(tmp->format->palette, screen->format->palette->colors, 0, screen->format->palette->ncolors);
	return(tmp);
}

void graphicsoff(void)
{
}

bool setmode(void)
{
	if (SDL_SetWindowFullscreen(sdlWindow, addflag) < 0)
		return(false);
	else
	{
		if(addflag == 0)
		{
			SDL_SetWindowSize(sdlWindow, 800, 600);
		}
		return(true);
	}
}

void switchmode(void)
{
	Uint32 saved;
	SDL_Surface *tmp = NULL;
	SDL_Surface *oldscreen;

	vgageti(0, 0, (Uint3 *)&tmp, 80, 200);
	oldscreen = screen;
	saved = addflag;

	if(addflag == 0)
		addflag = SDL_WINDOW_FULLSCREEN_DESKTOP;
	else
		addflag = 0;
	if(setmode() == false) {
		addflag = saved;
		if(setmode() == false) {
			fprintf(stderr, "Fatal: failed to change videomode and"\
				"fallback mode failed as well. Exitting.\n");
			exit(1);
		}
	}

	SDL_SetPaletteColors(screen->format->palette, tmp->format->palette->colors, 0, \
		tmp->format->palette->ncolors);
	vgaputi(0, 0, (Uint3 *)&tmp, 80, 200);
	SDL_FreeSurface(tmp);
	SDL_FreeSurface(oldscreen);
}


void vgainit(void)
{

	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_Surface *tmp = NULL;

	tmp = SDL_CreateRGBSurfaceFrom(Icon, 64, 64, 8, 64, 0, 0, 0, 0);
	SDL_SetColorKey(tmp, SDL_TRUE, 247);
	SDL_SetPaletteColors(tmp->format->palette, IconPalette, 0, 256);

	sdlWindow = SDL_CreateWindow("D I G G E R",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             0, 0,
                             SDL_WINDOW_OPENGL);
  if(sdlWindow == NULL)
	{
		fprintf(stderr, "Couldn't initialize SDL window: %s\n", SDL_GetError());
		exit(1);
	}
  SDL_SetWindowIcon(sdlWindow, tmp);

	sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
	if(sdlRenderer == NULL)
	{
		fprintf(stderr, "Couldn't initialize SDL renderer: %s\n", SDL_GetError());
		exit(1);
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
	SDL_RenderSetLogicalSize(sdlRenderer, 640, 480);

	sdlTexture = SDL_CreateTexture(sdlRenderer,
                               SDL_PIXELFORMAT_ARGB8888,
                               SDL_TEXTUREACCESS_STREAMING,
                               640, 480);
	if(sdlTexture == NULL)
	{
		fprintf(stderr, "Couldn't initialize SDL texture: %s\n", SDL_GetError());
		exit(1);
	}
	screen = SDL_CreateRGBSurface(0, 640, 480, 8, 0, 0, 0, 0);
	helper = SDL_CreateRGBSurface(0, 640, 480, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	if(screen == NULL || helper == NULL)
	{
		fprintf(stderr, "Couldn't initialize SDL surface: %s\n", SDL_GetError());
		exit(1);
	}

	if(setmode() == false) {
		fprintf(stderr, "Couldn't set 640x480x8 video mode: %s\n", SDL_GetError());
		exit(1);
  }
	SDL_ShowCursor(0);
}

void vgaclear(void)
{
	SDL_Surface *tmp = NULL;

	vgageti(0, 0, (Uint3 *)&tmp, 80, 200);
	memset(tmp->pixels, 0x00, tmp->w*tmp->h);
	vgaputi(0, 0, (Uint3 *)&tmp, 80, 200);
	SDL_FreeSurface(tmp);
}
void setpal(SDL_Color *pal)
{
	SDL_SetPaletteColors(screen->format->palette, pal, 0, 16);
}

void vgainten(Sint4 inten)
{
	if(inten == 1)
		setpal(ipalettes[currpal]);
	else
		setpal(npalettes[currpal]);
}

void vgapal(Sint4 pal)
{
	setpal(npalettes[pal]);
	currpal = pal;
}

void doscreenupdate(void)
{
	struct PendNode *p;

	for(p=First;p!=NULL;)
	{
		First = p->nextnode;
		free(p);
		p = First;
	}
	pendnum = 0;
	SDL_BlitSurface(screen, NULL, helper, NULL);
	SDL_UpdateTexture(sdlTexture, NULL, helper->pixels, helper->pitch);
	SDL_RenderClear(sdlRenderer);
	SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
	SDL_RenderPresent(sdlRenderer);
}

void vgaputi(Sint4 x, Sint4 y, Uint3 *p, Sint4 w, Sint4 h)
{
	SDL_Surface *tmp;
	SDL_Palette *reserv;
	struct PendNode *newPtr, *ptr;

	newPtr = new PendNode();
	memset(newPtr, 0x00, (sizeof (struct PendNode)));
	newPtr->rect.x = virt2scrx(x);
	newPtr->rect.y = virt2scry(y);
	newPtr->rect.w = virt2scrw(w*4);
	newPtr->rect.h = virt2scrh(h);

	memcpy(&tmp, p, (sizeof (SDL_Surface *)));
	reserv = tmp->format->palette;
	tmp->format->palette = screen->format->palette;
	SDL_BlitSurface(tmp, NULL, screen, &newPtr->rect);
	tmp->format->palette = reserv;
/*
 * Following piece of code comparing already pending updates with current with
 * main goal to prevent redrawing overlapping rectangles several times.
 */

	for(ptr=First;ptr!=NULL;ptr=ptr->nextnode) {
		if((newPtr->rect.x >= ptr->rect.x) &&
		   (newPtr->rect.y >= ptr->rect.y) &&
		   ((newPtr->rect.x+newPtr->rect.w) <= (ptr->rect.x+ptr->rect.w)) &&
		   ((newPtr->rect.y+newPtr->rect.h) <= (ptr->rect.y+ptr->rect.h))) {
			delete newPtr;
			return;
		} else if((newPtr->rect.x <= ptr->rect.x) &&
		   (newPtr->rect.y <= ptr->rect.y) &&
		   ((newPtr->rect.x+newPtr->rect.w) >= (ptr->rect.x+ptr->rect.w)) &&
		   ((newPtr->rect.y+newPtr->rect.h) >= (ptr->rect.y+ptr->rect.h))) {
			ptr->rect.x = newPtr->rect.x;
			ptr->rect.y = newPtr->rect.y;
			ptr->rect.w = newPtr->rect.w;
			ptr->rect.h = newPtr->rect.h;
			free(newPtr);
			return;
		}
	}

	if (pendnum == 0)
		First = newPtr;
	else {
		Last->nextnode = newPtr;
		newPtr->prevnode = Last;
	}

	Last = newPtr;
	pendnum++;
}

void vgageti(Sint4 x, Sint4 y, Uint3 *p, Sint4 w, Sint4 h)
{
	SDL_Surface *tmp;
	SDL_Rect src;

	memcpy(&tmp, p, (sizeof (SDL_Surface *)));
	if (tmp != NULL)
		SDL_FreeSurface(tmp); /* Destroy previously allocated bitmap */

	src.x = virt2scrx(x);
	src.y = virt2scry(y);
	src.w = virt2scrw(w*4);
	src.h = virt2scrh(h);

	tmp = SDL_CreateRGBSurface(SDL_SWSURFACE, src.w, src.h, 8, 0, 0, 0, 0);
	SDL_SetPaletteColors(tmp->format->palette, screen->format->palette->colors, 0, screen->format->palette->ncolors);
	SDL_BlitSurface(screen, &src, tmp, NULL);
	memcpy(p, &tmp, (sizeof (SDL_Surface *)));
}

Sint4 vgagetpix(Sint4 x, Sint4 y)
{
	SDL_Surface *tmp = NULL;
	Uint4 xi,yi;
	Uint4 i = 0;
	Sint4 rval = 0;
	Uint8 *pixels;

	if ((x > 319) || (y > 199))
	       return (0xff);

	vgageti(x, y, (Uint3 *)&tmp, 1, 1);
	pixels = (Uint8 *)tmp->pixels;
	for (yi=0;yi<tmp->h;yi++)
		for (xi=0;xi<tmp->w;xi++)
			if (pixels[i++])
				rval |= 0x80 >> xi;

	SDL_FreeSurface(tmp);

	return(rval & 0xee);
}

void vgaputim(Sint4 x, Sint4 y, Sint4 ch, Sint4 w, Sint4 h)
{
	SDL_Surface *tmp;
	SDL_Surface *mask;
	SDL_Surface *scr = NULL;
	Uint8   *tmp_pxl, *mask_pxl, *scr_pxl;
	Sint4 realsize;
	Sint4 i;

	tmp = ch2bmap(sprites[ch*2], w, h);
	mask = ch2bmap(sprites[ch*2+1], w, h);
	vgageti(x, y, (Uint3 *)&scr, w, h);
	realsize = scr->w * scr->h;
	tmp_pxl = (Uint8 *)tmp->pixels;
	mask_pxl = (Uint8 *)mask->pixels;
	scr_pxl = (Uint8 *)scr->pixels;
	for(i=0;i<realsize;i++)
		if(tmp_pxl[i] != 0xff)
			scr_pxl[i] = (scr_pxl[i] & mask_pxl[i]) | \
				tmp_pxl[i];

	vgaputi(x, y, (Uint3 *)&scr, w, h);
	tmp->pixels = NULL;   /* We should NULL'ify these ppointers, or the VGLBitmapDestroy */
	mask->pixels = NULL;  /* will shoot itself in the foot by trying to dellocate statically */
	SDL_FreeSurface(tmp);/* allocated arrays */
	SDL_FreeSurface(mask);
	SDL_FreeSurface(scr);
}

void vgawrite(Sint4 x, Sint4 y, Sint4 ch, Sint4 c)
{
	SDL_Surface *tmp;
	Uint8 *copy;
	Uint8 color;
	Sint4 w=3, h=12, size;
	Sint4 i;

	if(((ch - 32) >= 0x5f) || (ch < 32))
		return;
	tmp = ch2bmap(alphas[ch-32], w, h);
	size = tmp->w*tmp->h;
	copy = new Uint8[size];
	memcpy(copy, tmp->pixels, size);

	for(i = size;i!=0;) {
		i--;
		color = copy[i];
		if (color==10) {
			if (c==2)
				color=12;
            		else {
              			if (c==3)
				color=14;
			}
          	} else
			if (color==12) {
				if (c==1)
					color=2;
				else
					if (c==2)
						color=4;
					else
						if (c==3)
							color=6;
			}
		copy[i] = color;
	}
	tmp->pixels = copy;
	vgaputi(x, y, (Uint3 *)&tmp, w, h);
	SDL_FreeSurface(tmp);
}

void vgatitle(void)
{
	SDL_Surface *tmp=NULL;

	vgageti(0, 0, (Uint3 *)&tmp, 80, 200);
	gettitle((unsigned char*)tmp->pixels);
	vgaputi(0, 0, (Uint3 *)&tmp, 80, 200);
	SDL_FreeSurface(tmp);
}

void gretrace(void)
{
}

void savescreen(void)
{
/*	FILE *f;
	int i;

	f=fopen("screen.saw", "w");

	for(i=0;i<(VGLDisplay->Xsize*VGLDisplay->Ysize);i++)
		fputc(VGLDisplay->Bitmap[i], f);
	fclose(f);*/
}
