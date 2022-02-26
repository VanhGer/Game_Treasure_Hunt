#ifndef CHARACTER_H
#define CHARACTER_H
#include <SDL.h>
#include<LoadObject.h>
#include<Object.h>
class Character : public Object{
    protected:
        // 0:up, 1: down, 2: left, 3:right
        SDL_Rect Rdir[4];
        int NumFrames[4];
        int id, Hp;
        LoadObject  Dir[4];
    public:
        Character();
        ~Character();
        void setId(int val);
        int getHp();
        void setHp(int val);
        int getId();
        int getNum();
        void setNumFrames(int val);
        void changeNumFrames(int Num);
        void goUp(int Lim, int Num);
        void goDown(int Lim, int Num);
        void goLeft(int Lim, int Num);
        void goRight(int Lim, int Num);
        void display(SDL_Renderer *ren);
};

#endif // CHARACTER_H
