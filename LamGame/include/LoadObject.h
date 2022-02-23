#ifndef LOADOBJECT_H
#define LOADOBJECT_H

#include "Common_Function.h"
class LoadObject
{
public:
    LoadObject();
    ~LoadObject();
    bool loadFromFile(std::string path, SDL_Renderer *ren);
    void free();
    void render(int x, int y, SDL_Renderer* ren, SDL_Rect *clip = NULL);
    int getHeight();
    int getWidth();

protected:
    SDL_Texture* OTexture;
    int ObjW, ObjH;
};

#endif // LOADOBJECT_H
