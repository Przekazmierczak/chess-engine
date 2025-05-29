#include "AlfaBeta.h"

int AlfaBetaPruning::operator()(Board board, int depth, int alpha, int beta) {
    board.get_possible_actions();
    if (depth == 0) {
        board.get_rating();
        return board.final_rating;
    } else if (board.active_pieces.empty()) {
        if (!board.checkin_pieces.empty()) {
            if (board.turn == white) {
                return -100000 - depth;
            } else {
                return 100000 + depth;
            }
        } else {
            return 0;
        }
    } else {
        int res;
        int curr_min_max = (board.turn == white) ? -100000 : 100000;

        for (auto position : board.active_pieces) {
            for (auto move : board.board[position[0]][position[1]]->possible_actions) {
                auto make_and_minimax = [&](char promotion) {
                    return (*this)(board.make_action_board(position[0], position[1], move[0], move[1], promotion), depth - 1, alpha, beta);
                };

                if (board.turn == white) {
                    if (board.board[position[0]][position[1]]->possible_actions.promotion) {
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

                if (beta <= alpha) {
                    return curr_min_max;
                }
            }
        }
        return curr_min_max;
    }
}