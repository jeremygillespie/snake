#include <string>

#include "Game.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    int x = 10, y = 10;
    if(argc > 2)
    {
        x = stoi(argv[1]);
        y = stoi(argv[2]);
    }

    Game g = Game(x, y);
    g.print();

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
            g.print();
    }

}