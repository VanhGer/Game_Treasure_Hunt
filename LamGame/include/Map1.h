#ifndef MAP1_H
#define MAP1_H

#include "Common_Function.h"
#include "TextObject.h"
#include "LoadObject.h"
#include "MainCharacter.h"
static LoadObject rock, paper, scissors, monk, tg[3], choice[3];
static SDL_Point pos[3];
static SDL_Rect OTT[3], RNumber;
void show_num(SDL_Renderer *ren, TTF_Font *font, int a, int b);
void show_frame(SDL_Renderer *ren, TTF_Font *font, int id);
void Ex_come(MainCharacter &Explorer, SDL_Renderer *ren, bool &RunGame);
void LoadMap1(SDL_Renderer *GRenderer);
void CloseMap1(SDL_Renderer *GRenderer);
void Intro(SDL_Renderer *GRenderer, bool &RunGame, int id);
bool RunMap1(SDL_Renderer *GRenderer, TTF_Font *font, bool &RunGame);

#endif // MAP1_H
