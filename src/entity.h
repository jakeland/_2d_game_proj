#ifndef _ENTITY_
#define _ENTITY_

#include "graphics.h"


/*
  This file contains the data structures and function prototypes for handling entities.
  Entities all contain information about the associate sprite that may be unique to itself
  as well as the animation information, if it has any.
*/
#define MAXINVENTORY 100
#define QUESTLIST	 100
#define MAXENTITIES   511
#define MAXINV		  20
#define MAXSTATES     20
#define SOUNDSPERENT  4

enum STATES {ST_IDLE,ST_FIRE1,ST_FIRE2,ST_FIRE3, ST_HUNTING, ST_POWER1, ST_POWER2, ST_POWER3, ST_ATTACK,ST_ATTACK1, ST_ATTACK2, ST_ATTACK3, ST_DYING,ST_DEAD, ST_EVOLVING};
enum FACING {F_UP, F_DOWN, F_LEFT, F_RIGHT};
typedef struct ENTITY_T
{
  SPRITE *sprite;
  
  struct ENTITY_T *owner;
  void (*think) (struct ENTITY_T *self);
  /*the think function is the function called each frame to update the state of the entity*/
  int shown; /*if 1 then it will be rendered when it is on screen*/
  int used; /*used is for keeping track of what entities are free and should NEVER be touched.*/
  int frame;
  int isBoss;
  /*would like to make this it's own entity class, but as I don't have time for that it will have to wait*/
  int currentweapon; /*0,1,2*/
  int facing; /* 0, 1, 2 ,3 */
  int count;  /*used for animation*/
  int state;  /*making each entity a finite state machine.*/
  int sx,sy; /*sprite's coordinates*/
  int animframe;  /*specifies how many global frames go by before the sprite frame is animated.*/
  int busy;   /*used for some think functions when issuing a command, if its busy with an animation it won't bother the sprite*/
  int delay;
  int fdelay; /*used to figure out frame delays, not firing*/
  int vx,vy; /*vector values*/
  int gravity; /*0 = No, 1 = yes*/
  int grounded; /*0 = No, 1 = yes*/
  int canjump;
  int idleframe;
  int maxidle;
  int walkframe;
  int maxwalk;
  int attackframe;
  int maxattack;
  int type; /*0 = oddish, 1 = gloom, 2 = charmander, 3 = charizard, 4 = pikachu, 5 = raichu*/
  int evolvetype;
  int jumpdelay;
  /*an entity can have up to MAXSTATES states.  Each state shows the fram it Ends on, the previous state marks
    the begining.*/
  int framestates[MAXSTATES];
  /*the absolute screen coordinates and the map coordinates*/
  SDL_Rect bbox;  //The bounding box for hit detection, x and y are offsets from the position.
  int weaplevel;
  int health;
  int healthmax;
  int ammo;
  int heat;
  int fmod; /*facing modifier, -1 = left, 1 = right */
  int enemy;
}Entity;


void InitEntityList();
Entity *NewEntity();
void FreeEntity(Entity *ent);
void ClearEntities();
void DrawEntities();
void DrawEntity(Entity *ent);
void UpdateEntities();
void UpdateDisplayBar(Entity *player,SPRITE *window);

int GetNextCount(Entity *self);

void DrawPlatform(int x, int y, int height, int width);
void SetFloor();
Entity *MakePlayer(); 
Entity *MakePok(int x, int y);
Entity *MakePok3(int x, int y);
Entity *MakePlatform(int x, int y, int width, int height);
void PlayerThink(Entity *self);
void PokThink(Entity *self);
void PokThink3(Entity *self);
void StoneThought(Entity *self);
Entity *MakePok2();
Entity *SpawnBullet(Entity *Owner, int sx, int sy, int vx, int vy, int damage, int type, int enemy);
void ShotgunThink(Entity *self);
void PistolThink(Entity *self);
void AssaultThink(Entity *self);
#endif
