#include <stdlib.h>
#include <math.h>
#include "entity.h"
#include "wasteland.h"
enum FACTION {E_NONE,E_Player,E_Bugs}; /*labeling system for enemies, decides what effects what. */
extern SDL_Surface *screen;
extern SDL_Surface *level;
extern SDL_Event Event; //This is just so the think functions in this file can read what has been input.
extern SDL_Rect Camera;
extern int NumLevels;
extern int CurrentLevel;
Entity EntityList[MAXENTITIES];
Entity InventoryList[MAXINV];
Entity *Player; 
Entity *ground;
SDL_Rect temp;
Entity *platform;
Entity *oddish;
int NumLives = 3;
int NumEnts;
int MOUSEMOVE = 1;
int lastx,lasty;

SDL_Rect level1_floor;


enum WeaponType {Wp_Shotgun, Wp_Pistol, Wp_Assault, Wp_BuzzAx, Wp_Sniper};

void SetFloor(){
level1_floor.h = 20;
level1_floor.w = 2048;
level1_floor.x = 0;
level1_floor.y =  2018;


}




void DrawEntities()
{
  int i;
  for(i = 0; i < MAXENTITIES;i++)
  {
    if(EntityList[i].used)
    {
      if(EntityList[i].shown)
        DrawEntity(&EntityList[i]);
		
    }
  }
}

void UpdateEntities()
{
  int i;
  for(i = 0;i < MAXENTITIES;i++)
  {
    if(EntityList[i].used)
    {
      if(EntityList[i].think != NULL)
      {
        EntityList[i].think(&EntityList[i]);

      }
    }
  }
}

void UpdateInventory()
{
  int i;
  for(i = 0;i < MAXINV;i++)
  {
    if(InventoryList[i].used)
    {
      if(InventoryList[i].think != NULL)
      {
       InventoryList[i].think(&InventoryList[i]);
	   
      }
    }
  }
}


void DrawEntity(Entity *ent)
{
  DrawSprite(ent->sprite,screen,ent->sx-Camera.x,ent->sy-Camera.y,ent->frame);
}

void DrawPlatform(int x, int y, int w, int h){
	
	temp.x = x;
	temp.y = y;
	temp.h = h;
	temp.w = w;
	SDL_FillRect(level, &temp, 8);
}

void InitEntityList()
{
  int i/*, j*/;
  NumEnts = 0;
  for(i = 0;i < MAXENTITIES; i++)
  {
    EntityList[i].sprite = NULL;
    EntityList[i].owner = NULL;
    EntityList[i].think = NULL;
   /* for(j = 0;j < SOUNDSPERENT;j++)
    {
      EntityList[i].sound[j] = NULL;
    }
	*/
    EntityList[i].shown = 0;
    EntityList[i].used = 0;
  }
}
   
void InitInvList()
{
  int i/*, j*/;
  NumEnts = 0;
  for(i = 0;i < MAXINV; i++)
  {
    InventoryList[i].used = 0;
  }
}

Entity *NewEntity()
{
  int i;
  if(NumEnts + 1 >= MAXENTITIES)
  {
    return NULL;
  }
  NumEnts++;
  for(i = 0;i <= NumEnts;i++)
  {
    if(!EntityList[i].used)break;
  }
  EntityList[i].used = 1;
  return &EntityList[i];
}

/*done with an entity, now give back its water..I mean resources*/
void FreeEntity(Entity *ent)
{
  /*int j;*/
  ent->used = 0;
  NumEnts--;
  if(ent->sprite != NULL)FreeSprite(ent->sprite);
 /* for(j = 0;j < SOUNDSPERENT;j++)
  {
    if(ent->sound[j] != NULL)FreeSound(ent->sound[j]);
    ent->sound[j] = NULL;
  }
  */
  ent->sprite = NULL;
  ent->owner = NULL;
}

/*kill them all*/
void ClearEntities()
{
  int i = 0;
  for(i = 0;i < MAXENTITIES;i++)
  {
    FreeEntity(&EntityList[i]);
  }
}

/*
 * This handly little function will figure pace out a looping animation for an entity.
 * It could probably have been a macro...oh well.
 */
int GetNextCount(Entity *self)
{
  return (self->count+1)%(self->framestates[self->state + 1] - self->framestates[self->state]);
}

