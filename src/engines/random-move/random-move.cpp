#include <stdio.h>
#include "../../lib/chess.hpp"
#include "random-move.hpp"
using namespace chess;

Move randomMoveEngine::findMove(const Board &board) {
    Movelist legal_moves;
    movegen::legalmoves(legal_moves, board);
    Move random_move;
    size_t legal_moves_size = legal_moves.size();
    if (legal_moves_size > 0) {
        size_t idx = rand() % legal_moves_size;
        // printf("Random move engine picked move %zu of %zu possible moves\n", idx, legal_moves_size);
        random_move = legal_moves[idx];
    }
    return random_move;
}
