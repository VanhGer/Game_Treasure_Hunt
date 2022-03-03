#ifndef LBUTTON_H
#define LBUTTON_H
#include <Common_Function.h>
#include <SDL.h>
#include <SDL_image.h>
#include <LoadObject.h>
#include <stdio.h>
#include <string>
enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;
SDL_Rect gSpriteClips[ BUTTON_SPRITE_TOTAL ];
LoadObject gButtonSpriteSheetTexture;

//Buttons objects

class LButton
{
	public:
		//Initializes internal variables
		LButton();
        ~LButton();
		//Sets top left position
		void setPosition( int x, int y );

		//Handles mouse event
		void handleEvent( SDL_Event* e );

		//Shows button sprite
		void render();

	protected:
		//Top left position
		SDL_Point mPosition;

		//Currently used global sprite
		LButtonSprite mCurrentSprite;
};


#endif // LBUTTON_H
