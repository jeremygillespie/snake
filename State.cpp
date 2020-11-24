#include "State.hpp"

namespace Snake {

State::State() :
        head{0},
        apple{0},
        length{(HEIGHT + 1) / 2},
        time{0},
        board(SIZE, 0),
        visited(SIZE, false) {
    int y = length - 1;
    int x = (WIDTH + 1) / 2 - 1;

    head = point(x, y);

    for (int i = 0; i < length; ++i) {
        board[point(x, y - i)] = length - i;
    }

    nextApple(0);
    board[apple] = APPLE;
}

State::State(const State &prev, direction dir) :
        head{prev.head},
        apple{prev.apple},
        length{prev.length},
        time{prev.time + 1},
        board{prev.board},
        visited{prev.visited} {

    head = point(head, dir);

    if (val(head) == APPLE) {
        ++length;
        board[head] = length;

        visited = vector<bool>(SIZE, false);
        visited[head] = true;

        if (length < SIZE) {
            nextApple(0);
            board[apple] = APPLE;
        }
    } else {
        for (auto &p : board) {
            if (p > 0)
                --p;
        }
        board[head] = length;
        visited[head] = true;
    }
}

State::State(const State &prev) :
        head{prev.head},
        apple{prev.apple},
        length{prev.length},
        time{prev.time},
        board{prev.board},
        visited{prev.visited} {
    board[apple] = 0;
    nextApple(apple + 1);
    board[apple] = APPLE;
}

State::State(const State &prev, int n) :
        head{prev.head},
        apple{prev.apple},
        length{prev.length},
        time{prev.time},
        board{prev.board},
        visited{prev.visited} {
    board[apple] = 0;
    for (int i = 0; i < n; ++i) {
        nextApple(apple + 1);
    }

    board[apple] = APPLE;
}

bool State::canMove(direction dir) const {
    switch (dir) {
    case direction::right:
        return xCoord(head) < WIDTH - 1 && val(point(dir)) <= 1;
    case direction::left:
        return xCoord(head) > 0 && val(point(dir)) <= 1;
    case direction::up:
        return yCoord(head) < HEIGHT - 1 && val(point(dir)) <= 1;
    default:
        return yCoord(head) > 0 && val(point(dir)) <= 1;
    }
}

bool State::canExplore(direction dir) const {
    return visited[point(dir)] == false;
}

int State::val(int p) const { return board[p]; }

int State::point(direction dir) const { return point(head, dir); }

int State::point(int x, int y) const { return x * HEIGHT + y; }

int State::point(int p, direction dir) const {
    switch (dir) {
    case direction::right:
        return point(xCoord(p) + 1, yCoord(p));
    case direction::left:
        return point(xCoord(p) - 1, yCoord(p));
    case direction::up:
        return point(xCoord(p), yCoord(p) + 1);
    default:
        return point(xCoord(p), yCoord(p) - 1);
    }
}

int State::xCoord(int p) const { return p / HEIGHT; }

int State::yCoord(int p) const { return p % HEIGHT; }

void State::nextApple(int p) {
    while (val(p) != EMPTY) {
        ++p;
    }
    apple = p;
}

} // namespace Snake