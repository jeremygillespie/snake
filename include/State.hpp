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

    inline static int width;
    inline static int height;
    static int size() { return width * height; };

    // new state
    State(int l);

    // state after move
    State(const State &prev, direction dir);

    // state with next possible apple
    // equivalent to n=1
    State(const State &prev);

    // state with apple offset by n spaces
    State(const State &prev, int n);

    int head, apple, length, time;

    const bool canMove(direction dir) const;

    int &val(int p);
    const int val(int p) const;

    const int point(int p, direction dir) const;
    const int point(direction dir) const;
    const int point(int x, int y) const;

    const int xCoord(int p) const;
    const int yCoord(int p) const;

private:
    vector<int> board;

    void nextApple(int p);
};