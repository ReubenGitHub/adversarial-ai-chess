#include <stdio.h>
#include "chess_utils.hpp"
#include "lib/chess.hpp"
using namespace chess;

const std::string WHITE_SQUARE = "\u2B1C";
const std::string BLACK_SQUARE = "\u2B1B";
const std::string OCCUPIED_SQUARE = "\U0001F7E6";

std::string chessUtils::bitboardToString(const Bitboard& bitboard) {
    const std::uint64_t bitboard_bits = bitboard.getBits();
    std::string bitboard_string;
    std::string rank_str = "";
    std::string tile_str;
    bool tile_is_white = false;

    for (int i = 63; i >= 0; --i) {
        tile_str = (tile_is_white) ? WHITE_SQUARE : BLACK_SQUARE;
        if (bitboard_bits & (1ULL << i)) {
            rank_str = OCCUPIED_SQUARE + rank_str;
        } else {
            rank_str = tile_str + rank_str;
        }
        if (i % 8 == 0) {
            bitboard_string += rank_str + "\n";
            rank_str = "";
        } else {
            // Tiles alternate colour apart from when starting a new rank
            tile_is_white = !tile_is_white;
        }
    }
    return bitboard_string;
}
