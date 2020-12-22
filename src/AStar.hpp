#ifndef SNAKE_ASTAR_HPP
#define SNAKE_ASTAR_HPP

#include <queue>
#include <tuple>

#include "Engine.hpp"

namespace snake {

struct Cost {
    static constexpr int move = 100;
    static constexpr int turn = 1;
};

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

    bool update() {

        // continue current search
        if (!path_found) {
            path_found = update_search();
            return false;
        }

        // start new search
        if (path.empty()) {
            init_search();
            path_found = update_search();
            return false;
        }

        // follow old search
        search_path = {};
        move = path.front();
        path.pop_front();
        return true;
    }

private:
    Manhattan manhattan;

    static constexpr int max_nodes = 1000000;

    bool path_found = true;

    std::deque<Direction> path;

    std::vector<Node> tree;

    std::priority_queue<
    Node_comp::pair_t, std::vector<Node_comp::pair_t>, Node_comp>
    q;

    Node::it_t n;

    void init_search() {
        tree = {};
        tree.reserve(max_nodes);
        tree.push_back(
        Node{graph->head, graph->directions[graph->head], tree, graph->size});

        q = {};
        q.push({tree.begin(), 0});

        // update_search();
    }

    bool update_search() {
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

            int cost = n->cost + Cost::move;

            if (dir != n->direction)
                cost += Cost::turn;

            int heuristic = graph->distance(pos, graph->apple) * Cost::move;

            int priority = cost + heuristic;

            tree.push_back(Node{pos, time, cost, dir, n});
            q.push({--tree.end(), priority});

            if (tree.size() == tree.capacity()) {
                manhattan.update();
                move = manhattan.move;
                return true;
            }
        }

        Node::it_t current = n;
        search_path = {};
        while (current->parent != current) {
            search_path.push_front(current->direction);
            current = current->parent;
        }

        // apple found
        if (n->position == graph->apple) {
            while (n->parent != n) {
                path.push_front(n->direction);
                n = n->parent;
            }
            return true;
        }

        if (q.empty()) {
            manhattan.update();
            move = manhattan.move;
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