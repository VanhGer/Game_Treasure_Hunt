#ifndef MAP3_H
#define MAP3_H

#include "Common_Function.h"
#include "TextObject.h"
#include "LoadObject.h"
#include "Weapon.h"
static LoadObject lava[50], die, youdied, glow, arr;
static int info[1121][6001], dir_snake[35];
static int nlava, nsnake;
static pii CoorOst[50];
static Weapon fire[10], eball[10], snake[35];
static bool Fappear[10], Eappear[10];
static Uint64 Ftime[10], Etime[10];
static SDL_Rect RMap3, Rlava[2], Rarr;
void LoadObstacle(SDL_Renderer *ren);
void LoadMap3(SDL_Renderer *ren);
void CloseMap3(SDL_Renderer *ren);
void Intro3(SDL_Renderer *ren, bool &RunGame);
bool CheckInside(SDL_Rect R, int x, int y, int h, int w);
bool CheckIntersect(int x, int y, int w, int h, int x1, int y1, int w1, int h1);
void Spawn_Ost(SDL_Renderer *ren, int n);
void show_ExHp(MainCharacter &Explorer, SDL_Renderer *ren, TTF_Font *font);
void ChangeRMap3(int val);
void Explorer_Move(MainCharacter &Explorer, SDL_Event e, int minX, int maxX, int minY, int maxY);
void CheckCollision3();
bool RunMap3(SDL_Renderer *ren, TTF_Font *font, bool &RunGame);
#endif // MAP3_H
