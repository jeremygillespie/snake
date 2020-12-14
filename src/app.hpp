#ifndef SNAKE_APP_HPP
#define SNAKE_APP_HPP

#include <iostream>

#include <SDL2/SDL.h>

#include "graph.hpp"

namespace snake {

class app {

public:
    graph g;

    app() {
    }

    int execute() {
        std::cout << "hello world\n";
        return 0;
    }
};

} // namespace snake

#endif