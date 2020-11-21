#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <chrono>

using std::vector;
using std::cout;

const char EMPTY = 0,
    HEAD = 1,
    APPLE = 2,
    RIGHT = 3,
    LEFT = 4,
    UP = 5,
    DOWN = 6;

const char output[] = {
    '.','@','O','#','#','#','#'
};

class Game
{
public:
    Game(int x, int y, unsigned seed=0);

    bool update(char direction);

    void print();

    const int width, height;
    int headX, headY, tailX, tailY, appleX, appleY, nEmpty;
    vector<vector<char>> state;

private:
    bool postupdate();
    void apple();

    std::default_random_engine engine;
};

Game::Game(int x, int y, unsigned seed) :
    width{x},
    height{y},
    headX{width/2 - 1},
    headY{height/2 - 1},
    tailX{headX},
    tailY{headY - 3},
    appleX{0},
    appleY{0},
    nEmpty{width*height - 4},
    state(x, vector<char>(y, EMPTY)),
    engine(seed)
{
    if(seed == 0)
    {
        unsigned s = std::chrono::system_clock::now().time_since_epoch().count();
        engine = std::default_random_engine(s);
    }
    state[headX][headY] = HEAD;
    for(int i=1; i<=3; ++i)
        state[headX][headY-i] = UP;
    apple();
};

bool Game::update(char direction)
{
    state[headX][headY] = direction;
    switch(direction)
    {
        case RIGHT:
            if(++headX >= width)
                return false;
            return postupdate();
        case LEFT:
            if(--headX < 0)
                return false;
            return postupdate();
        case UP:
            if(++headY >= height)
                return false;
            return postupdate();
        case DOWN:
            if(--headY < 0)
                return false;
            return postupdate();
        default:
            return false;
    }
};

bool Game::postupdate()
{
    char taildir = state[tailX][tailY];

    switch(state[headX][headY])
    {
        case EMPTY:
            state[headX][headY] = HEAD;

            state[tailX][tailY] = EMPTY;
            switch(taildir)
            {
                case RIGHT:
                    ++tailX; break;
                case LEFT:
                    --tailX; break;
                case UP:
                    ++tailY; break;
                case DOWN:
                    --tailY; break;
            }
            return true;
        case APPLE:
            state[headX][headY] = HEAD;
            apple();
            return true;
        default:
            return false;
    }
};

void Game::apple()
{
    int x = 0, y = 0;

    // pick a random emtpy space
    std::uniform_int_distribution<int> dist(1,nEmpty);
    int n = dist(engine);

    // skip n-1 emtpy spaces
    for(int i=1; i<n;)
    {
        if(state[x][y] == EMPTY)
            ++i;
        if(++x == width)
        {
            x = 0;
            ++y;
        }
    }
    // find the nth empty space
    while(state[x][y] != EMPTY)
    {
        if(++x == width)
        {
            x = 0;
            ++y;
        }
    }
    appleX = x;
    appleY = y;
    state[x][y] = APPLE;
    --nEmpty;
};

void Game::print()
{
    for(int y=height-1; y>=0; --y)
    {
        for(int x=0; x<width; ++x)
        {
            cout << output[state[x][y]] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
};