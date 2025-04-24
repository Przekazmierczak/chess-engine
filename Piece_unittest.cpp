#include "Board.h"
#include "Piece.h"

#include "gtest/gtest.h"
#include <iostream>

namespace {
    Piece::Result create_expected_result(
        std::vector<std::array<int, 2>> moves,
        std::vector<std::array<int, 2>> attacks,
        bool promotion
    ) {
        Piece::Result expected_result;

        for (auto move : moves) {
            expected_result.moves.insert(move);
        }
        for (auto attack : attacks) {
            expected_result.attacks.insert(attack);
        }
        expected_result.promotion = promotion;
        
        return expected_result;
    }

    TEST(TestBasicMovements, Correct) {
        Board board("white", "____", {{
            {'R', ' ', ' ', ' ', 'K', ' ', ' ', ' '},
            {'P', ' ', ' ', ' ', ' ', ' ', 'P', ' '},
            {'N', ' ', ' ', ' ', 'P', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'B', ' ', 'Q', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', 'p', 'n', ' ', 'R'},
            {' ', ' ', ' ', 'p', 'N', ' ', ' ', 'p'},
            {'r', 'n', 'b', 'k', 'r', ' ', ' ', ' '}
        }});

        PositionSet attacked_positions;
        PositionMap checkin_pieces;
        PositionMap pinned_pieces;

        // WHITE
        EXPECT_EQ(
            board.board[1][0]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[2][4]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{3, 4}},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[1][6]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{2, 6}, {3, 6}},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[0][0]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{0, 1}, {0, 2}, {0, 3}},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[5][7]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{4, 7}, {3, 7}, {2, 7}, {1, 7}, {0, 7}, {5, 6}},
                {{6, 7}, {5, 5}},
                false
            )
        );

        EXPECT_EQ(
            board.board[2][0]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{0, 1}, {3, 2}, {1, 2}},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[6][4]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{4, 5}, {7, 6}, {5, 6}, {5, 2}},
                {{7, 2}},
                false
            )
        );

        EXPECT_EQ(
            board.board[4][1]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{5, 2}, {5, 0}, {3, 2}, {2, 3}, {1, 4}, {0, 5}, {3, 0}},
                {{6, 3}},
                false
            )
        );

        EXPECT_EQ(
            board.board[4][3]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{5, 3}, {3, 3}, {2, 3}, {1, 3}, {0, 3}, {4, 4}, {4, 5}, {4, 6}, {4, 7}, {4, 2}, {5, 2}, {6, 1}, {3, 4}, {2, 5}, {3, 2}, {2, 1}},
                {{6, 3}, {5, 4}, {7, 0}},
                false
            )
        );

        EXPECT_EQ(
            board.board[0][4]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{1, 4}, {0, 5}, {0, 3}, {1, 5}, {1, 3}},
                {},
                false
            )
        );

        // BLACK
        board.turn = "black";
        EXPECT_EQ(
            board.board[6][3]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{5, 3}},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[5][4]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{4, 4}},
                {{4, 3}},
                false
            )
        );

        EXPECT_EQ(
            board.board[6][7]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[7][0]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{6, 0}, {5, 0}, {4, 0}, {3, 0}},
                {{2, 0}},
                false
            )
        );

        EXPECT_EQ(
            board.board[7][4]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{7, 5}, {7, 6}, {7, 7}},
                {{6, 4}},
                false
            )
        );

        EXPECT_EQ(
            board.board[7][1]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{5, 2}, {5, 0}},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[5][5]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{7, 6}, {3, 6}, {3, 4}, {4, 7}},
                {{4 ,3}},
                false
            )
        );

        EXPECT_EQ(
            board.board[7][2]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{6, 1}, {5, 0}},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[7][3]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{6, 2}},
                {{6, 4}},
                false
            )
        );

    }

    TEST(BlockTheKingMove, Correct) {
        Board board("black", "____", {{
            {'K', ' ', ' ', ' ', ' ', 'N', ' ', ' '},
            {' ', ' ', 'P', ' ', ' ', 'R', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', 'B', ' '},
            {' ', ' ', ' ', ' ', 'k', ' ', ' ', 'Q'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        PositionSet attacked_positions;
        PositionMap checkin_pieces;
        PositionMap pinned_pieces;

        // Update attacked_positions, checkin_pieces, pinned_pieces
        for (int row = 0; row < board.ROWS; row++) {
            for (int col = 0; col < board.COLS; col++) {
                if (board.board[row][col] && board.board[row][col]->player != board.turn) {
                    board.board[row][col]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces);
                }
            }
        }

        EXPECT_EQ(
            board.board[3][4]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{4, 3}},
                {},
                false
            )
        );
    }

    TEST(BlockTheKingAttack, Correct) {
        Board board("black", "____", {{
            {'K', ' ', ' ', ' ', 'R', ' ', 'N', ' '},
            {' ', ' ', 'B', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', 'P', ' ', ' '},
            {' ', ' ', ' ', 'P', 'k', ' ', ' ', 'Q'},
            {' ', ' ', ' ', ' ', 'P', 'P', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});
        
        PositionSet attacked_positions;
        PositionMap checkin_pieces;
        PositionMap pinned_pieces;

        // Update attacked_positions, checkin_pieces, pinned_pieces
        for (int row = 0; row < board.ROWS; row++) {
            for (int col = 0; col < board.COLS; col++) {
                if (board.board[row][col] && board.board[row][col]->player != board.turn) {
                    board.board[row][col]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces);
                }
            }
        }

        EXPECT_EQ(
            board.board[3][4]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{4, 3}},
                {},
                false
            )
        );
    }

    TEST(AbsolutePin, Correct) {
        Board board("black", "____", {{
            {'K', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', 'B', ' '},
            {' ', ' ', ' ', ' ', ' ', 'p', ' ', ' '},
            {'R', ' ', 'n', ' ', 'k', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'b', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'Q', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});
        
        PositionSet attacked_positions;
        PositionMap checkin_pieces;
        PositionMap pinned_pieces;

        // Update attacked_positions, checkin_pieces, pinned_pieces
        for (int row = 0; row < board.ROWS; row++) {
            for (int col = 0; col < board.COLS; col++) {
                if (board.board[row][col] && board.board[row][col]->player != board.turn) {
                    board.board[row][col]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces);
                }
            }
        }

        EXPECT_EQ(
            board.board[2][5]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {},
                {{1, 6}},
                false
            )
        );
        
        EXPECT_EQ(
            board.board[3][2]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[5][2]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{6, 1}, {4, 3}},
                {{7, 0}},
                false
            )
        );
    }

    TEST(Check, Correct) {
        Board board("black", "____", {{
            {'K', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'n', 'b', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'Q', ' ', ' ', ' ', ' ', 'k', 'p'},
            {'p', ' ', 'p', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'q', ' ', ' ', ' ', 'r', ' ', ' '}
        }});

        PositionSet attacked_positions;
        PositionMap checkin_pieces;
        PositionMap pinned_pieces;

        // Update attacked_positions, checkin_pieces, pinned_pieces
        for (int row = 0; row < board.ROWS; row++) {
            for (int col = 0; col < board.COLS; col++) {
                if (board.board[row][col] && board.board[row][col]->player != board.turn) {
                    board.board[row][col]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces);
                }
            }
        }

        EXPECT_EQ(
            board.board[4][0]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {},
                {{3, 1}},
                false
            )
        );

        EXPECT_EQ(
            board.board[4][2]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{3, 2}},
                {{3, 1}},
                false
            )
        );

        EXPECT_EQ(
            board.board[3][7]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[7][5]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{3, 5}},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[1][2]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{3, 3}},
                {{3, 1}},
                false
            )
        );

        EXPECT_EQ(
            board.board[1][3]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{3, 5}},
                {{3, 1}},
                false
            )
        );

        EXPECT_EQ(
            board.board[7][1]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{3, 5}},
                {{3, 1}},
                false
            )
        );

        EXPECT_EQ(
            board.board[3][6]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{2, 5}, {2, 6}, {2, 7}, {4, 5}, {4, 6}, {4, 7}},
                {},
                false
            )
        );
    }

    TEST(DoubleCheck, Correct) {
        Board board("black", "____", {{
            {'K', ' ', ' ', ' ', ' ', ' ', 'R', ' '},
            {' ', ' ', 'n', 'b', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'Q', ' ', ' ', ' ', ' ', 'k', 'p'},
            {'p', ' ', 'p', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'q', ' ', ' ', ' ', 'r', ' ', ' '}
        }});

        PositionSet attacked_positions;
        PositionMap checkin_pieces;
        PositionMap pinned_pieces;

        // Update attacked_positions, checkin_pieces, pinned_pieces
        for (int row = 0; row < board.ROWS; row++) {
            for (int col = 0; col < board.COLS; col++) {
                if (board.board[row][col] && board.board[row][col]->player != board.turn) {
                    board.board[row][col]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces);
                }
            }
        }

        EXPECT_EQ(
            board.board[4][0]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[4][2]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[3][7]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[7][5]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[1][2]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[1][3]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[7][1]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[3][6]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{2, 5}, {2, 7}, {4, 5}, {4, 7}},
                {},
                false
            )
        );
    }

    TEST(Castling12, Correct) {
        Board board("black", "___q", {{
            {' ', ' ', ' ', 'K', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'r', ' ', ' ', 'k', ' ', ' ', ' ', 'r'}
        }});

        PositionSet attacked_positions;
        PositionMap checkin_pieces;
        PositionMap pinned_pieces;

        // Update attacked_positions, checkin_pieces, pinned_pieces
        for (int row = 0; row < board.ROWS; row++) {
            for (int col = 0; col < board.COLS; col++) {
                if (board.board[row][col] && board.board[row][col]->player != board.turn) {
                    board.board[row][col]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces);
                }
            }
        }

        EXPECT_EQ(
            board.board[7][3]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{7, 2}, {6, 2}, {6, 3}, {6, 4}, {7, 4}, {7, 5}},
                {},
                false
            )
        );

        board.castling = "__k_";
        EXPECT_EQ(
            board.board[7][3]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{7, 2}, {6, 2}, {6, 3}, {6, 4}, {7, 4}, {7, 1}},
                {},
                false
            )
        );
    }

    TEST(Castling3, Correct) {
        Board board("black", "KQkq", {{
            {' ', ' ', ' ', 'K', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'r', 'n', ' ', 'k', ' ', ' ', 'n', 'r'}
        }});

        PositionSet attacked_positions;
        PositionMap checkin_pieces;
        PositionMap pinned_pieces;

        // Update attacked_positions, checkin_pieces, pinned_pieces
        for (int row = 0; row < board.ROWS; row++) {
            for (int col = 0; col < board.COLS; col++) {
                if (board.board[row][col] && board.board[row][col]->player != board.turn) {
                    board.board[row][col]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces);
                }
            }
        }

        EXPECT_EQ(
            board.board[7][3]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{7, 2}, {6, 2}, {6, 3}, {6, 4}, {7, 4}},
                {},
                false
            )
        );
    }

    TEST(Castling4, Correct) {
        Board board("black", "KQkq", {{
            {' ', ' ', ' ', 'K', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', 'B', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'r', ' ', ' ', 'k', ' ', ' ', ' ', 'r'}
        }});

        PositionSet attacked_positions;
        PositionMap checkin_pieces;
        PositionMap pinned_pieces;

        // Update attacked_positions, checkin_pieces, pinned_pieces
        for (int row = 0; row < board.ROWS; row++) {
            for (int col = 0; col < board.COLS; col++) {
                if (board.board[row][col] && board.board[row][col]->player != board.turn) {
                    board.board[row][col]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces);
                }
            }
        }

        EXPECT_EQ(
            board.board[7][3]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{7, 2}, {6, 2}, {6, 3}, {7, 4}},
                {},
                false
            )
        );
    }

    TEST(Castling5, Correct) {
        Board board("black", "KQkq", {{
            {' ', ' ', ' ', 'K', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'R', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', 'B', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'r', ' ', ' ', 'k', ' ', ' ', ' ', 'r'}
        }});

        PositionSet attacked_positions;
        PositionMap checkin_pieces;
        PositionMap pinned_pieces;

        // Update attacked_positions, checkin_pieces, pinned_pieces
        for (int row = 0; row < board.ROWS; row++) {
            for (int col = 0; col < board.COLS; col++) {
                if (board.board[row][col] && board.board[row][col]->player != board.turn) {
                    board.board[row][col]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces);
                }
            }
        }

        EXPECT_EQ(
            board.board[7][3]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{7, 2}, {6, 2}, {6, 3}, {7, 4}},
                {},
                false
            )
        );
    }

    TEST(Enpassant12, Correct) {
        Board board("black", "KQkq", {2, 4}, {{
            {'K', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', 'p', 'P', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'r', ' ', ' ', ' ', ' ', ' ', ' ', 'k'}
        }});

        PositionSet attacked_positions;
        PositionMap checkin_pieces;
        PositionMap pinned_pieces;

        // Update attacked_positions, checkin_pieces, pinned_pieces
        for (int row = 0; row < board.ROWS; row++) {
            for (int col = 0; col < board.COLS; col++) {
                if (board.board[row][col] && board.board[row][col]->player != board.turn) {
                    board.board[row][col]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces);
                }
            }
        }

        EXPECT_EQ(
            board.board[3][3]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{2, 3}},
                {{2, 4}},
                false
            )
        );
        
        board.enpassant = {NULL, NULL};
        EXPECT_EQ(
            board.board[3][3]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{2, 3}},
                {},
                false
            )
        );
    }
}

// Main function for GoogleTest
// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }