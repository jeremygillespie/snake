#include <chrono>
#include <iostream>
#include <string>

#include "AStar.hpp"
#include "Display.hpp"

using namespace snake;

enum class E_type
{
    human,
    manhattan,
    reversal,
    astar
};

int main(int argc, char *argv[]) {

    int width = 8, height = 8, x = 0, y = 3, length = 4;
    E_type e = E_type::human;
    bool show_search = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg0{argv[i]};

        if (arg0.compare("-s") == 0) {
            show_search = true;
        }

        if (arg0.compare("--manhattan") == 0) {
            e = E_type::manhattan;
        }

        if (arg0.compare("--reversal") == 0) {
            e = E_type::reversal;
        }

        if (arg0.compare("--manastar") == 0) {
            e = E_type::astar;
        }
    }

    for (int i = 1; i < argc - 1; ++i) {
        std::string arg0{argv[i]};
        std::string arg1{argv[i + 1]};

        // start length
        if (arg0.compare("-l") == 0) {
            length = std::stoi(arg1);
        }
    }

    for (int i = 1; i < argc - 2; ++i) {
        std::string arg0{argv[i]};
        std::string arg1{argv[i + 1]};
        std::string arg2{argv[i + 2]};

        // dimensions
        if (arg0.compare("-d") == 0) {
            width = std::stoi(arg1);
            height = std::stoi(arg2);
        }

        // start location
        if (arg0.compare("-p") == 0) {
            x = std::stoi(arg1);
            y = std::stoi(arg2);
        }
    }

    // init graph
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    Graph::random_engine random_engine{seed};
    Graph graph = {width, height, x, y, length, &random_engine};

    // init engine
    Engine *engine = NULL;
    switch (e) {
    case E_type::manhattan:
        engine = new Manhattan{&graph};
        break;
    case E_type::reversal:
        engine = new Reversal{&graph, show_search};
        break;
    case E_type::astar:
        engine = new Manhattan_AStar{&graph, show_search};
        break;
    default:
        engine = new Human{&graph};
        break;
    }

    // init display
    Display display = {engine};

    // execute
    return display.execute();
}