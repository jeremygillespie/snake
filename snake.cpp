#include "Game.hpp"

int main(int argc, char *argv[])
{

    Game g = Game(10, 10);
    g.print();

    std::string in;
    while(getline(std::cin, in))
    {
        bool cont = true;
        if(in.compare("w") == 0)
            cont = g.update(UP);
        else if(in.compare("s") == 0)
            cont = g.update(DOWN);
        else if(in.compare("a") == 0)
            cont = g.update(LEFT);
        else if(in.compare("d") == 0)
            cont = g.update(RIGHT);
        else
            break;
        if(cont == false)
            break;
        else
            g.print();
    }

}