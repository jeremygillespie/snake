#ifndef SNAKE_DISPLAY_HPP
#define SNAKE_DISPLAY_HPP

#include <iostream>

#include <SDL.h>

#include "Engine.hpp"
#include "Graph.hpp"

namespace snake {

class Display {

public:
    Display(Engine *engine) {}

    int execute() {
        if (initialize() == -1)
            return -1;

        while (state == wall) {
            update_wall();
        }

        while (state == play) {
            update_play();
        }

        while (state == end) {
            render();
        }

        terminate();
        return 0;
    }

private:
    Engine *engine;

    enum State
    {
        wall,
        play,
        end,
        quit
    };

    State state = wall;

    int initialize();

    void update_wall();

    void update_play();

    void render();

    void terminate();
};

} // namespace snake

#endif