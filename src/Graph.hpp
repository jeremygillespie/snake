#ifndef SNAKE_GRAPH_HPP
#define SNAKE_GRAPH_HPP

#include <algorithm>
#include <random>
#include <vector>

namespace snake {

class Direction {
private:
    int val;

public:
    static constexpr int north = 0, east = 1, south = 2, west = 3,
                         turn_left = -1, turn_none = 0, turn_right = 1,
                         turn_reverse = 2;

    Direction(int val = north) : val{val < 0 ? (val % 4 + 4) % 4 : val % 4} {}

    int x() const {
        return val % 2 == 0 ? 0 : (val == 1 ? 1 : -1);
    }
    int y() const {
        return val % 2 == 0 ? (val == 0 ? 1 : -1) : 0;
    }

    int value() const {
        return val;
    }

    bool operator==(const Direction &other) const {
        return val == other.val;
    }

    bool operator!=(const Direction &other) const {
        return val != other.val;
    }

    Direction operator+(int turn) const {
        return Direction(val + turn);
    }

    void operator+=(int turn) {
        val = Direction(val + turn).val;
    }

    Direction &operator=(const Direction &other) = default;
};

class Graph {
public:
    using random_engine = std::default_random_engine;
    random_engine *r_engine;

    const int width, height;
    int size, head, apple, length;

    std::vector<int> occupied;
    std::vector<bool> walls;
    std::vector<Direction> incoming;
    std::vector<Direction> outgoing;

    int point(int x, int y) const {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return -1;
        return (x * height + y) % size;
    }

    int point(int p, Direction dir) const {
        return point(x(p) + dir.x(), y(p) + dir.y());
    }

    int x(int p) const {
        return (p % size) / height;
    }

    int y(int p) const {
        return p % height;
    }

    int distance(int p1, int p2) const {
        return std::abs(x(p1) - x(p2)) + std::abs(y(p1) - y(p2));
    }

    bool can_move(Direction dir) const {
        int p = point(head, dir);
        if (p == -1) {
            return false;
        }
        if (walls[p]) {
            return false;
        }
        if (occupied[p] > 1) {
            return false;
        }
        return true;
    };

    void move(Direction dir) {
        outgoing[head] = dir;
        head = point(head, dir);
        if (head == -1)
            return;

        occupied[head] = length + 1;
        incoming[head] = dir;
        if (head == apple) {
            ++length;
            update_apple();
        } else {
            // decrement tail
            for (auto i = occupied.begin(); i != occupied.end(); ++i) {
                if (*i > 0)
                    --(*i);
            }
        }
    }

    void update_apple() {
        apple = -1;
        if (length < size) {
            int r = std::uniform_int_distribution<>(1, size - length)(
            *r_engine);
            for (int i = 0; i < r; ++i) {
                ++apple;
                while (occupied[apple] || walls[apple])
                    ++apple;
            }
        }
    }

    Graph(
    int width, int height, int x, int y, int length, random_engine *r_engine) :
      width{width},
      height{height},
      size{width * height},
      head{point(x, y)},
      apple{-1},
      length{length},
      occupied(size, false),
      walls(size, false),
      incoming(size, Direction{}),
      outgoing(size, Direction{}),
      r_engine{r_engine} {
        for (int i = 0; i < length; ++i) {
            int p = point(x, y - i);
            occupied[p] = length - i;
        }

        update_apple();
    }
};

} // namespace snake

#endif