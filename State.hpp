#pragma once

#include <vector>

using std::vector;

class State
{
public:
    State(unsigned x, unsigned y, unsigned length);
    State(const State &other);

    const unsigned width, height;
    unsigned headX, headY, appleX, appleY, snakeLength;
    vector<vector<int>> state;
};