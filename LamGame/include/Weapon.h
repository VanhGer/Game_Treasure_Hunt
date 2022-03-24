#ifndef WEAPON_H
#define WEAPON_H

#include<LoadObject.h>
#include <Object.h>
class Weapon : public Object
{
    public:
        Weapon();
        ~Weapon();
        void clean();
        void display(SDL_Renderer *ren);
        void loadWeapon(SDL_Renderer *ren, std::string path);
        int getVx();
        int getVy();
        void setVx(int val);
        void setVy(int val);
        void setNum(int val);
        void setAdd(int val);
        void setRObj(int x, int y, int w, int h);
        void setDamn(int val);
        int getDamn();
    protected:
        LoadObject Obj;
        SDL_Rect RObj;
        int num, add, damn;
        int Vx, Vy;

};

#endif // WEAPON_H
