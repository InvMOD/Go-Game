#include "board.hpp"
#include "../lib/includes.hpp"

static uint64_t zobristTable[BOARD_SIZE][BOARD_SIZE][3];
static bool zobristInitialized = false;

void Board::initZobrist() {
    if (zobristInitialized) return;
    std::mt19937_64 rng(0x123456789ABCDEF0ULL);
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            zobristTable[r][c][0] = 0; // Empty
            zobristTable[r][c][1] = rng(); // Black
            zobristTable[r][c][2] = rng(); // White
        }
    }
    zobristInitialized = true;
}

uint64_t Board::computeHash(const Stone b[BOARD_SIZE][BOARD_SIZE]) {
    initZobrist();
    uint64_t h = 0;
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            int stoneType = static_cast<int>(b[r][c]);
            if (stoneType != 0) {
                h ^= zobristTable[r][c][stoneType];
            }
        }
    }
    return h;
}

bool Board::isSuperko(uint64_t hash) const {
    for (const auto& state : history) {
        if (state.hash == hash) {
            return true;
        }
    }
    return false;
}