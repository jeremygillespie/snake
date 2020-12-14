#ifndef SNAKE_GRAPH_HPP
#define SNAKE_GRAPH_HPP

namespace snake {

class graph {
public:
    graph();

    int x, y, lenght;

    int size() {
        return x * y;
    }
};

} // namespace snake

#endif