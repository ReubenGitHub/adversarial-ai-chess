#include <stdio.h>
#include "first-legal-move.hpp"
#include "lib/chess.hpp"
using namespace chess;

Move firstLegalMoveEngine::findMove(const Board &board) {
    Movelist legal_moves;
    movegen::legalmoves(legal_moves, board);
    Move first_move = legal_moves[0];
    return first_move;
}
