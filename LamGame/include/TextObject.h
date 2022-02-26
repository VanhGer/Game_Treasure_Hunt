#ifndef TEXTOBJECT_H
#define TEXTOBJECT_H

#include <iostream>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <SDL.h>
class TextObject
{
    public:
        TextObject();
        ~TextObject();
        enum TextColor{
            RED_TEXT = 0,
            WHITE_TEXT = 1,
            BLACK_TEXT = 2,
        };
        bool LoadFromRenderText(TTF_Font *font, SDL_Renderer *ren);
        void Free();
        void SetColor(Uint8 red, Uint8 green, Uint8 blue);
        void SetColor(int type);
        void RenderText(SDL_Renderer *ren,
                        int tx, int ty,
                        SDL_Rect* clip = NULL,
                        double angle = 0.0,
                        SDL_Point *center = NULL,
                        SDL_RendererFlip flip = SDL_FLIP_NONE);
        int GetWidth() const {return width;}
        int GetHeight() const {return height;}
        void setText(const std::string& text) {str_val = text;}
        std::string getText() const {return str_val;}
    protected:
        std::string str_val;
        SDL_Color text_color;
        SDL_Texture* texture;
        int width;
        int height;
    private:
};

#endif // TEXTOBJECT_H
