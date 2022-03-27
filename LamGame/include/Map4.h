#ifndef MAP4_H
#define MAP4_H

#include "Common_Function.h"
#include "Weapon.h"
#include "Enemy.h"
#include "Character.h"
#include "TextObject.h"
#include "Map3.h"
static TextObject Gametime;
static Weapon Arrow[4];
static Enemy E[3][100];
static int check[2000][2000];
static int EnemyCount[3];
static bool kt[3][100], canShoot[4];
static vector <pii> EnemyList;
static int NumOfArrow = 1, idArrow = 1, numEnemyKilled = 0;
void loadArrow(SDL_Renderer *ren);
pii RandPos();
void Enemy_Spawn(int id, SDL_Renderer *ren);
bool Can_Move(int x, int y);
void Enemy_Move();
void Arrow_Move();
void Arrow_Shot(int frame, SDL_Renderer *ren);
bool Check_Explorer(int &lastTimeDamage);
void show_game_time(SDL_Renderer *ren, TTF_Font *font);
void show_Enemy_Killed(SDL_Renderer *ren, TTF_Font *font);
void LoadMap4(SDL_Renderer *ren);
void CloseMap4(SDL_Renderer *ren);
bool RunMap4(SDL_Renderer *ren, TTF_Font *font, bool &RunGame);
#endif // MAP4_H
