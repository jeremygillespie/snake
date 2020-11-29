#include <chrono>
#include <iostream>
#include <random>

#include "Engine.hpp"
#include "State.hpp"

using std::cin;
using std::cout;
using namespace Snake;

void print(const State &s);
void printX(const State &s, int y);
void printPath(const AppleSearch::Path &p);

int gameTime = 0;

int main() {
    int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine engine(seed);

    State s = State();

    std::uniform_int_distribution<> dist(1, State::SIZE - s.length);

    int n = dist(engine);
    for (int i = 0; i < n; ++i)
        s = State::nextApple(s);

    print(s);

    for (;;) {
        char c;

        State::chunk_type dir = 0U;

        cin >> c;

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
            Exhaustive engine = Exhaustive(s);
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
                std::uniform_int_distribution<> dist(1, State::SIZE - s.length);
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

void printPath(const AppleSearch::Path &p) {
    for (size_t i = 0; i < p.moves.size(); ++i) {
        State::chunk_type dir = p.moves[i];
        switch (dir) {
        case State::UP:
            cout << "U";
            break;
        case State::DOWN:
            cout << "D";
            break;
        case State::LEFT:
            cout << "L";
            break;
        case State::RIGHT:
            cout << "R";
            break;
        }
    }
    cout << "\n";
}