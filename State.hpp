#ifndef STATE_HPP
#define STATE_HPP

#include <vector>

namespace Snake {

enum class Turn { LEFT, RIGHT, NONE };

class Direction {
public:
    static constexpr int NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3;

private:
    int val;

public:
    constexpr Direction(int value) : val{value % 4} {}

    int x() { return val % 2 == 0 ? 0 : (val == 0 ? 1 : -1); }

    int y() { return val % 2 == 0 ? (val == 1 ? 1 : -1) : 0; }

    int operator+(Turn turn) const {
        switch (turn) {
        case Turn::LEFT:
            return (val + 3) % 4;
        case Turn::RIGHT:
            return (val + 1) % 4;
        default:
            return val;
        }
    }
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
    int tail = point(x(head), y(head) - length + 1);
    for (int pY = y(head); pY > y(tail); --pY) {
        occupied[point(x(head), pY)] = pY;
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
} // namespace Snake

void State::Move(Direction dir) {
    head = point(head, dir);
    if (head != -1) {
        occupied[head] = 1;
    }
}

int State::point(int x, int y) const { return (x * HEIGHT + y) % SIZE; }

int State::point(int p, Direction dir) const {
    if (p < 0) {
        return -1;
    }
    int newX = x(p) + dir.x();
    int newY = y(p) + dir.y();

    if (newX <= 0 || newX >= WIDTH - 1 || newY <= 0 || newY >= HEIGHT - 1)
        return -1;

    Direction newDir{dir};

    newDir = Direction::EAST;

    newDir = newDir + Turn::LEFT;

    return point(newX, newY);
}

int State::x(int p) const { return (p % SIZE) / HEIGHT; }

int State::y(int p) const { return p % HEIGHT; }

} // namespace Snake

#endif