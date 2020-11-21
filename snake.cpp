#include "Game.hpp"

int main(int argc, char *argv[])
{

    Game g = Game(10, 10);
    g.print();

    for(char c;;)
    {
        std::cin >> c;
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