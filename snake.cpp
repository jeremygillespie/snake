#include <string>
#include <iostream>
#include <chrono>

#include "Game.hpp"

using namespace std;

void print(Game g);

int main(int argc, char *argv[])
{
    int x = 10, y = 10, l = 4;
    if(argc > 2)
    {
        x = stoi(argv[1]);
        y = stoi(argv[2]);
    }
    if(argc > 3)
    {
        l = stoi(argv[3]);
    }

    unsigned s = std::chrono::system_clock::now().time_since_epoch().count();

    Game g = Game(x, y, l, s);
    print(g);

    for(char c;;)
    {
        cin >> c;
        bool cont = true;
        if(c == 'w')
            cont = g.update(UP);
        else if(c == 's')
            cont = g.update(DOWN);
        else if(c == 'a')
            cont = g.update(LEFT);
        else if(c == 'd')
            cont = g.update(RIGHT);
        else
            break;
        if(cont == false)
            break;
        else
            print(g);
    }

}

void print(Game g)
{
    for(int y=g.height-1; y>=0; --y)
    {
        for(int x=0; x<g.width; ++x)
        {
            int c = g.state[x][y];
            if(c == 0)
                cout << ".";
            else if(c == -1)
                cout << "@";
            else if(c == g.snakeLength)
                cout << "H";
            else if(c > 9)
                cout << "#";
            else
                cout << c;
            cout << " ";
        }
        cout << "\n";
    }
    cout << g.tailX << "," << g.tailY << "\n";
    cout << "\n";
};