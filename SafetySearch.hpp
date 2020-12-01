#ifndef SAFETY_SEARCH_HPP
#define SAFETY_SEARCH_HPP

#include <queue>
#include <vector>

#include "State.hpp"

namespace Snake {

class SafetySearch {
public:
    // return potentially unsafe apple spawn locations
    virtual bool operator()(std::vector<int> &result) = 0;
};

} // namespace Snake

#endif