#ifndef __wasteland__
#define __wasteland__

#include "SDL.h"

#define WASTE_W		128
#define WASTE_H		128
#define MAX_ENT		128
#define MAX_OBJ		128


typedef struct INFO_TAG
{
	char name[40];
	Uint16 sx,sy;
	Uint16 UnitInfo;
	Uint16 UnitType;
	char info[40];
}Tag;

typedef struct LEVEL_STRUCT
{
	Uint8	tilemap[WASTE_H][WASTE_W];
	Uint16	width, height;
	Tag		infolist[MAX_OBJ];
	int		infocount;
	Tag		spawnlist[MAX_ENT];
	int		spawncount;
	Uint8	tileset;
	char	levelname[40];
	char	bgimage[40];
}Level;

void LoadLevelSprites();
void RenderLevel(Level *level);
void SpawnLevelEnts(Level *level);
Level *GetCurrentLevel(int level);
int MoveCamera(int Length, int Height);
void UpdateLevel(Level *level);


void GenerateLevel(int x, int y);
void LoadLevel(char *filename);
void SaveLevel(char *filename);
void DrawLevel();
void CloseLevel();



#endif