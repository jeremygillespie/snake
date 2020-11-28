#ifndef SAFETY_SEARCH_HPP
#define SAFETY_SEARCH_HPP

#include <queue>
#include <vector>

#include "State.hpp"

namespace Snake {

namespace SafetySearch {

struct Path {};

class Exhaustive {
public:
    // returns potentially unsafe apple spawn locations
    virtual bool operator()(std::vector<int> &result) = 0;
};

class DoubleBreadthFirst : public Exhaustive {
    DoubleBreadthFirst(const State &start) : start{start} {}

    bool operator()(std::vector<int> &result) {

        if (paths.empty()) {
            for (;;) {
                if (search.empty()) {
                    return false;
                }

                // pop a successor state
                // foreach dir if canLoop(dir) add Path(dir) to paths
            }
        }

        //  pop a path
        //  for each point along the path try to find a way to circumvent it
        //  return all points you can't circumvent

        return true;
    }

private:
    State start;

    std::queue<State> search;
    std::queue<Path> paths;
};

} // namespace SafetySearch

} // namespace Snake

#endif