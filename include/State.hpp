#pragma once

#include <vector>

using std::vector;

enum class direction
{
    right,
    left,
    up,
    down
};

class State
{
public:
    static const int APPLE = -1;
    static const int EMPTY = 0;

    // new state
    State(int w, int h, int l);

    // state after move
    State(const State &prev, direction dir);

    // state with different apple
    State(const State &prev);

    const int width, height;

    int head, apple, length, time;

    const bool canMove(direction dir) const;

    int &val(int p);
    int &val(int x, int y);

    const int &val(int p) const;
    const int &val(int x, int y) const;

    const int point(int p, direction dir) const;
    const int point(direction dir) const;
    const int point(int x, int y) const;

    const int xCoord(int p) const;
    const int yCoord(int p) const;

private:
    vector<int> board;

    void nextApple(int p);
};