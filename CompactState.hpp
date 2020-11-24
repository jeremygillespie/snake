#ifndef COMPACT_STATE_HPP
#define COMPACT_STATE_HPP

#include <cstdint>
#include <cassert>

namespace snake {

class CompactState {
public:
    static constexpr int WIDTH = 8;
    static constexpr int HEIGHT = 8;
    static constexpr int SIZE = WIDTH * HEIGHT;
    static constexpr int SPACE = SIZE * SIZE;

    using column_t = std::uint8_t;
    static constexpr int BITS_PER_COLUMN = sizeof(column_t)*8;
    static_assert(HEIGHT <= BITS_PER_COLUMN, "Height exceeds size of column_t");

    CompactState();
    static CompactState right(const CompactState &prev);
    static CompactState up(const CompactState &prev);
    static CompactState left(const CompactState &prev);
    static CompactState down(const CompactState &prev);
    static CompactState eat(const CompactState &prev);
    static CompactState nextApple(const CompactState &prev);

private:
    static constexpr column_t RIGHT = 0U;
    static constexpr column_t UP = 1U;
    static constexpr column_t LEFT = 2U;
    static constexpr column_t DOWN = 3U;

    int head, tail, apple, len;

    column_t body[SIZE * 2 / BITS_PER_COLUMN + 1];
    column_t visited[WIDTH];
    column_t occupied[WIDTH];
};

} // namespace snake

#endif