void ScaleVectors(float *vx, float *vy)
{
  float hyp;
  hyp = sqrt((*vx * *vx) + (*vy * *vy));
  hyp = 1 / hyp;
  *vx = (*vx * hyp);
  *vy = (*vy * hyp);
}

int Collide(SDL_Rect box1,SDL_Rect box2)
{
  /*check to see if box 1 and box 2 clip, then check to see if box1 is in box or vice versa*/
  if((box1.x + box1.w >= box2.x) && (box1.x <= box2.x+box2.w) && (box1.y + box1.h >= box2.y) && (box1.y <= box2.y+box2.h))
    return 1;
  return 0;
}

Entity *HitNextEnt(Entity *self,Entity *target)
{
  int i;
  SDL_Rect b1,b2;
  b1.x = self->sx + self->bbox.x;
  b1.y = self->sy + self->bbox.y;
  b1.w = self->bbox.w;
  b1.h = self->bbox.h;
  for(i = 0;i < MAXENTITIES; i++)
  {
    if(target == &EntityList[i])break;
  }
  for(i = i;i < MAXENTITIES; i++)
  {
    if(EntityList[i].used)
    {
      if((EntityList[i].health > 0)&&(EntityList[i].enemy != E_NONE)&&(EntityList[i].enemy != self->enemy))
      {
        b2.x = EntityList[i].sx + EntityList[i].bbox.x;
        b2.y = EntityList[i].sy + EntityList[i].bbox.y;
        b2.w = EntityList[i].bbox.w;
        b2.h = EntityList[i].bbox.h;
        if(Collide(b1,b2))
         return &EntityList[i];
      }
    }
  }
  return NULL;
}



Entity *GetClosestTarget(Entity *self)
{
  int i;
  int range = 2000;
  int Trange = 2000;
  Entity *target = NULL;
  for(i = 0;i < MAXENTITIES; i++)
  {
    if(EntityList[i].used)
    {
      if((EntityList[i].health > 0)&&(EntityList[i].enemy != E_NONE)&&(EntityList[i].enemy != self->enemy))
      {
        Trange = sqrt((EntityList[i].sx - self->sx)*(EntityList[i].sx - self->sx) + (EntityList[i].sy - self->sy)*(EntityList[i].sy - self->sy));
        if(Trange < range)
        {
          target = &EntityList[i];
          range = Trange;
        }
      }
    }
  }
  return target;
}


Entity *HitEnt(Entity *self)
{
  int i;
  SDL_Rect b1,b2;
  b1.x = self->sx + self->bbox.x;
  b1.y = self->sy + self->bbox.y;
  b1.w = self->bbox.w;
  b1.h = self->bbox.h;
  for(i = 0;i < MAXENTITIES; i++)
  {
    if(EntityList[i].used)
    {
      if((EntityList[i].health > 0)&&(EntityList[i].enemy != E_NONE)&&(EntityList[i].enemy != self->enemy))
      {
        b2.x = EntityList[i].sx + EntityList[i].bbox.x;
        b2.y = EntityList[i].sy + EntityList[i].bbox.y;
        b2.w = EntityList[i].bbox.w;
        b2.h = EntityList[i].bbox.h;
        if(Collide(b1,b2))
         return &EntityList[i];
      }
    }
  }
  return NULL;
}

Entity *HitPlat(Entity *self)
{
  int i;
  SDL_Rect b1,b2;
  b1.x = self->sx + self->bbox.x;
  b1.y = self->sy + self->bbox.y;
  b1.w = self->bbox.w;
  b1.h = self->bbox.h;
  for(i = 0;i < MAXENTITIES; i++)
  {
    if(EntityList[i].used)
    {
      if(EntityList[i].enemy == E_NONE)
      {
        b2.x = EntityList[i].sx + EntityList[i].bbox.x;
        b2.y = EntityList[i].sy + EntityList[i].bbox.y;
        b2.w = EntityList[i].bbox.w;
        b2.h = EntityList[i].bbox.h;
        if(Collide(b1,b2))
         return &EntityList[i];
      }
    }
  }
  return NULL;
}

