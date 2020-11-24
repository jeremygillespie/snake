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

    int head, tail, apple, length;

    CompactState();

    bool canRight() const;
    bool canUp() const;
    bool canLeft() const;
    bool canDown() const;

    static CompactState right(const CompactState &prev);
    static CompactState up(const CompactState &prev);
    static CompactState left(const CompactState &prev);
    static CompactState down(const CompactState &prev);

    static CompactState nextApple(const CompactState &prev);

private:
    using chunk_t = uint_fast8_t;
    static constexpr int B_POINT = 4; // bits per point
    static constexpr int P_CHUNK =
        sizeof(chunk_t) * 8 / B_POINT; // points per chunk
    chunk_t board[(SIZE - 1) / P_CHUNK + 1];

    chunk_t point(int pos) const;

    // set masked bits to true
    void point(int pos, chunk_t mask);

    // set masked bits to val
    void point(int pos, chunk_t val, chunk_t mask);

    bool canMove(chunk_t p) const;

    void firstApple();

    void moveTail();

    // bit 0: direction axis
    // bit 1: direction sign
    // bit 2: visited
    // bit 3: occupied
    static constexpr chunk_t RIGHT = 0b0000;
    static constexpr chunk_t UP = 0b0001;
    static constexpr chunk_t LEFT = 0b0010;
    static constexpr chunk_t DOWN = 0b0011;

    static constexpr chunk_t DIRECTION_MASK = 0b0011;
    static constexpr chunk_t MOVABLE_MASK = 0b1100;
    static constexpr chunk_t OCCUPIED_MASK = 0b0100;
    static constexpr chunk_t VISITED_MASK = 0b1000;
};

} // namespace Snake

#endif