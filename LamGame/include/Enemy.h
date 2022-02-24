#ifndef ENEMY_H
#define ENEMY_H

#include "Common_Function.h"
#include "LoadObject.h"
#include "Character.h"

class Enemy: public Character
{
    public:
        Enemy();
        ~Enemy();
        void clean();
        void loadCharacter(SDL_Renderer *ren, std::string path);
    protected:

};

#endif // Enemy_H
