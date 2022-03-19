#ifndef MAP3_H
#define MAP3_H

#include "Common_Function.h"
#include "TextObject.h"
#include "LoadObject.h"
#include "Map4.h"
static LoadObject lava[240];
static int info[1121][4501];
static int Otype[30];
static int nlava;
static pii CoorOst[240];
static Weapon fire, eball;
static SDL_Rect RMap3, Rlava[2];
void LoadObstacle(SDL_Renderer *ren);
void LoadMap3(SDL_Renderer *ren);
void CloseMap3(SDL_Renderer *ren);
void Intro3(SDL_Renderer *ren);
bool checkinside(SDL_Rect R, int x, int y, int h, int w);
bool checkintersect(int x, int y, int w, int h, int x1, int y1, int w1, int h1);
void RunMap3(SDL_Renderer *ren);
#endif // MAP3_H
