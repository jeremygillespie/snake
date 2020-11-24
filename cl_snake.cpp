#include <chrono>
#include <iostream>
#include <random>
#include <string>

#include "MinMax.hpp"
#include "State.hpp"

using namespace std;
using namespace Snake;

void print(const State &s);

int main() {
    int seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine engine(seed);

    State s = State();

    uniform_int_distribution<> dist(0, State::SIZE - s.length - 1);
    s = State(s, dist(engine));

    print(s);

    for (char c;;) {
        cin >> c;
        direction dir;

        switch (c) {
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
        case 'h':
            cout << "\n";
            dir = MinMax::bestMove(s);
            break;
        default:
            return 0;
        }

        if (s.canMove(dir)) {
            int l = s.length;
            s = State(s, dir);

            if (s.length == State::SIZE) {
                print(s);
                cout << "You win!\n";
                return 0;
            }

            if (l != s.length) {
                uniform_int_distribution<> dist(0, State::SIZE - s.length - 1);
                s = State(s, dist(engine));
            }
        } else {
            cout << "You lose!\n";
            return 0;
        }

        print(s);
    }
}

void print(const State &s) {
    for (int y = State::HEIGHT - 1; y >= 0; --y) {
        for (int x = 0; x < State::WIDTH; ++x) {
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