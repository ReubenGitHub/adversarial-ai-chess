#include <stdio.h>
#include "engines/first-legal-move/first-legal-move.hpp"
#include "engines/random-move/random-move.hpp"
#include "chess_utils/chess_utils.hpp"
#include "lib/chess.hpp"
using namespace chess;

const std::array<std::string, 3> PLAYERS = { "User", "First legal move engine", "Random engine" };

std::string getPromptForColor(const std::string& color) {
    std::string prompt;
    prompt += "Who should play for ";
    prompt += color;
    prompt += "? (";
    for (std::size_t i = 0; i < PLAYERS.size(); ++i) {
        prompt += std::to_string(i) + ": " + PLAYERS[i];
        if (i < PLAYERS.size() - 1) {
            prompt += ", ";
        }
    }
    prompt += "): ";
    return prompt;
}
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
    if (game_over.second == GameResult::LOSE) {
        if (board.sideToMove() == Color::WHITE) {
            result_str = "Black wins";
        } else {
            result_str = "White wins";
        }
    } else {
        result_str = "Draw";
    }

    printf("Game over! %s, due to %s.\n", result_str.c_str(), result_reason_str.c_str());
}

std::function<Move(const Board&)> getPlayerMoveGetter(std::size_t player_choice) {
    switch (player_choice) {
        case 0:
            return getUserMove;
        case 1:
            return firstLegalMoveEngine::findMove;
        case 2:
            return randomMoveEngine::findMove;
        default:
            printf("Invalid choice for player.\n");
            exit(1);
    }
}

std::pair<std::size_t, std::size_t> getPlayersFromUser() {
    std::size_t white_player_choice;
    std::size_t black_player_choice;

    printf(getPromptForColor("white").c_str());
    std::cin >> white_player_choice;

    printf(getPromptForColor("black").c_str());
    std::cin >> black_player_choice;

    return std::make_pair(white_player_choice, black_player_choice);
}

// Somehow write a function to find all the pawn attacks
// getPawnAttacks(const Board& board) {
//     attacks::pawn(Color c, Square sq)
// }

Bitboard getAllKnights(const Board& board, Color color) {
    return board.pieces(PieceType::KNIGHT, color);
}

int main() {
    Board board;
    printf("Initial board position: %s\n", board.getFen(true).c_str());

    std::string pgn_string;

    const auto [white_player_choice, black_player_choice] = getPlayersFromUser();

    auto getWhiteMove = getPlayerMoveGetter(white_player_choice);
    auto getBlackMove = getPlayerMoveGetter(black_player_choice);

    while (board.isGameOver().second == GameResult::NONE) {
        bool is_whites_move = (board.getFen(true).find(" w ") != std::string::npos);
        std::string turn_color = is_whites_move ? "White" : "Black";

        const Bitboard knights_bitboard = getAllKnights(board, is_whites_move ? Color::WHITE : Color::BLACK);
        const std::string knights_bitboard_string = chessUtils::bitboardToString(knights_bitboard);
        printf("%s's knights bits:\n%s", turn_color.c_str(), knights_bitboard_string.c_str());

        // Get turn's move
        Move this_turns_move = is_whites_move ? getWhiteMove(board) : getBlackMove(board);

        // Add move to PGN
        if (is_whites_move) {
            pgn_string += std::to_string(board.fullMoveNumber()) + ".";
        }
        pgn_string += " " + uci::moveToSan(board, this_turns_move);
        if (!is_whites_move) {
            pgn_string += "\n";
        }

        printf("%s's move: %s\n", turn_color.c_str(), uci::moveToUci(this_turns_move).c_str());

        board.makeMove(this_turns_move);
    }

    printGameResult(board);

    printf("Final board position: %s\n", board.getFen(true).c_str());

    printf("%s", pgn_string.c_str());

    return 0;
}
