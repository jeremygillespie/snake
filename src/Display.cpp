#include <iostream>
#include <string>

#include "Display.hpp"
#include "Engine.hpp"

int main(int argc, char *argv[]) {

    int width = 8, height = 8, x = 3, y = 3, length = 4;

    if (argc > 3) {
        width = std::stoi(argv[2]);
        height = std::stoi(argv[3]);
    }

    snake::Graph graph = {width, height, x, y, length};
    snake::Engine *engine;

    if (argc > 1 && std::strcmp(argv[1], "manhattan") == 0) {
    } else {
        engine = new snake::Engine{graph};
    }

    snake::Display display = {engine};

    return display.execute();
}