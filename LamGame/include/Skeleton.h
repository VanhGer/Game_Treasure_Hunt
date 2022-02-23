#ifndef SKELETON_H
#define SKELETON_H

#include "Common_Function.h"
#include "LoadObject.h"
#include "Character.h"

class Skeleton: public Character
{
    public:
        Skeleton();
        ~Skeleton();
        void clean();
        void loadCharacter(SDL_Renderer *ren);
    protected:

};

#endif // SKELETON_H
