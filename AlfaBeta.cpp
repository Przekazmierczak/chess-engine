#include "AlfaBeta.h"

int AlfaBetaPruning::operator()(Board board, const int depth, int alpha, int beta) {
    board.get_possible_actions(); // Generate all possible moves for the current board state

    if (depth == 0) { // Base case: evaluate and return the board rating at maximum search depth
        board.get_rating();
        return board.final_rating;
    } else if (board.active_pieces.empty()) {  // No active pieces means checkmate or stalemate
        if (!board.checkin_pieces.empty()) {  // Checkmate situation
            if (board.turn == white) {
                return -100000 - depth * 50; // Losing score adjusted by depth for quicker mate
            } else {
                return 100000 + depth * 50; // Winning score adjusted by depth
            }
        } else { // Stalemate or draw
            return 0;
        }
    } else {
        int res;
        int curr_min_max = (board.turn == white) ? -100000 : 100000; // Initialize best score

        // Iterate over all active pieces and their possible moves
        for (const auto& position : board.active_pieces) {
            for (const auto& move : board.board[position[0]][position[1]]->possible_actions) {
                // Lambda to apply move and recursively evaluate resulting board
                auto make_and_minimax = [&](char promotion) {
                    return (*this)(board.make_action_board(position[0], position[1], move[0], move[1], promotion), depth - 1, alpha, beta);
                };

                if (board.turn == white) {
                    if (board.board[position[0]][position[1]]->possible_actions.promotion) {
                        // Evaluate all promotion options for white
                        res = std::max({
                            make_and_minimax('Q'),
                            make_and_minimax('N'),
                            make_and_minimax('B'),
                            make_and_minimax('R'),
                        });
                    } else {
                        res = make_and_minimax(' ');
                    }
                    curr_min_max = std::max(curr_min_max, res);
                    alpha = std::max(alpha, res);

                } else {
                    if (board.board[position[0]][position[1]]->possible_actions.promotion) {
                        // Evaluate all promotion options for black
                        res = std::min({
                            make_and_minimax('q'),
                            make_and_minimax('n'),
                            make_and_minimax('b'),
                            make_and_minimax('r')
                        });
                    } else {
                        res = make_and_minimax(' ');
                    }
                    curr_min_max = std::min(curr_min_max, res);
                    beta = std::min(beta, res);

                }

                // Alpha-beta pruning: cut off search if no better outcome can be found
                if (beta <= alpha) {
                    return curr_min_max;
                }
            }
        }
        // Return the best score found
        return curr_min_max;
    }
}