#ifndef SNAKE_ASTAR_HPP
#define SNAKE_ASTAR_HPP

// DEBUG
#include <iostream>

#include <queue>
#include <stack>
#include <tuple>

#include "Engine.hpp"

namespace snake {

class Node {
public:
    using it_t = std::vector<Node>::iterator;
    using pair_t = std::tuple<it_t, int>;

    static constexpr int prio_el = 1;
    static constexpr int it_el = 0;

    int position, time, cost;
    Direction direction;
    it_t parent;
};

class Node_comp {
public:
    bool operator()(const Node::pair_t &left, const Node::pair_t &right) {
        return std::get<Node::prio_el>(left) > std::get<Node::prio_el>(right);
    }
};

class AStar : public Engine {
public:
    AStar(Graph *graph) : Engine{graph}, manhattan{graph} {}

    Direction next_move() {
        if (!search()) {
            std::cout << "search failed\n";
            no_path();
        }
        return move;
    }

private:
    Direction move;

    Manhattan manhattan;

    void no_path() {
        move = manhattan.next_move();
    }

    static constexpr int max_nodes = 100000000;

    std::vector<Node> tree;

    std::vector<bool> checked;

    std::priority_queue<Node::pair_t, std::vector<Node::pair_t>, Node_comp> q;

    bool search() {
        tree = {};
        tree.reserve(max_nodes);
        tree.push_back(
        {graph->head, 0, 0, graph->directions[graph->head], tree.begin()});

        checked = std::vector<bool>(graph->size, false);

        q = {};
        q.push({tree.begin(), 0});

        Node::it_t n;

        do {
            n = std::get<Node::it_el>(q.top());
            q.pop();

            for (int d = 0; d < 4; ++d) {
                Direction dir{d};
                int pos = graph->point(n->position, dir);
                int t = n->time + 1;

                if (pos == -1)
                    continue;

                if (checked[pos] && visited(n, pos))
                    continue;

                checked[pos] = true;

                if (graph->walls[pos])
                    continue;

                if (graph->occupied[pos] > t)
                    continue;

                if (!safe(dir, pos))
                    continue;

                int c = n->cost + 100;

                if (dir != n->direction)
                    ++c;

                int priority = c + graph->distance(pos, graph->apple);

                tree.push_back(Node{pos, t, c, dir, n});
                q.push({--tree.end(), c});
            }

            if (tree.size() + 3 > tree.capacity()) {
                std::cout << "insufficient memory\n";
                return false;
            }

        } while (n->position != graph->apple && !q.empty());

        if (n->position == graph->apple) {
            do {
                move = n->direction;
                n = n->parent;
            } while (n->parent != n);

            return true;
        }

        return false;
    }

    bool safe(Direction dir, int pos) {
        switch (dir.value()) {
        // positive y
        case Direction::north:
            // even column
            return graph->x(pos) % 2 == 0;
        // negative y
        case Direction::south:
            // odd column
            return graph->x(pos) % 2 == 1;
        // negative x
        case Direction::west:
            // even row
            return graph->y(pos) % 2 == 0;
        // positive x
        default:
            // odd row
            return graph->y(pos) % 2 == 1;
        }
    }

    bool visited(Node::it_t par, int pos) {
        Node::it_t n = par;

        while (n->parent != n) {
            if (n->position == pos)
                return true;
            n = n->parent;
        }
        return false;
    }
};

} // namespace snake

#endif