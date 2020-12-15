#ifndef SNAKE_ENGINE_HPP
#define SNAKE_ENGINE_HPP

#include "Graph.hpp"

namespace snake {

class Engine {
public:
    Engine(const Graph &graph) : graph{graph} {}

    Graph graph;

    virtual void initialize() {}

    virtual void nextMove(Direction dir) {}
};

} // namespace snake

#endif