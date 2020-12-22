#ifndef SNAKE_ASTAR_HPP
#define SNAKE_ASTAR_HPP

#include <queue>
#include <tuple>

#include "Engine.hpp"

namespace snake {

struct Cost {
    static constexpr int distance = 110;
    static constexpr int move = 100;
    static constexpr int turn = 1;
    static constexpr int empty_adjacent = 1;
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

class Manhattan_AStar : public Engine {
public:
    Manhattan_AStar(Graph *graph, bool show_search) :
      Engine(graph, show_search),
      fallback{graph} {}

    bool update() {

        // continue current search
        if (!move_found) {
            move_found = update_search();
            return false;
        }

        // start new search
        if (result_path.empty()) {
            init_search();
            move_found = update_search();
            return false;
        }

        // follow old search
        search_path = {};
        move = result_path.front();
        result_path.pop_front();
        return true;
    }

private:
    static constexpr int max_nodes = 100000;

    using fallback_engine = Manhattan;
    fallback_engine fallback;

    bool move_found = true;

    std::deque<Direction> result_path;

    std::vector<Node> search_tree;

    std::priority_queue<
    Node_comp::pair_t, std::vector<Node_comp::pair_t>, Node_comp>
    search_queue;

    Node::it_t n;

    void init_search() {
        search_tree = {};
        search_tree.reserve(max_nodes);
        search_tree.push_back(Node{
        graph->head, graph->directions[graph->head], search_tree, graph->size});

        search_queue = {};
        search_queue.push({search_tree.begin(), 0});

        update_search();
    }

    bool update_search() {
        n = std::get<Node_comp::it_el>(search_queue.top());
        search_queue.pop();

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

            int distance = graph->distance(pos, graph->apple);

            int cost = n->cost;

            if (distance > 2)
                cost += Cost::move;

            if (dir != n->direction)
                cost += Cost::turn;

            for (int d2 = 0; d2 < 4; ++d2) {
                int p = graph->point(n->position, {d2});
                if (graph->occupied[p] == 0)
                    cost += Cost::empty_adjacent;
            }

            int heuristic = distance * Cost::distance;

            int priority = cost + heuristic;

            search_tree.push_back(Node{pos, time, cost, dir, n});
            search_queue.push({--search_tree.end(), priority});

            if (search_tree.size() == search_tree.capacity()) {
                fallback.update();
                move = fallback.move;
                return true;
            }
        }

        if (show_search) {
            update_search_path();
        }

        if (n->position == graph->apple) {
            update_result_path();
            return true;
        }

        if (search_queue.empty()) {
            fallback.update();
            move = fallback.move;
            return true;
        }

        return false;
    }

    void update_result_path() {
        while (n->parent != n) {
            result_path.push_front(n->direction);
            n = n->parent;
        }
    }

    void update_search_path() {
        Node::it_t current = n;
        search_path = {};
        while (current->parent != current) {
            search_path.push_front(current->direction);
            current = current->parent;
        }
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