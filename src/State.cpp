#include "State.hpp"

State::State()
    : head{0}, apple{0}, length{(HEIGHT + 1) / 2}, time{0},
      board(SIZE)
{
    int x = length - 1;
    int y = (WIDTH + 1) / 2 - 1;

    head = point(x, y);

    for (int i = 0; i < length; ++i)
    {
        val(point(x, y - i)) = length - i;
    }

    nextApple(0);
    val(apple) = APPLE;
}

State::State(const State &prev, direction dir)
    : head{prev.head},
      apple{prev.apple},
      length{prev.length},
      time{prev.time + 1},
      board{prev.board}
{

    head = point(head, dir);

    if (val(head) == APPLE)
    {
        ++length;
        val(head) = length;
        if (length < SIZE)
        {
            nextApple(0);
            val(apple) = APPLE;
        }
    }
    else
    {
        for (auto &p : board)
        {
            if (p > 0)
                --p;
        }
        val(head) = length;
    }
}

State::State(const State &prev)
    : head{prev.head},
      apple{prev.apple},
      length{prev.length},
      time{prev.time},
      board{prev.board}
{
    val(apple) = 0;
    nextApple(apple + 1);
    val(apple) = APPLE;
}

State::State(const State &prev, int n)
    : head{prev.head},
      apple{prev.apple},
      length{prev.length},
      time{prev.time},
      board{prev.board}
{
    val(apple) = 0;
    for (int i = 0; i < n; ++i)
    {
        nextApple(apple + 1);
    }

    val(apple) = APPLE;
}

const bool State::canMove(direction dir) const
{
    switch (dir)
    {
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

int &State::val(int p) { return board[p]; }

const int State::val(int p) const { return board[p]; }

const int State::point(direction dir) const { return point(head, dir); }

const int State::point(int x, int y) const { return x * HEIGHT + y; }

const int State::point(int p, direction dir) const
{
    switch (dir)
    {
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

const int State::xCoord(int p) const { return p / HEIGHT; }

const int State::yCoord(int p) const { return p % HEIGHT; }

void State::nextApple(int p)
{
    while (val(p) != EMPTY)
    {
        ++p;
    }
    apple = p;
}