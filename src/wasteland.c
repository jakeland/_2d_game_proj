#include <string.h>
#include <stdlib.h>
#include "wasteland.h"
#include "entity.h"
#include "graphics.h"


extern SDL_Surface *buffer;
extern SDL_Rect Camera;
extern SDL_Rect offset;
extern SDL_Rect vcam;

extern Entity *dude;

SPRITE *LevelSprite;

int CurrentLevel = 0;
int NumLevel = 2;

extern SDL_Surface *screen;
extern SDL_Surface *level;
extern Entity *player;



	

void LoadLevelSprites()
{
	LevelSprite = LoadSprite("images/backgroundtest.png",2048,2048,-1,-1,-1);
}

void RenderLevel()
{
	SDL_BlitSurface(LevelSprite->image, NULL, level, NULL);
	 
	
}


void SetCamera()
{
	{
 /*int  i;*/
 int numkeys;
 Uint8 *keys = SDL_GetKeyState(&numkeys);
 
 {
	 if(vcam.x!= 0)
	 {
		 if(vcam.y > 0) vcam.y -=2;
		 else vcam.y += 2;
	 }
	 if(vcam.x!=0)
	 {
		if(vcam.x > 0) vcam.x -=2;
		else vcam.x += 2;
	 }
	Camera.x += vcam.x;
    Camera.y += vcam.y;
	
	if(Camera.x < 0)
	 Camera.x =0;
 if(Camera.y < 0)
	 Camera.y =0;
 if(Camera.x > level->w - Camera.w)
	 Camera.x = (level->w - (Camera.w));
  if(Camera.y > level->h - Camera.h)
	 Camera.y = (level->h - (Camera.h));
   
	 if(keys[SDLK_UP])
  {
         
          if(vcam.y > -5)
			  {
				 vcam.y -= 5;
				  
				  
			  }
		   
  }
  if(keys[SDLK_DOWN])
  {
          
          if(vcam.y < 5)
			  {
				  vcam.y += 5;
				  
		  }
		  
  }
  if (keys[SDLK_LEFT])
  {
	  
	  if(vcam.x > - 5)
		  {
			 vcam.x -= 5;
			  
		  }  
	  
 }
  if (keys[SDLK_RIGHT])
  {
	  
	  if(vcam.x <  5)
		  {
			  vcam.x += 5;
			   

	  }
	 
  }
 }
}
	
	
	
	
	SDL_BlitSurface(level, &Camera, screen, &offset);
	

	
	
/*keeps the camera within the bounds of the level*/
/*
 if(Camera.x < 0)
	 Camera.x =0;
 if(Camera.y < 0)
	 Camera.y =0;
 if(Camera.x > level->w - Camera.w)
	 Camera.x = (level->w - (Camera.w));
  if(Camera.y > level->h - Camera.h)
	 Camera.y = (level->h - (Camera.h));
	 */
}