Entity *SpawnBullet(Entity *Owner, int sx, int sy, int vy, int vx, int damage, int WeaponType, int enemy)
{
	Entity *blast;
	blast = NewEntity();
	if(blast == NULL)
		return blast;
	blast->owner = Owner;
	blast->sprite = LoadSprite("images/effects.png",16,16,-1,-1,-1);
	
	blast->shown =1;
	blast->health = damage;
	blast->sx = sx;
	blast->sy = sy;
	blast->vy = vy;
	blast->vx = vx;
	
	blast->enemy = enemy;  
	switch(WeaponType)
	{
	case Wp_Shotgun:
		blast->frame = 6;
		blast->state = ST_IDLE;
		blast->think = ShotgunThink;
		
		blast->bbox.x= 0;
		blast->bbox.y = 0;
		blast->bbox.w = 3;
		blast->bbox.h  = 2;
	break;
	case Wp_Pistol:
		blast->frame = 8;
		blast->state = ST_IDLE;
		blast->think = PistolThink;
		blast->bbox.x= 6;
		blast->bbox.y = 5;
		blast->bbox.w = 3;
		blast->bbox.h  = 2;
	case Wp_Assault:
		blast->frame = 9;
		blast->state = ST_IDLE;
		blast->think = AssaultThink;
		blast->bbox.x= 0;
		blast->bbox.y = 0;
		blast->bbox.w = 3;
		blast->bbox.h  = 2;
	break;
	default:
		blast->frame = 6;
		blast->state = ST_IDLE;
		blast->think = PistolThink;
		blast->bbox.x = 6;
		blast->bbox.y = 5;
		blast->bbox.w = 3;
		blast->bbox.h = 2;
	};
}

void ShotgunThink(Entity *self)
{
  Entity *target;
    /*Update position*/
  self->sx += self->vx;
  self->sy += self->vy;
    /*Check for colision if not already dying*/
  if(self->sx > level->w || self->sx < 0 || self->sy < 0 ||self->sy > level->h)
  { //We have gone off the map, there be dragons here, so lets get rid of us.
    self->shown = 0;
    FreeEntity(self);
    return;
  }
  if(self->state != ST_DYING)
  {
    target = HitEnt(self);
    if(target != NULL)
    {
	  
      self->state = ST_DYING;
      self->vx = 0;
      self->vy = 0;
      target->health -= self->health;
      target->vx += self->vx * self->health / 2;
	  if(target->health < 0)self->health = target->health * -1;
    }
	target = HitPlat(self);
	if (target!= NULL)
	{
		self->state = ST_DYING;
		self->vx= 0;
		self->vy = 0;
	}
		
  }
  switch(self->state)
  {
    case ST_DYING:
      self->frame++;
      if(self->frame >= 10)FreeEntity(self);
    break;

  }
}


void AssaultThink(Entity *self)
{
  Entity *target;
    /*Update position*/
  self->sx += self->vx;
  self->sy += self->vy;
    /*Check for colision if not already dying*/
  if(self->sx > level->w || self->sx < 0 || self->sy < 0 ||self->sy > level->h)
  { //We have gone off the map, there be dragons here, so lets get rid of us.
    self->shown = 0;
    FreeEntity(self);
    return;
  }
  if(self->state != ST_DYING)
  {
    target = HitEnt(self);
    if(target != NULL)
    {
	  
      self->state = ST_DYING;
      self->vx = 0;
      self->vy = 0;
      target->health -= self->health;
      target->vx += self->vx * self->health / 2;
    }
	target = HitPlat(self);
	if (target!= NULL)
	{
		self->state = ST_DYING;
		self->vx= 0;
		self->vy = 0;
	}
		
	
  }
  switch(self->state)
  {
    case ST_DYING:
		{
      self->frame++;
      if(self->frame >= 10)
		  FreeEntity(self);
		}
    break;
	case ST_IDLE:
		{

			break;
		}
	default:
		self->state = ST_IDLE;
		break;
  }
}



void PistolThink(Entity *self){

}

Entity *MakePlatform(int x, int y,int width, int height)
{
	Entity *platform;
	platform = NewEntity();
	if  (platform == NULL)
	{
		return platform;
	}
	platform->sx = x;
	platform->sy = y;
	platform->bbox.x=0;
	platform->bbox.y=0;
	platform->bbox.w=width;
	platform->bbox.h=height;
	platform->gravity = 0;
	platform->health = NULL;
	platform->enemy = E_NONE;
	DrawPlatform(x,y, width, height);
	ground = platform;
	return platform;



}

