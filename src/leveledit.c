#include <string.h>
#include <stdlib.h>

#include "wasteland.h"
#include "entity.h"
#include "graphics.h"
#include "leveledit.h"

Entity *ground;
int NumPlats;
Entity PlatList[MAXPLATS];
int evolutionlevel;


/*Broke and needs fixing, striped out everything that didn't work, which was everything....*/

/*
const int cols = 32;
const int rows = 32;

int levelGrid [cols][rows];
void setup(){
	int i, j;
	



}



Entity *CreateCell(){
	

}
*/

/*really bad code, just used for testing, will fix. */


void CreatePlats()
{
	ground = MakePlatform(0,1994,200,15);
	ground = MakePlatform(0,1994,200,15);

	ground = MakePlatform(40,1800,200,15);
	ground = MakePlatform(290,1900,100, 15);
	ground = MakePlatform(400,300, 200, 15);

	ground = MakePlatform(1000, 1420,200, 15);

	ground = MakePlatform(900, 1420, 100, 15);
	ground = MakePlatform(1900, 1300, 100, 15);

	
}









