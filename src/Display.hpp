#ifndef SNAKE_DISPLAY_HPP
#define SNAKE_DISPLAY_HPP

#include <queue>

#include <SDL.h>

#include "Engine.hpp"

namespace snake {

struct Textures {
    SDL_Texture *snake, *apple, *board, *button;
};

struct Layout {
    int vert_padding, board_padding, vert_size;
    SDL_Rect board;
};

struct Stats {
private:
    static constexpr int target_fps = 60;

public:
    const int max_frame_dur = 1000 / target_fps + 1;
    const int update_interval = 1000;

    unsigned last_frame_time;
    unsigned last_update_time;
    float move_interval = 0.5f;
    float accumulator = 0.0f;

    int frames_per_10s, moves_per_10s;
    int moves = 0, apples = 0;
};

enum class State
{
    wall,
    play,
    end,
    quit
};

class Display {
public:
    Display(Engine *engine) : engine{engine}, graph{engine->graph} {}

    int execute() {
        if (initialize() == -1)
            return -1;

        SDL_Event event;

        while (state != State::quit) {
            while (SDL_PollEvent(&event)) {
                on_event(&event);
            }
            update();
        }

        terminate();
        return 0;
    }

private:
    Engine *engine;
    Graph *graph;

    SDL_Window *window;
    SDL_Renderer *renderer;

    Textures textures;
    Layout layout;
    Stats stats;
    State state;

    int initialize();

    void init_wall();
    void init_play();

    void update();
    void update_play();
    void render();

    void on_event(SDL_Event *event);
    void on_click(int x, int y);
    void on_dir(Direction dir);
    void on_resize(int width, int height);

    void terminate();
};

} // namespace snake

#endif