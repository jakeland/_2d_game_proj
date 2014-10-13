#ifndef __wasteland__
#define __wasteland__

#include "SDL.h"

#define TAGS			4
#define TILERES			12
#define LEVELLENGTH		128
#define LEVELHEIGHT		128

enum Tags {T_Empty, T_POW1, T_POW2, T_Psycho, T_PsychoA, T_Mrdr, T_mrdrA};

typedef struct MAP_NODE
{char tags[TAGS];
char Tiles[TILERES];
}MapNode_T;

typedef struct LEVEL__STRUCT
{
	MapNode_T map [LEVELLENGTH];
	Uint16 length; 
	/* char bgmusic[20]; */
}Level;

void LoadLevelSprites();
void RenderLevel(Level *level);
void SpawnLevelEnts(Level *level);
Level *GetCurrentLevel(int Length);
void UpdateLevel(Level *level);

#endif
