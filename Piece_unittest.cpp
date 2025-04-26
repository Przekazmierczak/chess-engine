#include "Board.h"
#include "Piece.h"

#include "gtest/gtest.h"
#include <iostream>

namespace {
    Piece::Actions create_expected_possible_actions(
        std::vector<std::array<int, 2>> moves,
        std::vector<std::array<int, 2>> attacks,
        bool promotion
    ) {
        Piece::Actions expected_result;

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
            {'r', ' ', ' ', ' ', ' ', ' ', ' ', 'k'}
        }});

        Board board_NN("black", "KQkq", {NULL, NULL}, {{
            {'K', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', 'p', 'P', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'r', ' ', ' ', ' ', ' ', ' ', ' ', 'k'}
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
    }
}

// Main function for GoogleTest
// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }