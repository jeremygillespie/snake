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
    SDL_Surface *windowSurface;
    SDL_Surface *snakeSurface;
    SDL_Surface *appleSurface;
    SDL_Rect square;
};

App::App() : running{true}, windowSurface{NULL} {}

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

    window =
        SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, 900, 600, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        return false;
    }

    windowSurface = SDL_GetWindowSurface(window);

    snakeSurface = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);
    SDL_FillRect(snakeSurface, NULL,
                 SDL_MapRGB(snakeSurface->format, 0, 255, 0));

    appleSurface = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);
    SDL_FillRect(appleSurface, NULL,
                 SDL_MapRGB(snakeSurface->format, 255, 0, 0));

    square = SDL_Rect{0, 0, 32, 32};

    return true;
}

void App::OnEvent(SDL_Event *event) {
    if ((event->type == SDL_QUIT)) {
        running = false;
    }
}

void App::OnLoop() {}

void App::OnRender() {
    SDL_Rect snake{200, 200, 0, 0};
    SDL_Rect apple{400, 400, 0, 0};

    SDL_BlitSurface(snakeSurface, &square, windowSurface, &snake);
    SDL_BlitSurface(appleSurface, &square, windowSurface, &apple);

    SDL_UpdateWindowSurface(window);
}

void App::OnCleanup() {
    SDL_FreeSurface(windowSurface);
    windowSurface = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}

#endif