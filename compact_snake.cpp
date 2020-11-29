#include <chrono>
#include <iostream>
#include <random>

#include "Engine.hpp"
#include "State.hpp"

using namespace std;
using namespace Snake;

void print(const State &s);
void printX(const State &s, int y);

int gameTime = 0;

int main() {
    int seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine engine(seed);

    State s = State();

    uniform_int_distribution<> dist(0, State::SIZE - s.length - 1);

    int n = dist(engine);
    for (int i = 0; i < n; ++i)
        s = State::nextApple(s);

    print(s);

    for (char c;;) {
        cin >> c;
        State::chunk_type dir;

        switch (c) {
        case 'w':
            dir = State::UP;
            break;
        case 's':
            dir = State::DOWN;
            break;
        case 'a':
            dir = State::LEFT;
            break;
        case 'd':
            dir = State::RIGHT;
            break;
        case 'h': {
            Exhaustive engine(s);
            dir = engine.getPath(0).moves[0];
        } break;
        default:
            return 0;
        }

        if (s.canMove(dir)) {
            ++gameTime;

            int l = s.length;
            s = State::move(s, dir);

            if (s.length == State::SIZE) {
                print(s);
                cout << "You win!\n";
                return 0;
            }

            if (l != s.length) {
                uniform_int_distribution<> dist(0, State::SIZE - s.length - 1);
                int n = dist(engine);
                for (int i = 0; i < n; ++i)
                    s = State::nextApple(s);
            }
        } else {
            cout << "You lose!\n";
            return 0;
        }

        print(s);
    }
}

void print(const State &s) {
    for (int y = State::HEIGHT - 1; y > 0; --y) {
        printX(s, y);
    }
    printX(s, 0);
    cout << "Time: " << gameTime << "\n";
};

void printX(const State &s, int y) {
    for (int x = 0; x < State::WIDTH; ++x) {
        State::chunk_type c = s.value(x * State::HEIGHT + y);
        switch (c) {
        case State::HEAD:
            cout << "H";
            break;
        case State::APPLE:
            cout << "@";
            break;
        case State::UP:
            cout << "A";
            break;
        case State::DOWN:
            cout << "V";
            break;
        case State::LEFT:
            cout << "<";
            break;
        case State::RIGHT:
            cout << ">";
            break;
        default:
            cout << ".";
        }
        cout << " ";
    }
    cout << "\n";
}