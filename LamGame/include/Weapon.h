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
        int getVx();
        int getVy();
        void setVx(int val);
        void setVy(int val);
        void setNum(int val);
    protected:
        LoadObject Arr;
        SDL_Rect RArr;
        int num;
        int Vx, Vy;

};

#endif // WEAPON_H
