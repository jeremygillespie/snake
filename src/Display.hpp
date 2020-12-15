#ifndef SNAKE_DISPLAY_HPP
#define SNAKE_DISPLAY_HPP

#include <iostream>

#include <SDL.h>

#include "Engine.hpp"
#include "Graph.hpp"

namespace snake {

class Display {

public:
    Engine *engine;

    Display(Engine *engine) {}

    int execute() {
        std::cout << "hello world\n";
        return 0;
    }
};

} // namespace snake

#endif