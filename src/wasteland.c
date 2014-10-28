#include <string.h>
#include <stdlib.h>
#include "wasteland.h"
#include "entity.h"
#include "graphics.h"


extern SDL_Surface *buffer;
extern SDL_Rect Camera;
extern SDL_Rect offset;
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
	
	Camera.x += 1;
	
	
	
	SDL_BlitSurface(level, &Camera, screen, &offset);
	

	
	
/*keeps the camera within the bounds of the level*/
 if(Camera.x < 0)
	 Camera.x =0;
 if(Camera.y < 0)
	 Camera.y =0;
 if(Camera.x > level->w - Camera.w)
	 Camera.x = (level->w - (Camera.w));
  if(Camera.y > level->h - Camera.h)
	 Camera.y = (level->h - (Camera.h));
}
