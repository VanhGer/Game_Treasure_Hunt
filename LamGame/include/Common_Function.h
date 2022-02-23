#ifndef COMMON_FUNCTION_H
#define COMMON_FUNCTION_H

#include <iostream>
#include <chrono>
#include <random>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <Windows.h>
#include <string.h>
const int SCREEN_WIDTH = 1120;
const int SCREEN_HEIGHT = 630;
const std::string WINDOW_TITLE = "Hunting Treasure";
const int MainAnimation = 6;
const int SkeletonAnimation = 9;
const int ArrowAnimation = 4;
const int MainSpeed = 7;
const int SkeletonSpeed = 3;
const int ArrowSpeed = 10;
const int CharacterH = 60;
const int CharacterW = 40;
static SDL_Surface *WindowSurface = NULL;
static SDL_Window *window = NULL;
static SDL_Renderer* GRenderer = NULL;
void logSDLError(std::ostream& os, const std::string &msg, bool fatal);
#endif // COMMON_FUNCTION_H
