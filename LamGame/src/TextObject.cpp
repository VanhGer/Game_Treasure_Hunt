#include "TextObject.h"
TextObject::TextObject()
{
    text_color.r = 255;
    text_color.g = 255;
    text_color.b = 255;
    texture = NULL;
}

TextObject::~TextObject()
{
    Free();
}
bool TextObject:: LoadFromRenderText(TTF_Font *font, SDL_Renderer *ren){
    SDL_Surface * text_surface = TTF_RenderText_Solid(font, str_val.c_str(), text_color);
    if (text_surface == NULL){
        std::cout << "Cannot load TextObject \n";
        return 0;
    }
    texture = SDL_CreateTextureFromSurface(ren, text_surface);
    width = text_surface->w;
    height = text_surface->h;
    SDL_FreeSurface(text_surface);
    return 1;
}
void TextObject::Free(){
    if (texture != NULL){
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
}
void TextObject::SetColor(Uint8 red, Uint8 green, Uint8 blue){
    text_color.r = red;
    text_color.g = green;
    text_color.b = blue;
}
void TextObject::SetColor(int type){
    SDL_Color color;
    if (type == RED_TEXT){
        color = {255, 0, 0};
    }
    else if (type == WHITE_TEXT){
        color = {255, 255, 255};
    }
    else if (type == BLACK_TEXT){
        color = {0, 0, 0};
    }
    else {
        std::cout << "Cannot initialize color!"; return;
    }
    text_color = color;
}
void TextObject:: RenderText(SDL_Renderer *ren,
                        int tx, int ty,
                        SDL_Rect* clip,
                        double angle ,
                        SDL_Point *center,
                        SDL_RendererFlip flip){
    SDL_Rect RenderQuad = {tx, ty, width, height};
    if (clip != NULL){
        RenderQuad.w = clip->w;
        RenderQuad.h = clip->h;
    }
    SDL_RenderCopyEx(ren, texture, clip, &RenderQuad, angle, center, flip);
}
