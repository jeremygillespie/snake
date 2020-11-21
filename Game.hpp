#pragma once

#include <random>

#include "State.hpp"

enum direction {RIGHT, LEFT, UP, DOWN};

class Game
{
public:
    Game(unsigned x, unsigned y, unsigned length, unsigned seed=0);

    bool update(direction d);

    const unsigned width, height;
    unsigned headX, headY, appleX, appleY, snakeLength;
    vector<vector<int>> state;

private:
    bool postupdate();
    void decrement();
    void apple();

    std::default_random_engine engine;
};

Game::Game(unsigned x, unsigned y, unsigned length, unsigned seed) :
    width{x},
    height{y},
    headX{(width+1)/2 - 1},
    headY{(height+1)/2 - 1},
    appleX{0},
    appleY{0},
    snakeLength{0},
    state(x, vector<int>(y, 0)),
    engine(seed)
{
    if(length > width*height / 2 + width + height ||
        width < 2 || height < 2)
        return;

    state[headX][headY] = length;
    ++snakeLength;

    int tailX = headX, tailY = headY;

    direction d = DOWN;
    while(snakeLength < length)
    {
        switch(d)
        {
            case DOWN:
                if(tailY > 0 && state[tailX][tailY-1] == 0)
                {
                    --tailY;
                    state[tailX][tailY] = length - snakeLength;
                    ++snakeLength;
                }
                else
                {
                    d = LEFT;
                }
                break;
            case LEFT:
                if(tailX > 0 && state[tailX-1][tailY] == 0)
                {
                    --tailX;
                    state[tailX][tailY] = length - snakeLength;
                    ++snakeLength;
                }
                else
                {
                    d = UP;
                }
                break;
            case UP:
                if(tailY < height-1 && state[tailX][tailY+1] == 0)
                {
                    ++tailY;
                    state[tailX][tailY] = length - snakeLength;
                    ++snakeLength;
                }
                else
                {
                    d = RIGHT;
                }
                break;
            case RIGHT:
                if(tailX < width-1 && state[tailX+1][tailY] == 0)
                {
                    ++tailX;
                    state[tailX][tailY] = length - snakeLength;
                    ++snakeLength;
                }
                else
                {
                    d = DOWN;
                }
                break;
        }
    }

    apple();
};

bool Game::update(direction d)
{
    switch(d)
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
    switch(state[headX][headY])
    {
        case 1:
            decrement();
            state[headX][headY] = snakeLength;
            return true;
        case 0:
            decrement();
            state[headX][headY] = snakeLength;
            return true;
        case -1:
            ++snakeLength;
            state[headX][headY] = snakeLength;
            apple();
            return true;
        default:
            return false;
    }
};

void Game::decrement()
{
    for(int x=0; x<width; ++x)
    {
        for(int y=0; y<height; ++y)
        {
            if(state[x][y] > 0)
            {
                --state[x][y];
            }
        }
    }
}

void Game::apple()
{
    int x = 0, y = 0;

    // pick a random emtpy space
    std::uniform_int_distribution<int> dist(1,width*height-snakeLength);
    int n = dist(engine);

    // skip n-1 emtpy spaces
    for(int i=1; i<n;)
    {
        if(state[x][y] == 0)
            ++i;
        if(++x == width)
        {
            x = 0;
            ++y;
        }
    }
    // find the nth empty space
    while(state[x][y] != 0)
    {
        if(++x == width)
        {
            x = 0;
            ++y;
        }
    }
    appleX = x;
    appleY = y;
    state[x][y] = -1;
};