Entity *MakePok(){
	Entity *pok;
	pok = NewEntity();
	if(pok == NULL) return pok;
	pok->sprite = LoadSprite("images/mantid.png", 32,32,-1,-1,-1);
	pok->facing = 0;
	pok->bbox.x = 3;
	pok->bbox.y = 3;
	pok->bbox.w = 22;
	pok->gravity = 1;
	pok->grounded = 0;
	pok->bbox.h = 30; 
	pok->weaplevel = 0;
	pok->currentweapon = 0;
	pok->sx = 400;
	pok->sy = 400;
	pok->shown = 1;
	pok->state = ST_IDLE;
	pok->think = PokThink;
	pok->health = 35;
	pok->healthmax = 35;
	pok->ammo = 0;
	pok->owner = pok;
	pok->fmod = 1;
	pok->enemy = E_Player;
	oddish = pok;
	pok->facing = 0;
	pok->frame = 0;
	return pok;


}



void PokThink(Entity *self){
	/*what even is*/
	Entity *target;
	SDL_Rect b1, b2;
	
	
	


	b2.x = Player->sx + Player->bbox.x;
	b2.y = Player->sy + Player->bbox.y;
	b2.w = Player->bbox.w;
	b2.h = Player->bbox.h;
	
	b1.x = self->sx + self->bbox.x;
	b1.y = self->sy + self->bbox.y;
	b1.w = self->bbox.w;
	b1.h = self->bbox.h;


	
	target = HitPlat(self); 
			
	if (target != NULL)
	{
		if(self->grounded !=1)
		{
			
			self->sy  = target->sy - self->bbox.h;
			self->grounded = 1;
					
					
					
		}
	}

	 if(self->grounded ==1) /*grounded*/
	 {
		
		 self->vy = 0 ;

	 }
	 else /*not grounded*/
	 {
		 if(self->vy <4)
		 self->vy += 2;
	   

	 }
	 self->sx += self->vx;
	
     self->sy += self->vy;
	

	if (self->vx>0)
		self->vx -=2;
	if (self->vy>0)
		self->vy -=1;
	if (self->vx<0)
		self->vx +=1;
	if (self->vy<0)
		self->vy +=2;
    if(self->sy < 0)self->sy = 0;
	if(self->sx < 0)self->sx = 0;
	if(self->sx > level->w-self->bbox.w)self->sx =  level->w-self->bbox.w;
	if(self->sy > level->h-self->bbox.h)self->sy =	level->h-self->bbox.h;
	 if((self->sx > b2.x -10 || self-> sx < b2.x + 10) && self->sy >= b2.y)
	{
		if(self->health > 0){
		if(self->grounded == 1)
			
			self->state = ST_HUNTING;
		}
	}
	 if (self->sy <0)
	{
		self->sy  = 0;
		self->vy = 0;
		self->delay += rand() % 20;
	}
	
	else if ((Player->health > 0) && (self->state != ST_DYING) && (Collide(b1, b2)))
	{
		Player->health -= 8;
		self->state = ST_DYING;
		self->frame = 7;
		self->vy = -self->vy *3;

	}
	
	 if (self->health <= 0)
	 {
		 self->state = ST_DYING;
	 }

	if (self->sx < -32)
	{
		self->shown = 0;
		FreeEntity(self);

	}

	if (self->sy > level->h +32)
	{

		self->shown =0;
		FreeEntity(self);
	}
	if (self->sx<b2.x)
	{
		self->facing = F_RIGHT;
		
	}

	if (self->sx>b2.x)
	{
		self->facing = F_LEFT;
	}
	
	switch(self->facing)
	{
	case F_LEFT:
		{
			/**/
			self->fmod = -1;
			break;
		}
	case F_RIGHT:
		{
			/**/
			self->fmod = 1;
			break;
		}

	default:
		{
			 /**/
			if(self->sx < b2.x)
			{
				self->fmod = -1;
			}
			else
			{
				self->fmod = 1;
			}
			break;
		}
		   
	}
	
	switch(self->state)
	{
	case ST_DYING:
		if((rand() % 8) == 0)
		{
			/*drop items...*/
		}
		FreeEntity(self);
		
		break;
	case ST_FIRE1:



		break;
	case ST_HUNTING:
		{
			
			self->vx=2 * self->fmod;
			break;
		}
	case ST_IDLE:
		self->vx = 0;
	if (self->delay<0)
		self->frame++;
	if(self->frame>=3)
		self->frame = 0;
	if(self->delay<=0) 
		self->delay += rand()%26;
	if(self->delay > 0) 
		self->delay--;
	
	break;
	
	}
	
}


