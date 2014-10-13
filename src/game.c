#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "graphics.h"
#include "entity.h"
extern SDL_Surface *screen;
extern SDL_Surface *buffer; /*pointer to the draw buffer*/
extern SDL_Rect Camera;

void Init_All();


/*this program must be run from the directory directly below images and src, not from within src*/
/*notice the default arguments for main.  SDL expects main to look like that, so don't change it*/
int main(int argc, char *argv[])
{
  SPRITE *window;

  SDL_Surface *temp;
  SDL_Surface *bg;
  SPRITE *tile;

  int done;
  int keyn;
  Uint8 *keys;
  Entity *player;
  int i;
  int mx,my;
  
  Init_All();
  temp = IMG_Load("images/bgtest.png");/*notice that the path is part of the filename*/
  if(temp != NULL)						/*ALWAYS check your pointers before you use them*/
    bg = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);
  if(bg != NULL)
    SDL_BlitSurface(bg,NULL,buffer,NULL);
  tile = LoadSprite("images/32_32_16_2sprite.png",32,32, -1,-1,-1);
  if(tile != NULL)for(i = 0;i < 12;i++)
  {
    DrawSprite(tile,buffer,i * tile->w,0,0);
  }
  player = MakePlayer();
  done = 0;
  do
  {
    ResetBuffer ();
    DrawMouse();
	DrawEntities();
    NextFrame();
    SDL_PumpEvents();
    keys = SDL_GetKeyState(&keyn);
    if(SDL_GetMouseState(&mx,&my))
    {
      DrawSprite(tile,buffer,(mx /32) * 32,(my /32) * 32,0); 
    }
    if(keys[SDLK_ESCAPE])done = 1;
  }while(!done);
  exit(0);		/*technically this will end the program, but the compiler likes all functions that can return a value TO return a value*/
  return 0;
}

void CleanUpAll()
{
  CloseSprites();
  ClearEntities();
  /*any other cleanup functions can be added here*/ 
}

void Init_All()
{
  Init_Graphics();

  InitMouse();
  atexit(CleanUpAll);
}

