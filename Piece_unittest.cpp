#include "Board.h"
#include "Piece.h"

#include "gtest/gtest.h"
#include <iostream>

namespace {
    Actions create_expected_possible_actions(
        std::vector<std::array<int, 2>> moves,
        std::vector<std::array<int, 2>> attacks,
        bool promotion
    ) {
        Actions expected_result;

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
        Board board_white("white", "____", {{
            {'R', ' ', ' ', ' ', 'K', ' ', ' ', ' '},
            {'P', ' ', ' ', ' ', ' ', ' ', 'P', ' '},
            {'N', ' ', ' ', ' ', 'P', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'B', ' ', 'Q', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', 'p', 'n', ' ', 'R'},
            {' ', ' ', ' ', 'p', 'N', ' ', ' ', 'p'},
            {'r', 'n', 'b', 'k', 'r', ' ', ' ', ' '}
        }});

        Board board_black("black", "____", {{
            {'R', ' ', ' ', ' ', 'K', ' ', ' ', ' '},
            {'P', ' ', ' ', ' ', ' ', ' ', 'P', ' '},
            {'N', ' ', ' ', ' ', 'P', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'B', ' ', 'Q', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', 'p', 'n', ' ', 'R'},
            {' ', ' ', ' ', 'p', 'N', ' ', ' ', 'p'},
            {'r', 'n', 'b', 'k', 'r', ' ', ' ', ' '}
        }});

        // WHITE
        EXPECT_EQ(
            board_white.board[1][0]->possible_actions,
            create_expected_possible_actions(
                {},
                {},
                false
            )
        );

        EXPECT_EQ(
            board_white.board[2][4]->possible_actions,
            create_expected_possible_actions(
                {{3, 4}},
                {},
                false
            )
        );

        EXPECT_EQ(
            board_white.board[1][6]->possible_actions,
            create_expected_possible_actions(
                {{2, 6}, {3, 6}},
                {},
                false
            )
        );

        EXPECT_EQ(
            board_white.board[0][0]->possible_actions,
            create_expected_possible_actions(
                {{0, 1}, {0, 2}, {0, 3}},
                {},
                false
            )
        );

        EXPECT_EQ(
            board_white.board[5][7]->possible_actions,
            create_expected_possible_actions(
                {{4, 7}, {3, 7}, {2, 7}, {1, 7}, {0, 7}, {5, 6}},
                {{6, 7}, {5, 5}},
                false
            )
        );

        EXPECT_EQ(
            board_white.board[2][0]->possible_actions,
            create_expected_possible_actions(
                {{0, 1}, {3, 2}, {1, 2}},
                {},
                false
            )
        );

        EXPECT_EQ(
            board_white.board[6][4]->possible_actions,
            create_expected_possible_actions(
                {{4, 5}, {7, 6}, {5, 6}, {5, 2}},
                {{7, 2}},
                false
            )
        );

        EXPECT_EQ(
            board_white.board[4][1]->possible_actions,
            create_expected_possible_actions(
                {{5, 2}, {5, 0}, {3, 2}, {2, 3}, {1, 4}, {0, 5}, {3, 0}},
                {{6, 3}},
                false
            )
        );

        EXPECT_EQ(
            board_white.board[4][3]->possible_actions,
            create_expected_possible_actions(
                {{5, 3}, {3, 3}, {2, 3}, {1, 3}, {0, 3}, {4, 4}, {4, 5}, {4, 6}, {4, 7}, {4, 2}, {5, 2}, {6, 1}, {3, 4}, {2, 5}, {3, 2}, {2, 1}},
                {{6, 3}, {5, 4}, {7, 0}},
                false
            )
        );

        EXPECT_EQ(
            board_white.board[0][4]->possible_actions,
            create_expected_possible_actions(
                {{1, 4}, {0, 5}, {0, 3}, {1, 5}, {1, 3}},
                {},
                false
            )
        );

        // BLACK
        EXPECT_EQ(
            board_black.board[6][3]->possible_actions,
            create_expected_possible_actions(
                {{5, 3}},
                {},
                false
            )
        );

        EXPECT_EQ(
            board_black.board[5][4]->possible_actions,
            create_expected_possible_actions(
                {{4, 4}},
                {{4, 3}},
                false
            )
        );

        EXPECT_EQ(
            board_black.board[6][7]->possible_actions,
            create_expected_possible_actions(
                {},
                {},
                false
            )
        );

        EXPECT_EQ(
            board_black.board[7][0]->possible_actions,
            create_expected_possible_actions(
                {{6, 0}, {5, 0}, {4, 0}, {3, 0}},
                {{2, 0}},
                false
            )
        );

        EXPECT_EQ(
            board_black.board[7][4]->possible_actions,
            create_expected_possible_actions(
                {{7, 5}, {7, 6}, {7, 7}},
                {{6, 4}},
                false
            )
        );

        EXPECT_EQ(
            board_black.board[7][1]->possible_actions,
            create_expected_possible_actions(
                {{5, 2}, {5, 0}},
                {},
                false
            )
        );

        EXPECT_EQ(
            board_black.board[5][5]->possible_actions,
            create_expected_possible_actions(
                {{7, 6}, {3, 6}, {3, 4}, {4, 7}},
                {{4 ,3}},
                false
            )
        );

        EXPECT_EQ(
            board_black.board[7][2]->possible_actions,
            create_expected_possible_actions(
                {{6, 1}, {5, 0}},
                {},
                false
            )
        );

        EXPECT_EQ(
            board_black.board[7][3]->possible_actions,
            create_expected_possible_actions(
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

        EXPECT_EQ(
            board.board[3][4]->possible_actions,
            create_expected_possible_actions(
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

        EXPECT_EQ(
            board.board[3][4]->possible_actions,
            create_expected_possible_actions(
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

        EXPECT_EQ(
            board.board[2][5]->possible_actions,
            create_expected_possible_actions(
                {},
                {{1, 6}},
                false
            )
        );
        
        EXPECT_EQ(
            board.board[3][2]->possible_actions,
            create_expected_possible_actions(
                {},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[5][2]->possible_actions,
            create_expected_possible_actions(
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

        EXPECT_EQ(
            board.board[4][0]->possible_actions,
            create_expected_possible_actions(
                {},
                {{3, 1}},
                false
            )
        );

        EXPECT_EQ(
            board.board[4][2]->possible_actions,
            create_expected_possible_actions(
                {{3, 2}},
                {{3, 1}},
                false
            )
        );

        EXPECT_EQ(
            board.board[3][7]->possible_actions,
            create_expected_possible_actions(
                {},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[7][5]->possible_actions,
            create_expected_possible_actions(
                {{3, 5}},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[1][2]->possible_actions,
            create_expected_possible_actions(
                {{3, 3}},
                {{3, 1}},
                false
            )
        );

        EXPECT_EQ(
            board.board[1][3]->possible_actions,
            create_expected_possible_actions(
                {{3, 5}},
                {{3, 1}},
                false
            )
        );

        EXPECT_EQ(
            board.board[7][1]->possible_actions,
            create_expected_possible_actions(
                {{3, 5}},
                {{3, 1}},
                false
            )
        );

        EXPECT_EQ(
            board.board[3][6]->possible_actions,
            create_expected_possible_actions(
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

        EXPECT_EQ(
            board.board[4][0]->possible_actions,
            create_expected_possible_actions(
                {},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[4][2]->possible_actions,
            create_expected_possible_actions(
                {},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[3][7]->possible_actions,
            create_expected_possible_actions(
                {},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[7][5]->possible_actions,
            create_expected_possible_actions(
                {},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[1][2]->possible_actions,
            create_expected_possible_actions(
                {},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[1][3]->possible_actions,
            create_expected_possible_actions(
                {},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[7][1]->possible_actions,
            create_expected_possible_actions(
                {},
                {},
                false
            )
        );

        EXPECT_EQ(
            board.board[3][6]->possible_actions,
            create_expected_possible_actions(
                {{2, 5}, {2, 7}, {4, 5}, {4, 7}},
                {},
                false
            )
        );
    }

    TEST(Castling12, Correct) {
        Board board_q("black", "___q", {{
            {' ', ' ', ' ', 'K', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'r', ' ', ' ', 'k', ' ', ' ', ' ', 'r'}
        }});

        Board board_k("black", "__k_", {{
            {' ', ' ', ' ', 'K', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'r', ' ', ' ', 'k', ' ', ' ', ' ', 'r'}
        }});

        EXPECT_EQ(
            board_q.board[7][3]->possible_actions,
            create_expected_possible_actions(
                {{7, 2}, {6, 2}, {6, 3}, {6, 4}, {7, 4}, {7, 5}},
                {},
                false
            )
        );

        EXPECT_EQ(
            board_k.board[7][3]->possible_actions,
            create_expected_possible_actions(
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

        EXPECT_EQ(
            board.board[7][3]->possible_actions,
            create_expected_possible_actions(
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

        EXPECT_EQ(
            board.board[7][3]->possible_actions,
            create_expected_possible_actions(
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

        EXPECT_EQ(
            board.board[7][3]->possible_actions,
            create_expected_possible_actions(
                {{7, 2}, {6, 2}, {6, 3}, {7, 4}},
                {},
                false
            )
        );
    }

    TEST(Enpassant12, Correct) {
        Board board_24("black", "KQkq", {2, 4}, {{
            {'K', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', 'p', 'P', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'k'}
        }});

        Board board_NN("black", "KQkq", {NULL, NULL}, {{
            {'K', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', 'p', 'P', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'k'}
        }});

        Board board_24_check("black", "KQkq", {2, 4}, {{
            {'K', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'R', ' ', ' ', ' ', ' ', ' ', ' ', 'k'},
            {' ', ' ', ' ', 'p', 'P', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        EXPECT_EQ(
            board_24.board[3][3]->possible_actions,
            create_expected_possible_actions(
                {{2, 3}},
                {{2, 4}},
                false
            )
        );
        
        EXPECT_EQ(
            board_NN.board[3][3]->possible_actions,
            create_expected_possible_actions(
                {{2, 3}},
                {},
                false
            )
        );

        EXPECT_EQ(
            board_24_check.board[3][3]->possible_actions,
            create_expected_possible_actions(
                {{2, 3}},
                {{2, 4}},
                false
            )
        );
    }

    TEST(AttackRatingKing, Correct) {
        Board board_white("white", "____", {{
            {'K', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'k'}
        }});

        Board board_black("black", "____", {{
            {'K', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'k'}
        }});

        EXPECT_EQ(board_white.white_attack_rating, 3);
        EXPECT_EQ(board_white.black_attack_rating, -3);

        EXPECT_EQ(board_black.white_attack_rating, 3);
        EXPECT_EQ(board_black.black_attack_rating, -3);
    }

    TEST(AttackRatingQueen, Correct) {
        Board board_white("white", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'Q', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', 'q', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        Board board_black("black", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'Q', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', 'q', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        EXPECT_EQ(board_white.white_attack_rating, 25);
        EXPECT_EQ(board_white.black_attack_rating, -25);

        EXPECT_EQ(board_black.white_attack_rating, 25);
        EXPECT_EQ(board_black.black_attack_rating, -25);
    }

    TEST(AttackRatingRook, Correct) {
        Board board_white("white", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'R', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', 'r', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        Board board_black("black", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'R', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', 'r', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        EXPECT_EQ(board_white.white_attack_rating, 14);
        EXPECT_EQ(board_white.black_attack_rating, -14);

        EXPECT_EQ(board_black.white_attack_rating, 14);
        EXPECT_EQ(board_black.black_attack_rating, -14);
    }

    TEST(AttackRatingBishop, Correct) {
        Board board_white("white", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'B', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', 'b', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        Board board_black("black", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'B', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', 'b', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        EXPECT_EQ(board_white.white_attack_rating, 11);
        EXPECT_EQ(board_white.black_attack_rating, -11);

        EXPECT_EQ(board_black.white_attack_rating, 11);
        EXPECT_EQ(board_black.black_attack_rating, -11);
    }
    
    TEST(AttackRatingKnight, Correct) {
        Board board_white("white", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'N', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', 'n', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        Board board_black("black", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'N', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', 'n', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        EXPECT_EQ(board_white.white_attack_rating, 8);
        EXPECT_EQ(board_white.black_attack_rating, -8);

        EXPECT_EQ(board_black.white_attack_rating, 8);
        EXPECT_EQ(board_black.black_attack_rating, -8);
    }

    TEST(AttackRatingPawn, Correct) {
        Board board_white("white", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'P', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', 'p', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        Board board_black("black", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'P', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', 'p', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        EXPECT_EQ(board_white.white_attack_rating, 2);
        EXPECT_EQ(board_white.black_attack_rating, -2);

        EXPECT_EQ(board_black.white_attack_rating, 2);
        EXPECT_EQ(board_black.black_attack_rating, -2);
    }

    TEST(AttackRatingKingPawnAttack, Correct) {
        Board board_white("white", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'p', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'K', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', 'k', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', 'P', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        Board board_black("black", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'p', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'K', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', 'k', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', 'P', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        EXPECT_EQ(board_white.white_attack_rating, 12);
        EXPECT_EQ(board_white.black_attack_rating, -12);

        EXPECT_EQ(board_black.white_attack_rating, 12);
        EXPECT_EQ(board_black.black_attack_rating, -12);
    }

    TEST(AttackRatingKingPawnCover, Correct) {
        Board board_white("white", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'P', ' ', ' ', ' ', ' ', ' '},
            {' ', 'K', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', 'k', ' '},
            {' ', ' ', ' ', ' ', ' ', 'p', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        Board board_black("black", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'P', ' ', ' ', ' ', ' ', ' '},
            {' ', 'K', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', 'k', ' '},
            {' ', ' ', ' ', ' ', ' ', 'p', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        EXPECT_EQ(board_white.white_attack_rating, 10);
        EXPECT_EQ(board_white.black_attack_rating, -10);

        EXPECT_EQ(board_black.white_attack_rating, 10);
        EXPECT_EQ(board_black.black_attack_rating, -10);
    }

    TEST(AttackRatingPawnAttackQueen, Correct) {
        Board board_white("white", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'P', ' ', ' ', ' ', ' ', ' ', ' '},
            {'q', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'Q'},
            {' ', ' ', ' ', ' ', ' ', ' ', 'p', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        Board board_black("black", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'P', ' ', ' ', ' ', ' ', ' ', ' '},
            {'q', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'Q'},
            {' ', ' ', ' ', ' ', ' ', ' ', 'p', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        EXPECT_EQ(board_white.white_attack_rating, 50);
        EXPECT_EQ(board_white.black_attack_rating, -50);

        EXPECT_EQ(board_black.white_attack_rating, 50);
        EXPECT_EQ(board_black.black_attack_rating, -50);
    }

    TEST(AttackRatingPawnCoverQueen, Correct) {
        Board board_white("white", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'P', ' ', ' ', ' ', ' ', ' ', ' '},
            {'Q', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'q'},
            {' ', ' ', ' ', ' ', ' ', ' ', 'p', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        Board board_black("black", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'P', ' ', ' ', ' ', ' ', ' ', ' '},
            {'Q', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'q'},
            {' ', ' ', ' ', ' ', ' ', ' ', 'p', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        EXPECT_EQ(board_white.white_attack_rating, 40);
        EXPECT_EQ(board_white.black_attack_rating, -40);

        EXPECT_EQ(board_black.white_attack_rating, 40);
        EXPECT_EQ(board_black.black_attack_rating, -40);
    }

    TEST(AttackRatingQueenAttackBishop, Correct) {
        Board board_white("white", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'Q', 'b', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', 'B', 'q'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        Board board_black("black", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'Q', 'b', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', 'B', 'q'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        EXPECT_EQ(board_white.white_attack_rating, 32);
        EXPECT_EQ(board_white.black_attack_rating, -32);

        EXPECT_EQ(board_black.white_attack_rating, 32);
        EXPECT_EQ(board_black.black_attack_rating, -32);
    }

    TEST(AttackRatingQueenCoverBishop, Correct) {
        Board board_white("white", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'Q', 'B', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', 'b', 'q'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        Board board_black("black", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'Q', 'B', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', 'b', 'q'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        EXPECT_EQ(board_white.white_attack_rating, 29);
        EXPECT_EQ(board_white.black_attack_rating, -29);

        EXPECT_EQ(board_black.white_attack_rating, 29);
        EXPECT_EQ(board_black.black_attack_rating, -29);
    }

    TEST(AttackRatingBishopAttackRook, Correct) {
        Board board_white("white", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'r', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'B', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', 'b', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'R'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        Board board_black("black", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'r', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'B', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', 'b', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'R'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        EXPECT_EQ(board_white.white_attack_rating, 37);
        EXPECT_EQ(board_white.black_attack_rating, -37);

        EXPECT_EQ(board_black.white_attack_rating, 37);
        EXPECT_EQ(board_black.black_attack_rating, -37);
    }

    TEST(AttackRatingBishopCoverRook, Correct) {
        Board board_white("white", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'R', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'B', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', 'b', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        Board board_black("black", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'R', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'B', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', 'b', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        EXPECT_EQ(board_white.white_attack_rating, 32);
        EXPECT_EQ(board_white.black_attack_rating, -32);

        EXPECT_EQ(board_black.white_attack_rating, 32);
        EXPECT_EQ(board_black.black_attack_rating, -32);
    }

    TEST(AttackRatingRookAttackKnight, Correct) {
        Board board_white("white", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'R', 'n', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', 'N', 'r'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        Board board_black("black", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'R', 'n', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', 'N', 'r'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        EXPECT_EQ(board_white.white_attack_rating, 22);
        EXPECT_EQ(board_white.black_attack_rating, -22);

        EXPECT_EQ(board_black.white_attack_rating, 22);
        EXPECT_EQ(board_black.black_attack_rating, -22);
    }

    TEST(AttackRatingRookCoverKnight, Correct) {
        Board board_white("white", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'R', 'N', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', 'n', 'r'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        Board board_black("black", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'R', 'N', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', 'n', 'r'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        EXPECT_EQ(board_white.white_attack_rating, 19);
        EXPECT_EQ(board_white.black_attack_rating, -19);

        EXPECT_EQ(board_black.white_attack_rating, 19);
        EXPECT_EQ(board_black.black_attack_rating, -19);
    }

    TEST(AttackRatingKnightAttackKing, Correct) {
        Board board_white("white", "____", {{
            {'k', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'N', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', 'n', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'K'}
        }});

        Board board_black("black", "____", {{
            {'k', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'N', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', 'n', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'K'}
        }});

        EXPECT_EQ(board_white.white_attack_rating, 3);
        EXPECT_EQ(board_white.black_attack_rating, -158);

        EXPECT_EQ(board_black.white_attack_rating, 158);
        EXPECT_EQ(board_black.black_attack_rating, -3);
    }

    TEST(AttackRatingKnightCoverKing, Correct) {
        Board board_white("white", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'N', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'K', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', 'k', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'n'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        Board board_black("black", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'N', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'K', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', 'k', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'n'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        EXPECT_EQ(board_white.white_attack_rating, 11);
        EXPECT_EQ(board_white.black_attack_rating, -11);

        EXPECT_EQ(board_black.white_attack_rating, 11);
        EXPECT_EQ(board_black.black_attack_rating, -11);
    }

    TEST(AttackRatingKnightPin, Correct) {
        Board board_white("white", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'K', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'N', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'r', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'k'}
        }});

        Board board_black("black", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'K', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'N', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'r', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'k'}
        }});

        EXPECT_EQ(board_white.white_attack_rating, 8);
        EXPECT_EQ(board_white.black_attack_rating, -22);

        EXPECT_EQ(board_black.white_attack_rating, 16);
        EXPECT_EQ(board_black.black_attack_rating, -22);
    }

    TEST(AttackRatingBishopPin, Correct) {
        Board board_white("white", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'K', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'B', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'r', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'k'}
        }});

        Board board_black("black", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'K', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'B', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'r', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'k'}
        }});

        EXPECT_EQ(board_white.white_attack_rating, 8);
        EXPECT_EQ(board_white.black_attack_rating, -21);

        EXPECT_EQ(board_black.white_attack_rating, 19);
        EXPECT_EQ(board_black.black_attack_rating, -21);
    }

    TEST(AttackRatingPawnPin, Correct) {
        Board board_white("white", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'K', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'P', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'r', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'k'}
        }});

        Board board_black("black", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'K', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'P', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'r', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'k'}
        }});

        EXPECT_EQ(board_white.white_attack_rating, 8);
        EXPECT_EQ(board_white.black_attack_rating, -16);

        EXPECT_EQ(board_black.white_attack_rating, 10);
        EXPECT_EQ(board_black.black_attack_rating, -16);
    }

    TEST(AttackRatingBlockKingMove, Correct) {
        Board board_white("white", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'K', ' ', 'p', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'n', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', 'b', ' ', ' ', ' ', ' '},
            {'r', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'k'}
        }});

        Board board_black("black", "____", {{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'K', ' ', 'p', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'n', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', 'b', ' ', ' ', ' ', ' '},
            {'r', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'k'}
        }});


        EXPECT_EQ(board_white.white_attack_rating, 2);
        EXPECT_EQ(board_white.black_attack_rating, -52);

        EXPECT_EQ(board_black.white_attack_rating, 8);
        EXPECT_EQ(board_black.black_attack_rating, -52);
    }

    TEST(AttackRatingBlockKingAttack, Correct) {
        Board board_white("white", "____", {{
            {'K', ' ', ' ', ' ', 'R', ' ', 'N', ' '},
            {' ', ' ', 'B', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', 'P', ' ', ' '},
            {' ', ' ', ' ', 'P', 'k', ' ', ' ', 'Q'},
            {' ', ' ', ' ', ' ', 'P', 'P', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        Board board_black("black", "____", {{
            {'K', ' ', ' ', ' ', 'R', ' ', 'N', ' '},
            {' ', ' ', 'B', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', 'P', ' ', ' '},
            {' ', ' ', ' ', 'P', 'k', ' ', ' ', 'Q'},
            {' ', ' ', ' ', ' ', 'P', 'P', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        EXPECT_EQ(board_white.black_attack_rating, -16);

        EXPECT_EQ(board_black.black_attack_rating, -1);
    }

    TEST(AttackRatingCheck, Correct) {
        Board board_white("white", "____", {{
            {'Q', ' ', ' ', ' ', ' ', ' ', ' ', 'R'},
            {' ', 'P', ' ', 'B', ' ', 'N', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', 'K', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', 'r', ' ', ' ', ' ', 'k'}
        }});

        EXPECT_EQ(board_white.white_attack_rating, 6);
        EXPECT_EQ(board_white.black_attack_rating, -160);
    }

    TEST(AttackRatingCheck2, Correct) {
        Board board_black("black", "____", {{
            {' ', 'K', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', 'n', 'b', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'Q', ' ', ' ', ' ', ' ', 'k', 'p'},
            {'p', ' ', 'p', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'q', ' ', ' ', ' ', 'r', ' ', ' '}
        }});

        EXPECT_EQ(board_black.white_attack_rating, 217);
        EXPECT_EQ(board_black.black_attack_rating, -145);
    }

    TEST(AttackRatingDoubleCheck, Correct) {
        Board board_black("black", "____", {{
            {' ', 'K', ' ', ' ', ' ', ' ', 'R', ' '},
            {' ', ' ', 'n', 'b', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'Q', ' ', ' ', ' ', ' ', 'k', 'p'},
            {'p', ' ', 'p', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'q', ' ', ' ', ' ', 'r', ' ', ' '}
        }});

        EXPECT_EQ(board_black.white_attack_rating, 374);
        EXPECT_EQ(board_black.black_attack_rating, -4);
    }

    TEST(AttackRatingEnpassant, Correct) {
        Board board_24("black", "____", {2, 4}, {{
            {'K', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', 'p', 'P', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'k'}
        }});

        Board board_NN("black", "____", {NULL, NULL}, {{
            {'K', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', 'p', 'P', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'k'}
        }});

        Board board_24_check("black", "____", {2, 4}, {{
            {'K', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'R', ' ', ' ', ' ', ' ', ' ', ' ', 'k'},
            {' ', ' ', ' ', 'p', 'P', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }});

        EXPECT_EQ(board_24.black_attack_rating, -7);

        EXPECT_EQ(board_NN.black_attack_rating, -5);
        
        EXPECT_EQ(board_24_check.black_attack_rating, -7);
    }

}

// Main function for GoogleTest
// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }