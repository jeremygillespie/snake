#ifndef SNAKE_DISPLAY_HPP
#define SNAKE_DISPLAY_HPP

#include <algorithm>
#include <deque>

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

class Stats {
private:
    const int target_fps = 60;

public:
    const int max_frame_dur = 1000 / target_fps + 1;
    const int update_interval = 500;
    const int moving_average_n = 4;

    unsigned last_frame_ticks;
    unsigned last_update_ticks;

    float move_interval = 0.5f;
    float accumulator = 0.0f;

    std::deque<int> frame_counts;
    std::deque<int> move_counts;

    int moves = 0, apples = 0;

    Stats() {
        frame_counts.push_back(0);
        move_counts.push_back(0);
    }

    void update() {
        move_counts.push_back(0);
        frame_counts.push_back(0);

        if (move_counts.size() > moving_average_n) {
            move_counts.pop_front();
            frame_counts.pop_front();
        }
    }

    int frames_per_second(int decimals = 0) {
        if (frame_counts.empty())
            return 0;

        int numerator = 0, denominator = 0;

        for (auto it = frame_counts.begin(); it != frame_counts.end(); ++it) {
            numerator += *it;
            denominator += 1;
        }

        for (int i = 0; i < decimals; ++i)
            numerator *= 10;

        numerator *= 1000;
        denominator *= update_interval;

        return numerator / denominator;
    }

    int moves_per_second(int decimals = 0) {
        if (move_counts.empty())
            return 0;

        int numerator = 0, denominator = 0;

        for (auto it = move_counts.begin(); it != move_counts.end(); ++it) {
            numerator += *it;
            denominator += 1;
        }

        for (int i = 0; i < decimals; ++i)
            numerator *= 10;

        numerator *= 1000;
        denominator *= update_interval;

        return numerator / denominator;
    }

    int moves_per_apple(int decimals = 0) {
        int numerator = moves;
        int denominator = apples;

        for (int i = 0; i < decimals; ++i)
            numerator *= 10;

        return numerator / denominator;
    }
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

    void start_wall();
    void start_play();
    void end_play();

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