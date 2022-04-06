#ifndef MAP5_H
#define MAP5_H

#include <Common_Function.h>
#include <LoadObject.h>
static LoadObject treasure, congra, bg;
void CloseMap5(SDL_Renderer *ren);
void LoadMap5(SDL_Renderer *ren);
void RunMap5(SDL_Renderer *ren, bool &RunGame);
#endif // MAP5_H
