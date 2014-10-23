#include <stdlib.h>
#include "wasteland.h"
#include "entity.h"
#include "graphics.h"


extern SDL_Surface *buffer;
extern SDL_Rect Camera;

int CurrentLevel = 0;
int NumLevel = 2;

extern SDL_Surface *screen;

SDL_Surface *screen;

void SetCamera()
{
	
	
	
	
/*keeps the camera within the bounds of the level*/
 if(Camera.x < 0)
	 Camera.x =0;
 if(Camera.y < 0)
	 Camera.y =0;
 if(Camera.x > LEVEL_LENGTH - Camera.w)
	 Camera.x = (LEVEL_LENGTH - (Camera.w));
  if(Camera.y > LEVEL_WIDTH - Camera.h)
	 Camera.y = (LEVEL_LENGTH - (Camera.h));
}