Entity *MakePlayer()
{
  Entity *dude;
  dude = NewEntity();
  if(dude == NULL)return dude;
  dude->sprite = LoadSprite("images/ship.png",32,32,-1,-1,-1); //Load the dude file, no palette swapping
  /*
  dude->sound[0] = LoadSound("sounds/blaster.wav",MIX_MAX_VOLUME/5);
  dude->sound[1] = LoadSound("sounds/redblast.wav",MIX_MAX_VOLUME/5);
  dude->sound[2] = LoadSound("sounds/xplode.wav",MIX_MAX_VOLUME/5);
  dude->sound[3] = LoadSound("sounds/explode.wav",MIX_MAX_VOLUME/5);
  */
  dude->facing = 0;
  dude->sx =200;
   dude->sy = 10;
  dude->bbox.x = 0;
  dude->bbox.y = 0;
  dude->bbox.w = 30;
  dude->bbox.h = 30;
  dude->weaplevel = 0;
  dude->currentweapon = 0;
  dude->frame = F_DOWN;
  dude->jumpdelay = 10;
  dude->grounded = 0;
 
  dude->gravity = 1;
  dude->shown = 1;
  dude->state = ST_IDLE;
  dude->think = PlayerThink;
  dude->health = 30;
  dude->healthmax = 30;
  dude->ammo = 5;
  dude->owner = dude;
  dude->enemy = E_Bugs;
  dude->canjump = 0;
  Player = dude;
  return dude;
}



