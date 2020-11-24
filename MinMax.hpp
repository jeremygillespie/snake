#ifndef MINMAX_HPP
#define MINMAX_HPP

#include <iostream>
#include <string>

using std::cout;

#include "State.hpp"

namespace Snake {

std::string directionstring(direction dir) {
    switch (dir) {
    case direction::right:
        return "right";
    case direction::left:
        return "left";
    case direction::up:
        return "up";
    default:
        return "down";
    }
};

class MinMax {
public:
    static direction bestMove(const State &prev) {
        float bestEval = -1.0f;
        direction bestMove = direction::up;

        for (direction dir : allDir) {
            if (prev.canMove(dir) && prev.canExplore(dir)) {
                State next = State(prev, dir);
                float eval = evalMove(next);
                cout << directionstring(dir) << ": " << eval << "\n";
                if (bestEval == -1.0f || eval < bestEval) {
                    bestEval = eval;
                    bestMove = dir;
                }
            }
        }

        return bestMove;
    }

    static float evalMove(const State &prev) {
        if (prev.length == State::SIZE) {
            return 0.0f;
        }

        float bestEval = 1000.0f;

        for (direction dir : allDir) {
            if (prev.canMove(dir) && prev.canExplore(dir)) {
                State next = State(prev, dir);
                float eval;
                if (next.length > prev.length) {
                    if (next.length == State::SIZE) {
                        eval = 0.0f;
                    } else {
                        eval = evalApple(next);
                    }
                } else {
                    eval = evalMove(next);
                }
                if (eval < bestEval) {
                    bestEval = eval;
                }
            }
        }

        return bestEval + 1.0f;
    }

    // requires State with apple at 0
    static float evalApple(const State &prev) {
        int n = State::SIZE - prev.length;
        float sum = 0.0f;

        for (int i = 0; i < n; ++i) {
            State s = State(prev, i);
            float eval = evalMove(s);
            sum += eval;
        }

        return sum / n;
    }
};

} // namespace Snake

#endif