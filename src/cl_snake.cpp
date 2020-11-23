#include <string>
#include <iostream>
#include <chrono>
#include <random>

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

    State::width = w;
    State::height = h;

    int seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine engine(seed);

    State s = State(l);

    uniform_int_distribution<> dist(0, State::size() - s.length - 1);
    s = State(s, dist(engine));

    print(s);

    for (char c;;)
    {
        cin >> c;
        direction dir;

        switch (c)
        {
        case 'w':
            dir = direction::up;
            break;
        case 's':
            dir = direction::down;
            break;
        case 'a':
            dir = direction::left;
            break;
        case 'd':
            dir = direction::right;
            break;
        default:
            return 0;
        }

        if (s.canMove(dir))
        {
            int l = s.length;
            s = State(s, dir);
            if (l != s.length)
            {
                uniform_int_distribution<> dist(0, State::size() - s.length - 1);
                s = State(s, dist(engine));
            }
        }
        else
        {
            cout << "You lose!\n";
            return 0;
        }

        print(s);

        if (s.length == State::size())
        {
            cout << "You win!\n";
            return 0;
        }
    }
}

void print(const State &s)
{
    for (int y = s.height - 1; y >= 0; --y)
    {
        for (int x = 0; x < s.width; ++x)
        {
            int c = s.val(s.point(x, y));
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
    cout << "Time: " << s.time << "\n";
};