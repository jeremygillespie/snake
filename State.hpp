#ifndef STATE_HPP
#define STATE_HPP

#include <vector>

namespace Snake {

class Direction {
public:
    static constexpr int NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3,
                         LEFTTURN = -1, NOTURN = 0, RIGHTTURN = 1;

private:
    int val;

    static constexpr int mod4(int n) { return n < 0 ? (n % 4 + 4) % 4 : n % 4; }

public:
    constexpr Direction(int value = NORTH) : val{mod4(value)} {}

    int x() const { return val % 2 == 0 ? 0 : (val == 1 ? 1 : -1); }

    int y() const { return val % 2 == 0 ? (val == 0 ? 1 : -1) : 0; }

    Direction operator+(int turn) const { return Direction(val + turn); }

    void operator+=(int turn) { val = mod4(val + turn); }
};

class State {
public:
    State(int width = 8, int height = 8, int length = 4);
    State(int width, int height, int length, const std::vector<bool> &walls);

    bool CanMove(Direction dir) const;

    void Move(Direction dir);

    int Occupied(int x, int y) const;

    const int WIDTH, HEIGHT, SIZE;
    int head, apple, length;

    int point(int x, int y) const;
    int point(int p, Direction dir) const;
    int x(int p) const;
    int y(int p) const;

    State(const State &other) = default;

private:
    std::vector<int> occupied;
    std::vector<bool> walls;
};

State::State(int width, int height, int length) :
        WIDTH{width},
        HEIGHT{height},
        SIZE{width * height},
        occupied(SIZE, 0),
        walls(SIZE, false) {
    apple = 0;
    head = point((width - 1) / 2, (height - 1) / 2);
    for (int l = length; l > 0; --l) {
        occupied[head - length + l] = l;
    }
}

bool State::CanMove(Direction dir) const {
    int p = point(head, dir);
    if (p == -1) {
        return false;
    }
    if (walls[p]) {
        return false;
    }
    return occupied[p] <= 1;
}

int State::Occupied(int x, int y) const {
    int p = point(x, y);
    if (walls[p]) {
        return false;
    }
    return occupied[p];
}

void State::Move(Direction dir) {
    head = point(head, dir);
    if (head != -1) {
        occupied[head] = length;
    }
}

int State::point(int x, int y) const { return (x * HEIGHT + y) % SIZE; }

int State::point(int p, Direction dir) const {
    if (p < 0) {
        return -1;
    }
    int newX = x(p) + dir.x();
    int newY = y(p) + dir.y();

    if (newX < 0 || newX >= WIDTH || newY < 0 || newY >= HEIGHT)
        return -1;

    return point(newX, newY);
}

int State::x(int p) const { return (p % SIZE) / HEIGHT; }

int State::y(int p) const { return p % HEIGHT; }

} // namespace Snake

#endif