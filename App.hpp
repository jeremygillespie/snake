#ifndef APP_HPP
#define APP_HPP

#include <iostream>

#include <SDL2/SDL.h>

class App {
public:
    App();

    int OnExecute();

    bool OnInit();

    void OnEvent(SDL_Event *Event);

    void OnLoop();

    void OnRender();

    void OnCleanup();

private:
    bool running;

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *snakeTexture;
    SDL_Texture *appleTexture;
};

App::App() :
        running{true},
        window{NULL},
        renderer{NULL},
        snakeTexture{NULL},
        appleTexture{NULL} {}

int App::OnExecute() {
    if (OnInit() == false) {
        return -1;
    }

    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            OnEvent(&event);
        }
        OnLoop();
        OnRender();
    }

    OnCleanup();

    return 0;
}

bool App::OnInit() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, 900, 600,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        return false;
    }

    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        return false;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

    SDL_Surface *surface = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);

    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 200, 0));
    snakeTexture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 200, 0, 0));
    appleTexture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    return true;
}

void App::OnEvent(SDL_Event *event) {
    if ((event->type == SDL_QUIT)) {
        running = false;
    }
}

void App::OnLoop() {}

void App::OnRender() {
    SDL_RenderClear(renderer);

    SDL_Rect src{0, 0, 1, 1};

    SDL_Rect dst;
    dst = {200, 20, 80, 80};
    SDL_RenderCopy(renderer, snakeTexture, &src, &dst);
    dst = {200, 120, 80, 80};
    SDL_RenderCopy(renderer, snakeTexture, &src, &dst);
    dst = {200, 220, 80, 80};
    SDL_RenderCopy(renderer, appleTexture, &src, &dst);

    SDL_RenderPresent(renderer);
}

void App::OnCleanup() {
    SDL_DestroyTexture(snakeTexture);
    snakeTexture = NULL;
    SDL_DestroyTexture(appleTexture);
    appleTexture = NULL;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = NULL;
    window = NULL;

    SDL_Quit();
}

#endif