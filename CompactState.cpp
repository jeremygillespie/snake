#include "CompactState.hpp"

namespace Snake {

bool CompactState::canRight() const {
    return head + HEIGHT < SIZE && safe(point(head + HEIGHT));
}

bool CompactState::canLeft() const {
    return head - HEIGHT >= 0 && safe(point(head - HEIGHT));
}

bool CompactState::canUp() const {
    return head % HEIGHT < HEIGHT - 1 && safe(point(head + 1));
}

bool CompactState::canDown() const {
    return head % HEIGHT > 0 && safe(point(head - 1));
}

CompactState::byte CompactState::point(int pos) const {
    if(pos % 2 == 0) {
        return board[pos];
    }
    else {
        return board[pos] >> 4;
    }
}

bool CompactState::safe(byte p) const {
    return (p & SAFETY_MASK) == SAFETY_MASK;
}

} // namespace snake