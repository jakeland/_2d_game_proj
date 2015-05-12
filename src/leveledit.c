#include <string.h>
#include <stdlib.h>

#include "wasteland.h"
#include "entity.h"
#include "graphics.h"
#include "leveledit.h"

Entity *ground;
int NumPlats;
Entity PlatList[MAXPLATS];

/*really bad code, just used for testing, will fix. */

/*for now, I'm just going to create an array, and then fix/create my level editor to emulate this behavior*/


void CreatePlats()
{
	ground = MakePlatform(0,1994,1994,15);
	ground = MakePlatform(40,1800,200,15);
	ground = MakePlatform(290,1900,100, 15);
	ground = MakePlatform(400,300, 200, 15);
}









