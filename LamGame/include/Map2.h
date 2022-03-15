#ifndef MAP2_H
#define MAP2_H

#include "Common_Function.h"
#include "TextObject.h"
#include "LoadObject.h"
static LoadObject Sphinx, Talk, guess, button, blank;
static SDL_Rect Rtalk, Rguess, Rbutton, Rblank;
static TextObject TextGuess;
void LoadMap2(SDL_Renderer *ren);
void CloseMap2(SDL_Renderer *ren);
void Intro2(SDL_Renderer *ren);
int to_num(string s);
bool RunMap2(SDL_Renderer *ren, TTF_Font *font);

#endif // MAP2_H
