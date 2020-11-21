#pragma once

#include "State.hpp"

class Safety
{
public:
    virtual bool operator()(const State &s) = 0;
};

class DepthFirst : public Safety
{
public:
    virtual bool operator()(const State &s);
private:
    bool recurse();
};

bool DepthFirst::operator()(const State &s)
{
    return true;
}

