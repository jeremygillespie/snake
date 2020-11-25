#include <chrono>
#include <iostream>
#include <random>

#include "CompactState.hpp"

using namespace std;
using namespace Snake;

void print(const CompactState &s);

int gameTime = 0;

int main() {
    int seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine engine(seed);

    CompactState s = CompactState();

    uniform_int_distribution<> dist(0, CompactState::SIZE - s.length - 1);
    s = CompactState(s, dist(engine));

    print(s);

    for (char c;;) {
        cin >> c;
        CompactState::chunk_type dir;

        switch (c) {
        case 'w':
            dir = CompactState::UP;
            break;
        case 's':
            dir = CompactState::DOWN;
            break;
        case 'a':
            dir = CompactState::LEFT;
            break;
        case 'd':
            dir = CompactState::RIGHT;
            break;
        // case 'h':
        //     cout << "\n";
        //     dir = Search::bestMove(s);
        //     break;
        default:
            return 0;
        }

        if (s.canMove(dir)) {
            CompactState::size_type l = s.length;
            s = CompactState(s, dir);

            if (s.length == CompactState::SIZE) {
                print(s);
                cout << "You win!\n";
                return 0;
            }

            if (l != s.length) {
                uniform_int_distribution<> dist(0, CompactState::SIZE -
                                                       s.length - 1);
                s = CompactState(s, dist(engine));
            }
        } else {
            cout << "You lose!\n";
            return 0;
        }

        print(s);
    }
}

void printX(const CompactState &s, CompactState::size_type y);

void print(const CompactState &s) {
    for (CompactState::size_type y = CompactState::HEIGHT - 1; y > 0; --y) {
        printX(s, y);
    }
    printX(s, 0);
    cout << "Time: " << gameTime << "\n";
};

void printX(const CompactState &s, CompactState::size_type y) {
    for (CompactState::size_type x = 0; x < CompactState::WIDTH; ++x) {
        CompactState::chunk_type c = s.val(x * CompactState::HEIGHT + y);
        switch (c) {
        case CompactState::HEAD:
            cout << "H";
            break;
        case CompactState::APPLE:
            cout << "@";
            break;
        case CompactState::UP:
            cout << "A";
            break;
        case CompactState::DOWN:
            cout << "V";
            break;
        case CompactState::LEFT:
            cout << "<";
            break;
        case CompactState::RIGHT:
            cout << ">";
            break;
        default:
            cout << ".";
        }
        cout << " ";
    }
    cout << "\n";
}