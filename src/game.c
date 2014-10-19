#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "SDL.h"
#include "SDL_image.h"
#include "graphics.h"
#include "entity.h"
#include "wasteland.h"


extern SDL_Surface *screen;
extern SDL_Surface *buffer;
extern SDL_Surface background;

extern SDL_Rect Camera;
extern Level level;
extern SDL_Surface *clipmask;
int drawents = 1, drawboxes =0;
int windowed = 0;

SDL_Joystick *joy;
Uint8 *keys;

SDL_Joystick *joy;
Uint8 *keys;

void Init_All();
int main(int argc, char *argv[])
{
	SPRITE *window;
	int done;
	int keyn;
	Entity *player;
	Uint8 *keys;
	Level *testlevel;
	Init_All();
	/*testlevel = getCurrentLevel(0);*/
	/*RenderLevel(testlevel);*/
	LoadSprite("images/effects.png",16,16,-1,-1,-1);
	window = DisplayBar();
	player = MakePlayer();
	done = 0;
	do
	{
		UpdateEntities();
		ResetBuffer();
		DrawEntities();
		UpdateDisplayBar(player,window);
		NextFrame();
		SDL_PumpEvents();
		keys = SDL_GetKeyState(&keyn);
		if(keys[SDLK_ESCAPE])done = 1;
	}while(!done);

	exit(0);
	return 0;




}



void CleanUpAll()
{
	CloseSprites();
	ClearEntities();

}

void Init_All()
{
	Init_Graphics();
	InitSpriteList();
	InitEntityList();
	InitMouse();
	/*LoadLevelSprites();*/
	atexit(CleanUpAll);
}