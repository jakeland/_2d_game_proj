#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "SDL.h"
#include "SDL_image.h"
#include "graphics.h"
#include "entity.h"
#include"wasteland.h"


extern SDL_Surface *screen;
extern SDL_Surface *buffer;

extern SDL_Surface *level;

extern SDL_Rect Camera;
extern SDL_Rect level1_floor;
extern SDL_Surface *clipmask;


int drawents = 1, drawboxes =0;



SDL_Joystick *joy;
Uint8 *keys;

void Init_All();
int main(int argc, char *argv[])
{
	SDL_Surface *temp;
	SDL_Surface *buffer;
	SPRITE *tile;
	int done;
	int keyn;
	Entity *player;
	Entity *oddish;
	Entity *ground;
	Uint8 *keys;
	
	Init_All();

	SetFloor();
	
	/*testlevel = getCurrentLevel(0);*/
	
	RenderLevel();
	

	LoadSprite("images/effects.png",16,16,-1,-1,-1);
	/*window = DisplayBar();*/
	ground = MakePlatform(0,2014, 2048, 20);
	player = MakePlayer();
	oddish = MakePok();
	done = 0;
	do
	{
		
		UpdateEntities();
		
		ResetBuffer();
		
		SetCamera();
		DrawEntities();
		SDL_FillRect( level, &player->bbox, SDL_MapRGB( screen->format, 0x77, 0x77, 0x77 ) );
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
	
	LoadLevelSprite();
	atexit(CleanUpAll);
}