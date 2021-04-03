#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../Headers/bird.h"
#include "../Headers/window.h"

Bird player;

void init(SDL_Renderer *renderer) {
    player.width = 50;
    player.height = 34;
    player.x = WINDOW_WIDTH/2-15;
    player.y = WINDOW_HEIGHT/2-15;
    player.velocityY = 0;
    player.texture = IMG_LoadTexture(renderer, "./assets/flappy-bird.png");
}

void initializeBird() {
    player.Rect.x = player.x;
    player.Rect.y = player.y;
    player.Rect.w = player.width;
    player.Rect.h = player.height;
}

void renderBird(SDL_Renderer *renderer) {
    initializeBird();
    // render the bird's texture
    SDL_RenderCopy(renderer, player.texture, NULL, &player.Rect);
}