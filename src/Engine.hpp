#ifndef SNAKE_ENGINE_HPP
#define SNAKE_ENGINE_HPP

#include "Graph.hpp"

namespace snake {

class Engine {
public:
    Engine(const Graph &graph) : graph{graph} {}

    Graph current_graph() const {
        return graph;
    }

    void set_wall(int x, int y);

    virtual void nextMove(Direction dir) {}

private:
    Graph graph;
};

} // namespace snake

#endif