void PlayerThink(Entity *self)
{
	Entity *target;    
 /*int  i;*/



 int numkeys;
 Uint8 *keys = SDL_GetKeyState(&numkeys);
 if(self->heat > 0)self->heat--;
 if(self->busy>0)self->busy--;
 if(self->health >0)
	 
 {
	 if (self->state != ST_DYING)
		{
			target = HitPlat(self); 
			
			if (target != NULL)
			{
				if(self->grounded !=1)
				{
					self->sy = target->sy - self->bbox.h;
					self->grounded = 1;
					
					
					
				}
			}
			


		}
	 if(self->grounded ==1) /*grounded*/
	 {
		self->canjump = 2;
		 self->vy = 0 ;
		 self->grounded = 0;
		 self->canjump = 2;

	 }
	 else /*not grounded*/
	 {
		 if(self->vy <4)
		 self->vy += 2;
	    self->jumpdelay = 3;

	 }
	 
		if(keys[SDLK_SPACE])
		{
			if(self->jumpdelay <= 0 && self->canjump != 0)
			{
				self->grounded = 0;
				self->vy= -30;
				self->canjump--;
			}
			
		}
	 
	 if(keys[SDLK_UP])
  {
       self->facing = F_UP;  
	  
	     
  }
  if(keys[SDLK_DOWN])
  {
      self->facing = F_DOWN;    
	  
	     
  }
  if (keys[SDLK_LEFT])
  {
	  
	  self->facing = F_LEFT;
	    self->vx-=2;
	     
 }
  if (keys[SDLK_RIGHT])
  {
	 self->facing = F_RIGHT;
	 self->vx+=2;
  }
  if (keys[SDLK_d])
  {
	  if(self-> state == ST_IDLE)
	  {
		  self->state = ST_FIRE1;
	  }
 }
  if (keys[SDLK_a])
  {
		if(self->state == ST_IDLE)
		{
			self->state = ST_FIRE2;
			
		}

  }
	self->sx += self->vx;
	
    self->sy += self->vy;
	

	if (self->vx>0)
		self->vx -=2;
	if (self->vy>0)
		self->vy -=1;
	if (self->vx<0)
		self->vx +=2;
	if (self->vy<0)
		self->vy +=2;
    if(self->sy < 0)self->sy = 0;
	if(self->sx < 0)self->sx = 0;
	if(self->sx > level->w-self->bbox.w)self->sx =  level->w-self->bbox.w;
	if(self->sy > level->h-self->bbox.h)self->sy =	level->h-self->bbox.h;
	
	self->jumpdelay -= 2;
  switch(self->state)
  {

  case ST_DEAD:
	  {
	  self->frame =13;
	  self->shown = 0;
	  if(self->delay <= 0)
	  {
		  if(NumLives > 0)
		  {
			  self->health = self->healthmax;
			  self->state = ST_IDLE;
			  self->shown = 1;
			  NumLives--;
		  }
		  else exit (1);
	  }
	  else self->delay --;
	break;
	  }
   case ST_DYING:
	   {
      if(self->frame >= 13)
      {
        self->state = ST_DEAD;
        self->delay = 10;
      }
      self->frame++;
	  
    break;
	   }
  case ST_IDLE:
	  {
	  switch(self->facing)
	  {
	  case F_UP:
	  self->frame = 0;
	  break;
	  case F_DOWN:
	  self->frame = 4;
	  break;
	  case F_LEFT:
	  self->frame = 8;
	  break;
	  case F_RIGHT:
	  self->frame = 12;
	  break;
	  }
	  break;
	  }
  case ST_FIRE1:
	  {
	   if(self->delay > 0)
      {
        self->delay--;
        if(self->delay <= 0)
        {
          self->delay = 0;
          
        }
      }
      else if(self->frame >= 0)
	  {
		  switch(self->facing)
		  {
  case F_RIGHT:
		  SpawnBullet(self, self->sx + 10, self->sy + 3, 0,8,2, Wp_Assault,E_Bugs);
		  self->delay=14;
		  break;
  case F_UP:
		  SpawnBullet(self, self->sx + 10, self->sy + 3, -8,0,2, Wp_Assault,E_Bugs);
		  self->delay=14;
		  break;
  case F_LEFT:
		  SpawnBullet(self, self->sx + 10, self->sy + 3, 0,-8,2, Wp_Assault,E_Bugs);
		  self->delay=14;
		  break;
  case F_DOWN:
		  SpawnBullet(self, self->sx + 10, self->sy + 3, 8,0,2, Wp_Assault,E_Bugs);
		  self->delay=14;
		  break;
		  }
		self->state = ST_IDLE;

		  
	  }
	  else self->frame++;
	  break;
	  }
  case ST_FIRE2:
	  {
	   if(self->delay > 0)
      {
        self->delay--;
        if(self->delay <= 0)
        {
          self->delay = 0;
          
        }
      }
      else if(self->frame >= 0)
      {
			switch (self->facing)
			{
				case F_UP:
				{	
					SpawnBullet(self,self->sx + 10,self->sy + 3,-5,0,3,Wp_Shotgun,E_Bugs);
					SpawnBullet(self,self->sx + 10,self->sy + 3,-3,2,3,Wp_Shotgun,E_Bugs);
					SpawnBullet(self,self->sx + 10,self->sy + 3,-3,-2,3,Wp_Shotgun,E_Bugs);
					self->delay = 20;
					break;
				}
			  case F_DOWN:
				{
					
					SpawnBullet(self,self->sx + 10,self->sy + 3,5,0,3,Wp_Shotgun,E_Bugs);
					SpawnBullet(self,self->sx + 10,self->sy + 3,3,2,3,Wp_Shotgun,E_Bugs);
					SpawnBullet(self,self->sx + 10,self->sy + 3,3,-2,3,Wp_Shotgun,E_Bugs);
					self->delay = 20;
					break;
					
					
				}
			 case F_RIGHT:
				{
					SpawnBullet(self,self->sx + 10,self->sy + 3,-2,3,3,Wp_Shotgun,E_Bugs);
					SpawnBullet(self,self->sx + 10,self->sy + 3,0,5,3,Wp_Shotgun,E_Bugs);
					SpawnBullet(self,self->sx + 10,self->sy + 3,2,3,3,Wp_Shotgun,E_Bugs);
					self->delay = 20;
					 break;
          
					
				}
			case F_LEFT:
				{
					SpawnBullet(self,self->sx + 10,self->sy + 3,-2,-3,3,Wp_Shotgun,E_Bugs);
					SpawnBullet(self,self->sx + 10,self->sy + 3,0,-5,3,Wp_Shotgun,E_Bugs);
					SpawnBullet(self,self->sx + 10,self->sy + 3,2,-3,3,Wp_Shotgun,E_Bugs);
					self->delay = 20;
					
					break;
				}
			}
				/*spawn the little blue bullet*/
				self->state = ST_IDLE;
			 
			}
      else self->frame++;
    break;
    
  }
  default:
	  {
	self->state= ST_IDLE;
	break;
	  }


  
 }
 
}

}


