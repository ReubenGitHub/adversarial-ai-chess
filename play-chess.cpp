#include <stdio.h>
#include "engines/first-legal-move/first-legal-move.hpp"
#include "chess-lib/chess.hpp"
using namespace chess;

void printMoveList(const Board &board, const Movelist &moves) {
    printf("Listing all possible moves:\n");
    for (const auto& move : moves) {
        std::string uci_move = uci::moveToUci(move);
        std::string san_move = uci::moveToSan(board, move);
        std::string lan_move = uci::moveToLan(board, move);
        printf("UCI Move: %s, SAN Move: %s, LAN Move: %s\n", uci_move.c_str(), san_move.c_str(), lan_move.c_str());
    }
}

Move getUserMove(const Board &board) {
    std::string user_input;
    printf("Enter your move in UCI format: ");
    std::cin >> user_input;
    Move move = uci::uciToMove(board, user_input);

    if (move == Move::NO_MOVE) {
        printf("Invalid move format. Try again.\n");
        return getUserMove(board);
    }

    Movelist legal_moves;
    movegen::legalmoves(legal_moves, board);
    if (std::find(legal_moves.begin(), legal_moves.end(), move) == legal_moves.end()) {
        printf("Illegal move. Try again.\n");
        return getUserMove(board);
    }

    return move;
}

void printGameResult(const Board &board) {
    auto game_over = board.isGameOver();

    std::string result_reason_str;
    switch (game_over.first) {
        case GameResultReason::CHECKMATE:
            result_reason_str = "checkmate";
            break;
        case GameResultReason::STALEMATE:
            result_reason_str = "stalemate";
            break;
        case GameResultReason::INSUFFICIENT_MATERIAL:
            result_reason_str = "insufficient material";
            break;
        case GameResultReason::FIFTY_MOVE_RULE:
            result_reason_str = "50 move rule";
            break;
        case GameResultReason::THREEFOLD_REPETITION:
            result_reason_str = "3 fold repetition";
            break;
        default:
            result_reason_str = "unknown";
    }

    std::string result_str;
    if (game_over.second == GameResult::WIN) {
        if (board.sideToMove() == Color::WHITE) {
            result_str = "White wins";
        } else {
            result_str = "Black wins";
        }
    } else {
        result_str = "Draw";
    }

    printf("Game over! %s, due to %s.\n", result_str.c_str(), result_reason_str.c_str());
}

int main() {
    Board board;
    printf("Initial board position: %s\n", board.getFen(true).c_str());

    std::string pgn_string;

    bool play_against_engine;
    char choice;
    printf("Do you want to play against the engine? (y/n): ");
    std::cin >> choice;
    play_against_engine = (choice == 'y' || choice == 'Y');

    bool player_is_white = true;

    while (board.isGameOver().second == GameResult::NONE) {
        bool is_whites_move = (board.getFen(true).find(" w ") != std::string::npos);
        bool is_players_turn = (player_is_white == is_whites_move);

        // Get turn's move
        Move this_turns_move;
        if (play_against_engine) {
            if (is_players_turn) {
                // User's move
                this_turns_move = getUserMove(board);
            } else {
                // Engine's move
                this_turns_move = firstLegalMoveEngine::findMove(board);
            }
        } else {
            // Engine vs Engine
            this_turns_move = firstLegalMoveEngine::findMove(board);
        }

        // Add move to PGN
        if (is_whites_move) {
            pgn_string += std::to_string(board.fullMoveNumber()) + ".";
        }
        pgn_string += " " + uci::moveToSan(board, this_turns_move);
        if (!is_whites_move) {
            pgn_string += "\n";
        }

        std::string color = is_whites_move ? "White" : "Black";
        printf("%s's move: %s\n", color.c_str(), uci::moveToUci(this_turns_move).c_str());

        board.makeMove(this_turns_move);
    }

    printGameResult(board);

    printf("Final board position: %s\n", board.getFen(true).c_str());

    printf("%s", pgn_string.c_str());

    return 0;
}
