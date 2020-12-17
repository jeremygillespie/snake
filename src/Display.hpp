#ifndef SNAKE_DISPLAY_HPP
#define SNAKE_DISPLAY_HPP

#include <SDL.h>

#include "Engine.hpp"
#include "Graph.hpp"

namespace snake {

struct Textures {
    SDL_Texture *snake, *apple, *board, *button;
};

struct Layout {
    int vert_padding, board_padding, vert_size;
    SDL_Rect board;
};

class Display {
public:
    Display(Engine *engine) : engine{engine}, graph{&(engine->graph)} {}

    int execute() {
        if (initialize() == -1)
            return -1;

        SDL_Event event;

        while (state != quit) {
            while (SDL_PollEvent(&event)) {
                on_event(&event);
            }
            update();
        }

        terminate();
        return 0;
    }

private:
    enum State
    {
        wall,
        play,
        end,
        quit
    };

    Engine *engine;
    Graph *graph;

    SDL_Window *window;
    SDL_Renderer *renderer;

    Textures textures;
    Layout layout;

    State state;
    Direction direction;
    unsigned last_move_time;
    unsigned move_interval = 500;

    int initialize();

    void update();
    void on_event(SDL_Event *event);

    void update_play();
    void render();

    void on_resize(int width, int height);
    void on_click();

    void terminate();
};

} // namespace snake

#endif