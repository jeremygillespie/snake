#include <iostream>
#include <string>

#include "Display.hpp"
#include "Engine.hpp"

using namespace snake;

int main(int argc, char *argv[]) {

    int width = 8, height = 8, x = 3, y = 3, length = 4;

    if (argc > 3) {
        width = std::stoi(argv[2]);
        height = std::stoi(argv[3]);
    }

    Graph graph = {width, height, x, y, length};
    Engine *engine;

    if (argc > 1 && std::strcmp(argv[1], "manhattan") == 0) {
    } else {
        engine = new Engine{graph};
    }

    Display display = {engine};

    return display.execute();
}

namespace snake {

int Display::initialize() {
    return -1;
}

void Display::update_wall() {}

void Display::update_play() {}

void Display::render() {}

void Display::terminate() {}

} // namespace snake