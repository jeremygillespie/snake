#ifndef SNAKE_ASTAR_HPP
#define SNAKE_ASTAR_HPP

// DEBUG
#include <iostream>

#include <queue>
#include <tuple>

#include "Engine.hpp"

namespace snake {

class Node {
public:
    using it_t = std::vector<Node>::iterator;

    Node(
    int position, Direction direction, std::vector<Node> &tree,
    int graph_size) :
      position{position},
      time{0},
      cost{0},
      direction{direction},
      parent{tree.begin()},
      visited(graph_size, false) {
        visited[position] = true;
    }

    Node(int position, int time, int cost, Direction direction, it_t parent) :
      position{position},
      time{time},
      cost{cost},
      direction{direction},
      parent{parent},
      visited{parent->visited} {
        visited[position] = true;
    }

    int position, time, cost;
    Direction direction;
    it_t parent;
    std::vector<bool> visited;
};

class Node_comp {
public:
    using pair_t = std::tuple<Node::it_t, int>;

    static constexpr int priority_el = 1;
    static constexpr int it_el = 0;

    bool operator()(const pair_t &left, const pair_t &right) {
        return std::get<priority_el>(left) > std::get<priority_el>(right);
    }
};

class AStar : public Engine {
public:
    AStar(Graph *graph) : Engine{graph}, manhattan{graph} {}

    void update() {
        bool tail_nearby = false;
        for (int d1 = 0; !tail_nearby && d1 < 3; ++d1) {
            int p1 = graph->point(graph->head, {d1});

            if (p1 != -1 && graph->occupied[p1] == 1) {
                tail_nearby = true;
            }

            for (int d2 = 0; !tail_nearby && d2 < 3; ++d2) {
                int p2 = graph->point(p1, {d2});
                if (p2 != -1 && graph->occupied[p2] == 1) {
                    tail_nearby = true;
                }
            }
        }

        if (tail_nearby || path.empty()) {
            if (!search()) {
                no_path();
            }
        }
        move = path.top();
        path.pop();
    }

private:
    Manhattan manhattan;

    void no_path() {
        manhattan.update();
        move = manhattan.move;
    }

    static constexpr int max_nodes = 100000;

    std::vector<Node> tree;

    std::priority_queue<
    Node_comp::pair_t, std::vector<Node_comp::pair_t>, Node_comp>
    q;

    bool search() {
        tree = {};
        tree.reserve(max_nodes);
        tree.push_back(
        Node{graph->head, graph->directions[graph->head], tree, graph->size});

        q = {};
        q.push({tree.begin(), 0});

        Node::it_t n;

        do {
            n = std::get<Node_comp::it_el>(q.top());
            q.pop();

            for (int d = 0; d < 4; ++d) {
                Direction dir{d};
                int pos = graph->point(n->position, dir);
                int time = n->time + 1;

                if (pos == -1)
                    continue;

                if (graph->walls[pos])
                    continue;

                if (graph->occupied[pos] > time)
                    continue;

                if (!safe(dir, pos))
                    continue;

                if (n->visited[pos])
                    continue;

                int cost = n->cost + 100;

                if (dir != n->direction)
                    ++cost;

                int priority = cost + graph->distance(pos, graph->apple);

                tree.push_back(Node{pos, time, cost, dir, n});
                q.push({--tree.end(), priority});

                if (tree.size() == tree.capacity()) {
                    std::cout << "insufficient memory\n";
                    return false;
                }
            }

        } while (n->position != graph->apple && !q.empty());

        if (n->position == graph->apple) {
            do {
                path.push(n->direction);
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
};

} // namespace snake

#endif