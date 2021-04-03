#ifndef PIPE_HEADER
#define PIPE_HEADER
#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct pipe {
    SDL_Rect top;
    SDL_Rect bottom;
    int gap;
    int width;
    double speedX;
    SDL_Texture* textureTop;
    SDL_Texture* textureBottom;
    SDL_Surface* surfaceTop;
    SDL_Surface* surfaceBottom;
} Pipe;

// collision detection function
bool collides(Pipe p, Bird player) {
    if (player.x < p.top.x + p.top.w && player.x + player.width > p.top.x && player.y < p.top.y + p.top.h && player.y + player.height > p.top.y) {
        return true;
    } else if (player.x < p.bottom.x + p.bottom.w && player.x + player.width > p.bottom.x && player.y < p.bottom.y + p.bottom.h && player.y + player.height > p.bottom.y) {
        return true;
    } else {
        return false;
    }
}

#endif