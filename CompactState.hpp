#ifndef COMPACT_STATE_HPP
#define COMPACT_STATE_HPP

#include <cstdint>

namespace Snake {

class CompactState {
public:

    static constexpr int WIDTH = 8;
    static constexpr int HEIGHT = 8;
    static constexpr int SIZE = WIDTH * HEIGHT;
    static constexpr int SPACE = SIZE * SIZE;

    int head, tail, apple, len;

    CompactState();

    bool canRight() const;
    bool canUp() const;
    bool canLeft() const;
    bool canDown() const;

    static CompactState right(const CompactState &prev);
    static CompactState up(const CompactState &prev);
    static CompactState left(const CompactState &prev);
    static CompactState down(const CompactState &prev);

    static CompactState firstApple(const CompactState &prev);
    static CompactState nextApple(const CompactState &prev);

private:
    using byte = uint8_t;

    byte board[SIZE + 1 / 2];

    byte point(int pos) const;

    bool safe(byte p) const;

    static constexpr byte RIGHT = 0U;
    static constexpr byte UP = 1U;
    static constexpr byte LEFT = 2U;
    static constexpr byte DOWN = 3U;

    static constexpr byte DIRECTION_MASK = 3U; // bits 1, 2
    static constexpr byte SAFETY_MASK = 3U << 2; // bits 3, 4
    static constexpr byte NOVELTY_MASK = (1U << 3) + (1U << 7); // bits 4, 8
};

} // namespace snake

#endif