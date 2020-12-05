#ifndef APP_HPP
#define APP_HPP

#include <SDL2/SDL.h>

#include <functional>
#include <map>

#include "Engine.hpp"
#include "State.hpp"

namespace Snake {

struct Textures {
    SDL_Texture *snake, *apple, *board, *button;
};

struct Layout {
    static constexpr int vertPadding = 5;
    static constexpr int widgetPadding = 10;

    SDL_Rect board;
    int widthVerts = 20, heightVerts = 20, vertSize;
};

struct Game {
    State *state;
    Engine *engine;
    int speed;
    Direction dir;
};

class App {
private:
    enum AppState { INIT, MENU, PLAY, PAUSE, QUIT };
    enum KeyFunction {
        APPK_NORTH,
        APPK_SOUTH,
        APPK_EAST,
        APPK_WEST,
        APPK_MENU,
        APPK_FAST,
        APPK_SLOW
    };

    AppState appState;

    SDL_Window *window;
    SDL_Renderer *renderer;

    Textures textures;
    Layout layout;
    Game game;

    std::multimap<int, KeyFunction> keyMap;

    unsigned int lastMoveTime;

public:
    App() :
            appState{INIT},
            window{},
            renderer{},
            textures{},
            layout{},
            game{},
            keyMap{{SDLK_w, APPK_NORTH},
                   {SDLK_s, APPK_SOUTH},
                   {SDLK_a, APPK_WEST},
                   {SDLK_d, APPK_EAST}} {}

    int Exectute() {
        if (onInit() == false) {
            return -1;
        }

        SDL_Event event;

        while (appState != QUIT) {
            while (SDL_PollEvent(&event)) {
                onEvent(&event);
            }
            onLoop();
            onRender();
        }

        onCleanup();

        return 0;
    }

private:
    bool onInit();

    void onEvent(SDL_Event *Event);

    void onLoop();

    void onRender();

    void onResize(int width, int height);

    void onCleanup();

    void onKey(KeyFunction key);

    void onKeyDir(Direction dir), onKeyMenu(), onKeySpeed(bool faster);
}; // namespace Snake

} // namespace Snake

#endif