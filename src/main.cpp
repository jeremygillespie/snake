#include <chrono>
#include <iostream>
#include <string>

#include "AStar.hpp"
#include "Display.hpp"

using namespace snake;

int main(int argc, char *argv[]) {

    // initialize graph
    int width = 8, height = 8, x = 3, y = 3, length = 4;
    if (argc > 2) {
        width = std::stoi(argv[1]);
        height = std::stoi(argv[2]);
    }
    if (argc > 4) {
        x = std::stoi(argv[3]);
        y = std::stoi(argv[4]);
    }
    if (argc > 5) {
        length = std::stoi(argv[5]);
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    Graph::random_engine random_engine{seed};
    Graph graph = {width, height, x, y, length, &random_engine};

    // initialize engine
    Engine *engine = NULL;

    if (argc > 1) {
        std::string e = argv[argc - 1];
        if (e.compare("manhattan") == 0) {
            engine = new Manhattan{&graph};
        } else if (e.compare("astar") == 0) {
            engine = new AStar{&graph};
        } else if (e.compare("human") == 0) {
            engine = new Human{&graph};
        }
    }

    using default_engine = Human;
    if (engine == NULL)
        engine = new default_engine{&graph};

    // initialize display
    int target_fps = 60;

    // execute
    Display display = {engine, target_fps};
    return display.execute();
}