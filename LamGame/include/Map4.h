#ifndef MAP4_H
#define MAP4_H

#include "Common_Function.h"
#include "Weapon.h"
#include "Enemy.h"
#include "Character.h"
#include "TextObject.h"
#include "Map3.h"
static Weapon Arrow[4];
static int NumEnemy = 0, DamnRange = 25, AttackSpeed = 500, kt[4], damn[5] = {1, 2, 1, 2, 4};
static bool canShoot[4], finish[4];
static Enemy EnemyList[1000];
static int NumOfArrow = 1, idArrow = 1, numEnemyKilled = 0;
static SDL_Rect RMap4;
void loadArrow(SDL_Renderer *ren);
bool Can_Move(int x, int y, int id);
void Enemy_Move(MainCharacter &Explorer);
void Set_Arrow(int curx, int cury);
void CheckCollision4(MainCharacter &Explorer, Uint64 &LastTimeDamage);
void Arrow_Shot(int frame, SDL_Renderer *ren);
void Change_Arrow(SDL_Renderer *ren);
void show_game_time(SDL_Renderer *ren, TTF_Font *font, Uint64 RunningTime);
void show_Enemy_Killed(SDL_Renderer *ren, TTF_Font *font);
void show_Damn(SDL_Renderer *ren, TTF_Font *font);
void Circle_Spawn(SDL_Renderer *ren, int tt, int curx);
void Normal_Spawn(SDL_Renderer *ren, int tt, int curx);
void ChangeRMap4(int curx, int cury);
void Enemy_Display(SDL_Renderer *ren);
void LoadMap4(SDL_Renderer *ren);
void CloseMap4(SDL_Renderer *ren);
bool RunMap4(SDL_Renderer *ren, TTF_Font *font, bool &RunGame);
#endif // MAP4_H
