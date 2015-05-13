#include <string.h>
#include <stdlib.h>
#include "wasteland.h"
#include "entity.h"
#include "graphics.h"


extern SDL_Surface *buffer;
extern SDL_Rect Camera;
extern SDL_Rect offset;
extern SDL_Rect vcam;

extern Entity *Player;

SPRITE *LevelSprite;

int CurrentLevel = 0;
int NumLevel = 2;

extern SDL_Surface *screen;
extern SDL_Surface *level;
extern Entity *player;



	

void LoadLevelSprite()
{
	
	LevelSprite = LoadSprite("images/backgroundtest.png",2048,2048,-1,-1,-1);
}

void RenderLevel()
{
	
	SDL_BlitSurface(LevelSprite->image, NULL, level, NULL);
	
	
}


void SetCamera()
{
	
	
	if(Player!= NULL)
	{
	vcam.x = ((int)Player->sx + (int)Player->bbox.w/2) - screen->w/2;
	vcam.y = ((int)Player->sy + (int)Player->bbox.h/2) - screen->h/2;
	}
	

	
 if(vcam.x < 0)
	 vcam.x =0;
 if(vcam.y < 0)
	 vcam.y =0;
 if(vcam.x > level->w - Camera.w)
	 vcam.x = (level->w - (Camera.w));
  if(vcam.y > level->h - Camera.h)
	 vcam.y = (level->h - (Camera.h));

	Camera.x = vcam.x;
	Camera.y = vcam.y;

	

	

	
	
	
	
/*keeps the camera within the bounds of the level*/

	 
  /*SDL_BlitSurface (level, &Camera, screen, NULL); */
}
