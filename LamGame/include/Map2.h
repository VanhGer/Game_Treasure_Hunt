#ifndef MAP2_H
#define MAP2_H

#include "Common_Function.h"
#include "TextObject.h"
#include "LoadObject.h"
#include "Map1.h"
static LoadObject Sphinx, Talk, guess, gframe;
static SDL_Rect Rtalk, Rguess, Rsphinx, Rgframe;
static TextObject TextGuess;
void movingin(MainCharacter &Explorer, SDL_Renderer *ren, bool &RunGame);
void talk(MainCharacter &Explorer, SDL_Renderer *ren, bool &RunGame);
void LoadMap2(SDL_Renderer *ren);
void CloseMap2(SDL_Renderer *ren);
int to_num(string s);
bool RunMap2(SDL_Renderer *ren, TTF_Font *font, bool &RunGame);

#endif // MAP2_H
