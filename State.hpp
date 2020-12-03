#ifndef STATE_HPP
#define STATE_HPP

#include <vector>

namespace Snake {

enum class Direction { up, down, left, right };

class State {
public:
    State(int width = 8, int height = 8, int length = 4);
    State(int width, int height, int length, const std::vector<bool> &walls);

    int isOccupied(Direction dir) const;
    int isOccupied(int x, int y) const;

    bool isVisited(Direction dir) const;
    bool isVisited(int x, int y) const;

    void move(Direction dir);

    const int WIDTH, HEIGHT, SIZE;
    int head, tail, apple, length;

    int point(int x, int y) const;
    int point(int p, Direction dir) const;
    int x(int p) const;
    int y(int p) const;

    State(const State &other) = default;

private:
    std::vector<bool> visited;
    std::vector<int> occupied;
    std::vector<Direction> direction;
    std::vector<bool> walls;
};

State::State(int width, int height, int length) :
        WIDTH{width},
        HEIGHT{height},
        SIZE{width * height},
        visited(SIZE, false),
        occupied(SIZE, 0),
        direction(SIZE, Direction::up),
        walls(SIZE, false) {
    apple = 0;
    head = point((width - 1) / 2, (height - 1) / 2);
    tail = point(x(head), y(head) - length + 1);
    for (int pY = y(head); pY > y(tail); --pY) {
        occupied[point(x(head), pY)] = pY;
    }
}

int State::isOccupied(Direction dir) const {
    int p = point(head, dir);
    if (p == -1) {
        return false;
    }
    if (walls[p]) {
        return false;
    }
    return occupied[p];
}

int State::isOccupied(int x, int y) const {
    int p = point(x, y);
    if (p == -1) {
        return false;
    }
    if (walls[p]) {
        return false;
    }
    return occupied[p];
}

bool State::isVisited(Direction dir) const {
    int p = point(head, dir);
    if (p == -1) {
        return false;
    }
    if (walls[p]) {
        return false;
    }
    return visited[p];
}

bool State::isVisited(int x, int y) const {
    int p = point(x, y);
    if (p == -1) {
        return false;
    }
    if (walls[p]) {
        return false;
    }
    return visited[p];
}

void State::move(Direction dir) {
    head = point(head, dir);
    if (head != -1) {
        occupied[head] = 1;
    }
}

int State::point(int x, int y) const { return (x * HEIGHT + y) % SIZE; }

int State::point(int p, Direction dir) const {
    switch (dir) {
    case Direction::up:
        if (y(p) >= HEIGHT - 1) {
            return -1;
        }
        return point(x(p), y(p) + 1);
    case Direction::down:
        if (y(p) <= 0) {
            return -1;
        }
        return point(x(p), y(p) - 1);
    case Direction::right:
        if (x(p) >= WIDTH - 1) {
            return -1;
        }
        return point(x(p) + 1, y(p));
    default:
        if (x(p) <= 0) {
            return -1;
        }
        return point(x(p) - 1, y(p));
    }
}

int State::x(int p) const { return (p % SIZE) / HEIGHT; }

int State::y(int p) const { return p % HEIGHT; }

} // namespace Snake

#endif