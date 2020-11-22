#include <string>
#include <iostream>

#include "State.hpp"

using namespace std;

void print(const State &s);

int main(int argc, char *argv[])
{
    int w = 10, h = 10, l = 4;
    if (argc > 2)
    {
        w = stoi(argv[1]);
        h = stoi(argv[2]);
    }
    if (argc > 3)
    {
        l = stoi(argv[3]);
    }

    State s(w, h, l);

    print(s);

    // int s = std::chrono::system_clock::now().time_since_epoch().count();

    // Game g = Game(x, y, l, s);
    // print(g);

    // for(char c;;)
    // {
    //     cin >> c;
    //     bool cont = true;
    //     if(c == 'w')
    //         cont = g.update(UP);
    //     else if(c == 's')
    //         cont = g.update(DOWN);
    //     else if(c == 'a')
    //         cont = g.update(LEFT);
    //     else if(c == 'd')
    //         cont = g.update(RIGHT);
    //     else
    //         break;
    //     if(cont == false)
    //         break;
    //     else
    //         print(g);
    // }
}

void print(const State &s)
{
    for (int y = s.height - 1; y >= 0; --y)
    {
        for (int x = 0; x < s.width; ++x)
        {
            int c = s.val(x, y);
            if (c == 0)
                cout << ".";
            else if (c == -1)
                cout << "@";
            else if (c == s.length)
                cout << "H";
            else if (c > 9)
                cout << "#";
            else
                cout << c;
            cout << " ";
        }
        cout << "\n";
    }
    cout << "\n";
};