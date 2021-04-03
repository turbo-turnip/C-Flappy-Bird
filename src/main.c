#include <time.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2_TTF/SDL_TTF.h>
#include "../Headers/window.h"
#include "./bird.c"
#include "../Headers/backgroundchunk.h"
#include "../Headers/pipe.h"

#define GRAVITY 0.6

int addscore(int* score) { 
    (*score)++; 
    printf("%d\n", *score);
    return *score;
}

int main(int argc, char* argv[]) {

    time_t t;
    srand((unsigned) time(&t));

    int iter = 0;
    int score = 0;
    char scoreText[10];
    bool alreadyCheckedScores[1000] = {};

    SDL_Init(SDL_INIT_AUDIO);
    SDL_Window *window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetWindowTitle(window, "Flappy Bird");
    SDL_SetRenderDrawColor(renderer, 100, 208, 232, 255);

    // SDL_ttf font setup
    TTF_Init();
    TTF_Font* Arial = TTF_OpenFont("./Fonts/Ceriph0765.ttf", 16);
    SDL_Color fontBlack = {0, 0, 0};

    init(renderer);

    BackgroundChunk bgChunks[500] = {};
    Pipe pipes[1000] = {};
    size_t pipesSize = 0;

    for (int i = 0; i < 500; ++i) {
        BackgroundChunk* chunk = &bgChunks[i];
        chunk->x = i*WINDOW_WIDTH;
        chunk->y = WINDOW_HEIGHT-300;
        chunk->speedX = 1;
        chunk->width = WINDOW_WIDTH;
        chunk->height = 300;
        chunk->textureSrc = "./assets/background.png";
        chunk->texture = IMG_LoadTexture(renderer, chunk->textureSrc);
        chunk->rect.x = chunk->x;
        chunk->rect.y = chunk->y;
        chunk->rect.w = chunk->width;
        chunk->rect.h = chunk->height;
        chunk->alpha = 180;

        SDL_SetTextureBlendMode(chunk->texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(chunk->texture, chunk->alpha);
    }

    SDL_Rect demoRect = {WINDOW_WIDTH/2-225, WINDOW_HEIGHT/2-135, 450, 270};
    SDL_Texture* demoTexture = IMG_LoadTexture(renderer, "./assets/demo.png");

    bool running = true;
    bool startFalling = false;
    bool dead = false;
    SDL_Event e;
    while (running) {

        iter++;

        sprintf(scoreText, "%d", score);

        SDL_Surface* scoreSurface = TTF_RenderText_Solid(Arial, scoreText, fontBlack);
        SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface); 
        SDL_Rect scoreRect = {WINDOW_WIDTH/2-25, 10, 25, 50};

        if (iter % 50 == 0) {
            pipesSize++;
            Pipe *pipe = &pipes[pipesSize-1];

            int heightTop = rand() % (int) ((WINDOW_HEIGHT/2)*1.2 - WINDOW_HEIGHT/10 + 1) + WINDOW_HEIGHT/10;

            // asset handling
            pipe->surfaceTop = IMG_Load("./assets/pipe-down.png");
            pipe->surfaceBottom = IMG_Load("./assets/pipe-up.png");
            pipe->textureTop = SDL_CreateTextureFromSurface(renderer, pipe->surfaceTop);
            pipe->textureBottom = SDL_CreateTextureFromSurface(renderer, pipe->surfaceBottom);
            SDL_FreeSurface(pipe->surfaceTop);
            SDL_FreeSurface(pipe->surfaceBottom);

            pipe->gap = 200;
            pipe->width = 60;
            pipe->speedX = 2;

            pipe->top.x = pipesSize * 100 + 500;
            pipe->top.y = 0;
            pipe->top.w = pipe->width;
            pipe->top.h = heightTop;

            pipe->bottom.x = pipesSize * 100 + 500;
            pipe->bottom.y = heightTop+pipe->gap;
            pipe->bottom.w = pipe->width;
            pipe->bottom.h = WINDOW_HEIGHT-(pipe->bottom.y);
        }

        if (player.y+30 > WINDOW_HEIGHT) {
            dead = true;
        } else if (player.y < 0) {
            dead = true;
        } else if (startFalling) {
            player.velocityY += GRAVITY;
        }

        // collision detection
        for (int i = 0; i < pipesSize; ++i) {
            if (collides(pipes[i], player)) {
                for (int j = 0; j < pipesSize; ++j) {
                    pipes[j].speedX = 0;
                }
                player.velocityY = 0;
                player.y = WINDOW_HEIGHT-player.height;
                for (int j = 0; j < 500; ++j) {
                    bgChunks[j].speedX = 0;
                }
                dead = true;
            }

            if (!alreadyCheckedScores[i] == true && player.x > pipes[i].top.x) {
                alreadyCheckedScores[i] = true;
                addscore(&score);
            }
        }

        for (int i = 0; i < 500; ++i) {
            bgChunks[i].rect.x = bgChunks[i].x;
            bgChunks[i].x -= bgChunks[i].speedX;
        }

        for (int i = 0; i < pipesSize; ++i) {
            pipes[i].top.x -= pipes[i].speedX;
            pipes[i].bottom.x -= pipes[i].speedX;
        }

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_SPACE:
                        player.velocityY = -6;
                        startFalling = true;
                        break;
                    default: 
                        break;
                }
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                startFalling = true;
                player.velocityY = -6;
            } else if (e.type == SDL_QUIT) {
                running = false;
                break;
            }
        }

        player.y += (int) player.velocityY;

        SDL_SetRenderDrawColor(renderer, 100, 208, 232, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < 500; ++i) {
            SDL_RenderFillRect(renderer, &bgChunks[i].rect);
            SDL_RenderCopy(renderer, bgChunks[i].texture, NULL, &bgChunks[i].rect);
        }
        if (!dead) {
            renderBird(renderer);
        }
        SDL_RenderCopy(renderer, player.texture, NULL, &player.Rect);
        for (int i = 0; i < pipesSize; ++i) {
            SDL_Rect pipeTop = pipes[i].top;
            SDL_Rect pipeBottom = pipes[i].bottom;

            SDL_RenderCopy(renderer, pipes[i].textureTop, NULL, &pipeTop);
            SDL_RenderCopy(renderer, pipes[i].textureBottom, NULL, &pipeBottom);
        }
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

        if (!startFalling && !dead) {
            SDL_RenderCopy(renderer, demoTexture, NULL, &demoRect);
        }

        if (dead) {
            SDL_Rect gameOverRect = {WINDOW_WIDTH/2-150, WINDOW_HEIGHT/2-80, 300, 160};
            SDL_Texture* gameOverTexture = IMG_LoadTexture(renderer, "./assets/game-over.png");
            SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverRect);
        }

        SDL_SetRenderDrawColor(renderer, 18, 161, 27, 255);
        SDL_Rect ground = {0, WINDOW_HEIGHT-25, WINDOW_WIDTH, 25};
        SDL_RenderFillRect(renderer, &ground);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}