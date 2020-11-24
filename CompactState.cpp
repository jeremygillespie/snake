#include "CompactState.hpp"

namespace Snake {

CompactState::CompactState() :
        board{} {
    length = (HEIGHT + 1) / 2;

    int x = (WIDTH + 1) / 2 - 1;

    head = x * HEIGHT + length - 1;
    tail = x * HEIGHT;

    firstApple();
}

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

CompactState CompactState::right(const CompactState &prev) {

}

CompactState CompactState::up(const CompactState &prev) {

}

CompactState CompactState::left(const CompactState &prev) {

}

CompactState CompactState::down(const CompactState &prev) {

}

CompactState CompactState::nextApple(const CompactState &prev) {

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
    return (p & SAFETY_MASK) == 0;
}

} // namespace snake