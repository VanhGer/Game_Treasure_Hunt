#include "LoadObject.h"

LoadObject::LoadObject()
{
    ObjH = 0;
    ObjW = 0;
    OTexture =NULL;
}

LoadObject::~LoadObject()
{
     free();
}
void LoadObject:: free() {
    if (OTexture != NULL){
        SDL_DestroyTexture(OTexture);
        OTexture = NULL;
        ObjH = ObjW = 0;
    }
}
int LoadObject::getHeight(){return ObjH;}
int LoadObject::getWidth(){return ObjW;}
void LoadObject :: render(int x, int y, SDL_Renderer* ren, SDL_Rect* clip) {
    //Set rendering space
    SDL_Rect renderQuad = {x, y, ObjW, ObjH};
    //set clip rendering dimension:
    if (clip != NULL) {
        renderQuad.h = clip->h;
        renderQuad.w = clip->w;
    }
    //to Screen
    SDL_RenderCopy(ren, OTexture, clip, &renderQuad);
}
bool LoadObject :: loadFromFile(std::string path, SDL_Renderer* ren) {
    free(); // clean preTexture
    SDL_Texture *newTexture = NULL;
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
       std::cout <<"Unable to load image" + path << '\n';
    }
    else {
        // set colorKey
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
        newTexture = SDL_CreateTextureFromSurface(ren, loadedSurface);
        if (newTexture == NULL)
            std::cout <<"Unable to load Texture" + path << '\n';
        else{
            ObjH = loadedSurface->h;
            ObjW = loadedSurface->w;
        }
        SDL_FreeSurface(loadedSurface);
    }
    OTexture = newTexture;
    return OTexture != NULL;
}
