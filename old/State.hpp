#ifndef STATE_HPP
#define STATE_HPP

#include <random>
#include <vector>

namespace Snake {

class Direction {
public:
    static constexpr int NORTH = 0, EAST = 1, SOUTH = 2,
                         WEST = 3, LEFTTURN = -1,
                         NOTURN = 0, RIGHTTURN = 1,
                         UTURN = 2;

private:
    int val;

    static constexpr int mod4(int n) {
        return n < 0 ? (n % 4 + 4) % 4 : n % 4;
    }

public:
    Direction(int value = NORTH) : val{mod4(value)} {
    }

    int x() const {
        return val % 2 == 0 ? 0 : (val == 1 ? 1 : -1);
    }

    int y() const {
        return val % 2 == 0 ? (val == 0 ? 1 : -1) : 0;
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
        val = mod4(val + turn);
    }
};

class State {
public:
    State(int width = 8, int height = 8, int length = 4);

    bool CanMove(Direction dir) const;

    void Move(Direction dir);

    const int WIDTH, HEIGHT, SIZE;
    int head, apple, length;

    int point(int x, int y) const;
    int point(int p, Direction dir) const;
    int x(int p) const;
    int y(int p) const;

    void wall(int p, bool val) {
        wall_[p] = val;
    }

    const std::vector<Direction> direction() const {
        return direction_;
    }

    const std::vector<int> occupied() const {
        return occupied_;
    }

    const std::vector<bool> wall() const {
        return wall_;
    }

    State(const State &other) = default;

private:
    std::vector<Direction> direction_;
    std::vector<int> occupied_;
    std::vector<bool> wall_;
    std::default_random_engine randomEngine;
};

State::State(int width, int height, int length) :
  WIDTH{width},
  HEIGHT{height},
  SIZE{width * height},
  length{length},
  direction_(SIZE),
  occupied_(SIZE, 0),
  wall_(SIZE, false),
  randomEngine{} {
    apple = 0;
    head = point((width - 1) / 2, (height - 1) / 2);
    for (int l = length; l > 0; --l) {
        occupied_[head - length + l] = l;
    }
}

bool State::CanMove(Direction dir) const {
    int p = point(head, dir);
    if (p == -1) {
        return false;
    }
    if (wall_[p]) {
        return false;
    }
    if (occupied_[p] > 1) {
        return false;
    }
    return true;
}

void State::Move(Direction dir) {
    direction_[head] = dir;
    head = point(head, dir);
    if (head != -1) {
        occupied_[head] = length + 1;
        direction_[head] = dir;
        if (head == apple) {
            ++length;
            apple = -1;
            if (length < SIZE) {
                int r = std::uniform_int_distribution<>(
                    1, SIZE - length)(randomEngine);
                for (int i = 0; i < r; ++i) {
                    ++apple;
                    while (occupied_[apple] || wall_[apple])
                        ++apple;
                }
            }
        } else {
            // decrement tail
            for (auto i = occupied_.begin();
                 i != occupied_.end(); ++i) {
                if (*i > 0)
                    --(*i);
            }
        }
    }
}

int State::point(int x, int y) const {
    return (x * HEIGHT + y) % SIZE;
}

int State::point(int p, Direction dir) const {
    if (p < 0) {
        return -1;
    }
    int newX = x(p) + dir.x();
    int newY = y(p) + dir.y();

    if (newX < 0 || newX >= WIDTH || newY < 0 ||
        newY >= HEIGHT)
        return -1;

    return point(newX, newY);
}

int State::x(int p) const {
    return (p % SIZE) / HEIGHT;
}

int State::y(int p) const {
    return p % HEIGHT;
}

} // namespace Snake

#endif