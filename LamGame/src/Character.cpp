#include "Character.h"
#include "iostream"
Character::Character()
{
    Object();
    for (int i = 0; i < 4; i++){
        Rdir[i].h = 60; Rdir[i].w = 40;
        Rdir[i].x = 0; Rdir[i].y = 0;
        NumAnimation[i] = 0;
    }
    id = 0;
}
Character::~Character()
{
    for (int i = 0; i < 4; i++){
        Rdir[i].h = 0; Rdir[i].w = 0;
        Rdir[i].x = 0; Rdir[i].y = 0;
        NumAnimation[i] = 0;
        Dir[i].free();
    }
    id = 0;
}
void Character:: setId(int val){
    id = val;
}
int Character:: getId(){
    return id;
}
int Character:: getNum(){
    return NumAnimation[id];
}
void Character:: changeNumAnimation(int Num){
    NumAnimation[id]++;
    NumAnimation[id] %= Num;
}
void Character:: goUp(int Lim, int Num){
    id = 0;
    changeNumAnimation(Num);
    ty = std::max(Lim, ty - Speed);
}
void Character:: goDown(int Lim, int Num){
    id = 1;
    changeNumAnimation(Num);
    ty = std::min(Lim, ty +  Speed);
}
void Character:: goLeft(int Lim, int Num){
    id = 2;
    changeNumAnimation(Num);
    tx = std::max(Lim, tx -  Speed);
}
void Character:: goRight(int Lim, int Num){
    id = 3;
    changeNumAnimation(Num);
    tx = std::min(Lim, tx +  Speed);
}

void Character:: display(SDL_Renderer *ren){
    Rdir[id].x = 64*NumAnimation[id];
    Dir[id].render(tx, ty, ren, &Rdir[id]);
}
