#ifndef WEAPON_H
#define WEAPON_H

#include<Common_Function.h>
#include<LoadObject.h>
#include <Object.h>
class Weapon : public Object
{
    public:
        Weapon();
        ~Weapon();
        void clean();
        void display(SDL_Renderer *ren);
        void loadWeapon(SDL_Renderer *ren);
    protected:
        LoadObject Arr;
        SDL_Rect RArr;
        int num;

};

#endif // WEAPON_H
