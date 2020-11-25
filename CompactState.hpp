#ifndef COMPACT_STATE_HPP
#define COMPACT_STATE_HPP

#ifndef SNAKE_STATE_BOARD_WIDTH
#define SNAKE_STATE_BOARD_WIDTH 4
#define SNAKE_STATE_BOARD_HEIGHT 4
#endif

#include <array>
#include <cstdint>

namespace Snake {

class CompactState {
public:
    using chunk_type = uint_fast8_t;
    using size_type = uint_fast16_t;

    static constexpr size_type WIDTH = SNAKE_STATE_BOARD_WIDTH;
    static constexpr size_type HEIGHT = SNAKE_STATE_BOARD_HEIGHT;
    static constexpr size_type SIZE = WIDTH * HEIGHT;
    static constexpr size_type SPACE = SIZE * SIZE;

    // assert size fits in size_type

    static constexpr chunk_type RIGHT = 0U;
    static constexpr chunk_type UP = 1U;
    static constexpr chunk_type LEFT = 2U;
    static constexpr chunk_type DOWN = 3U;
    static constexpr chunk_type EMPTY = 4U;
    static constexpr chunk_type HEAD = 5U;
    static constexpr chunk_type APPLE = 6U;

    size_type head, tail, apple, length;

    // new game constructor
    CompactState();

    // next apple constructor
    CompactState(const CompactState &prev);

    bool canMove(chunk_type dir) const;

    // step constructor
    CompactState(const CompactState &prev, chunk_type dir);

    chunk_type val(size_type pos) const;

    CompactState &operator=(const CompactState &other) = default;

private:
    static constexpr size_type B_POINT = 4; // bits per point
    static constexpr size_type P_CHUNK =    // point s per chunk
        sizeof(chunk_type) * 8 / B_POINT;

    std::array<chunk_type, (SIZE - 1) / P_CHUNK + 1> board;

    chunk_type point(size_type pos) const;

    // set masked bits to true
    void point(size_type pos, chunk_type mask);

    // set masked bits to val
    void point(size_type pos, chunk_type mask, chunk_type val);

    static size_type step(size_type pos, chunk_type dir);

    bool movable(chunk_type p) const;

    void eatApple();

    void moveTail();

    // bit 0: direction axis
    // bit 1: direction sign
    // bit 2: visited
    // bit 3: occupied
    static constexpr chunk_type DIRECTION_MASK = 0b0011;
    static constexpr chunk_type MOVABLE_MASK = 0b1100;
    static constexpr chunk_type OCCUPIED_MASK = 0b0100;
    static constexpr chunk_type VISITED_MASK = 0b1000;
};

} // namespace Snake

#endif