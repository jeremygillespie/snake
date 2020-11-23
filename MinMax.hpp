#ifndef MINMAX_HPP
#define MINMAX_HPP

#include "State.hpp"

namespace Snake {

class MinMax
{
    static float evalMove(const State &prev)
    {
        if (prev.length == State::SIZE)
        {
            return 0.0f;
        }
        // else if(head == anywhere head has been before)
        {
            return -1.0f;
        }
        // else
        float bestEval = -1.0f;
        direction bestMove = direction::up;
        float eval = 0.0f;

        // for each possible move next = State(prev, dir)
            // if(next.length > prev.length)
                // eval = evalApple(next)
            // else
                // eval = evalMove(next)
            // if( bestEval == -1.0f || eval < bestEval )
                // bestEval = eval; bestMove = dir;

        if (bestEval == -1.0f)
        {
            return -1.0f;
        }
        else
        {
            return bestEval + 1.0f;
        }
    }

    // requires State with apple at 0
    static float evalApple(const State &prev)
    {
        int n = State::SIZE - prev.length;
        float sum = 0.0f;

        State s = prev;
        float eval = evalMove(s);
        if(eval == -1.0f)
            return -1.0f;

        for(int i=1; i<n; ++i)
        {
            s = State(s);
            eval = evalMove(s);
            if(eval == -1.0f)
                return -1.0f;
            sum += eval;
        }

        return sum / n;
    }

};

} // namespace Snake

#endif