#include "State.hpp"

State::State(int w, int h, int l)
    : width{w}, height{h},
      head{0}, apple{0}, length{l}, time{0},
      board(width * height)
{
    if (l >= width * height)
        return;

    if (length <= (height + 1) / 2)
    {
        int x = (width + 1) / 2 - 1;
        int y = (height + 1) / 2 - 1;

        head = point(x, y);

        for (int i = 0; i < length; ++i)
        {
            val(point(x, y - i)) = length - i;
        }
    }
    else
    {
        for (int i = 0; i < length; ++i)
        {
            int x = xCoord(i);
            int y = yCoord(i);
            if (x % 2 == 1)
            {
                y = height - y - 1;
            }

            val(point(x, y)) = i + 1;

            if (i == length - 1)
            {
                head = i;
            }
        }
    }

    nextApple(0);
    val(apple) = APPLE;
}

State::State(const State &prev, direction dir)
    : width{prev.width},
      height{prev.height},
      head{prev.head},
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
        nextApple(0);
        val(apple) = APPLE;
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
    : width{prev.width},
      height{prev.height},
      head{prev.head},
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
    : width{prev.width},
      height{prev.height},
      head{prev.head},
      apple{prev.apple},
      length{prev.length},
      time{prev.time},
      board{prev.board}
{
    if(n > width * height - length)
    {
        return;
    }

    val(apple) = 0;
    for(int i = 0; i < n; ++i)
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
        return xCoord(head) < width - 1 && val(point(dir)) <= EMPTY;
    case direction::left:
        return xCoord(head) > 0 && val(point(dir)) <= EMPTY;
    case direction::up:
        return yCoord(head) < height - 1 && val(point(dir)) <= EMPTY;
    default:
        return yCoord(head) > 0 && val(point(dir)) <= EMPTY;
    }
}

int &State::val(int p) { return board[p]; }

const int State::val(int p) const { return board[p]; }

const int State::point(direction dir) const { return point(head, dir); }

const int State::point(int x, int y) const { return x * height + y; }

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

const int State::xCoord(int p) const { return p / height; }

const int State::yCoord(int p) const { return p % height; }

void State::nextApple(int p)
{
    while (val(p) != EMPTY)
    {
        ++p;
    }
    apple = p;
}