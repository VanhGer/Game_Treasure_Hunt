#ifndef MAINCHARACTER_H
#define MAINCHARACTER_H

//#include "Common_Function.h"
#include "LoadObject.h"
#include "Character.h"
class MainCharacter : public Character
{
    public:
        MainCharacter();
        ~MainCharacter();
        void clean();
        void loadCharacter(SDL_Renderer *ren);
    protected:


};

#endif // MAINCHARACTER_H
