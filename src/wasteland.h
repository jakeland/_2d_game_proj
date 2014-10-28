#ifndef __wasteland__
#define __wasteland__


#define LEVEL_LENGTH	2048
#define LEVEL_WIDTH		2048

#include "SDL.h"
#include "SDL_image.h"
typedef struct LEVEL_STRUCT
{
	Uint16 length;
	Uint16 width;

}Level;

void LoadLevelSprite();
void SetCamera();
void RenderLevel();

#endif