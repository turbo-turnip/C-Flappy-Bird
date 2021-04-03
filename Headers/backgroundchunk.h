#ifndef BACKGROUNDCHUNK_HEADER
#define BACKGROUNDCHUNK_HEADER
#include <SDL2/SDL.h>

typedef struct Background {
    int x;
    int y;
    double speedX;
    int width, height;
    char* textureSrc;
    SDL_Texture* texture;
    SDL_Rect rect;
    double alpha;
} BackgroundChunk;

#endif