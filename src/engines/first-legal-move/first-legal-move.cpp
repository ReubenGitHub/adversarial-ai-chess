#include <stdio.h>
#include "../../lib/chess.hpp"
#include "first-legal-move.hpp"
using namespace chess;

Move firstLegalMoveEngine::findMove(const Board &board) {
    Movelist legal_moves;
    movegen::legalmoves(legal_moves, board);
    Move first_move = legal_moves[0];
    return first_move;
}
