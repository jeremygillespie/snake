#ifndef MINMAX_HPP
#define MINMAX_HPP

#include <iostream>
#include <string>

using std::cout;

#include "State.hpp"

namespace Snake {

class Search {
public:
    static direction bestMove(const State &prev) {
        float bestEval = -1.0f;
        direction bestMove = direction::up;

        for (direction dir : allDir) {
            if (prev.canMove(dir) && prev.canExplore(dir)) {
                State next = State(prev, dir);
                float eval = evalMove(next);
                if (bestEval == -1.0f || eval < bestEval) {
                    bestEval = eval;
                    bestMove = dir;
                }
            }
        }

        return bestMove;
    }

    static int evalState(const State &state) {
        // success
        if (state.length == State::SIZE) {
            return state.time;
        }
        for (direction dir : allDir) {
            // neither success nor failure
            if (state.canMove(dir) && state.canExplore(dir)) {
                return State::SIZE * (State::SIZE - state.length) + state.time;
            }
        }
        // failure
        return State::SPACE - state.time;
    }

    static float evalMove(const State &prev) {
        float bestEval = (float)(State::SPACE - prev.time + 1);

        for (direction dir : allDir) {
            if (prev.canMove(dir) && prev.canExplore(dir)) {
                State next = State(prev, dir);
                float eval;
                if (next.length > prev.length) {
                    if (next.length == State::SIZE) {
                        eval = (float)next.time;
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

        return bestEval - 1.0f;
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