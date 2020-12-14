#ifndef SNAKE_APP_HPP
#define SNAKE_APP_HPP

#include <iostream>

#include <SDL2/SDL.h>

namespace snake {

class app {
public:
    app() {
    }

    int execute() {
        std::cout << "hello world\n";
        return 0;
    }
};

} // namespace snake

#endif