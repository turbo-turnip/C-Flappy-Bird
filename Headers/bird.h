#include <stdbool.h>
#ifndef BIRD_HEADER
#define BIRD_HEADER
#include <SDL2/SDL.h>

typedef struct bird {
    SDL_Rect Rect;
    int width;
    int height;
    int x;
    int y;
    double velocityY;
    SDL_Texture *texture;
} Bird;

#endif