#include "Board.h"
#include "Piece.h"

#include "gtest/gtest.h"
#include <iostream>

namespace {
    TEST(CreateBoardMethod, Correct) {
        Board board;
        auto actual_board = board.create_board();
        
        std::array<std::array<std::unique_ptr<Piece>, 8>, 8> expected_board;

        expected_board[0][0] = std::make_unique<Rook>('R', "rook", "white", 0, 0);
        expected_board[0][1] = std::make_unique<Knight>('N', "knight", "white", 0, 1);
        expected_board[0][2] = std::make_unique<Bishop>('B', "bishop", "white", 0, 2);
        expected_board[0][3] = std::make_unique<King>('K', "king", "white", 0, 3);
        expected_board[0][4] = std::make_unique<Queen>('Q', "queen", "white", 0, 4);
        expected_board[0][5] = std::make_unique<Bishop>('B', "bishop", "white", 0, 5);
        expected_board[0][6] = std::make_unique<Knight>('N', "knight", "white", 0, 6);
        expected_board[0][7] = std::make_unique<Rook>('R', "rook", "white", 0, 7);

        expected_board[1][0] = std::make_unique<Pawn>('P', "pawn", "white", 1, 0);
        expected_board[1][1] = std::make_unique<Pawn>('P', "pawn", "white", 1, 1);
        expected_board[1][2] = std::make_unique<Pawn>('P', "pawn", "white", 1, 2);
        expected_board[1][3] = std::make_unique<Pawn>('P', "pawn", "white", 1, 3);
        expected_board[1][4] = std::make_unique<Pawn>('P', "pawn", "white", 1, 4);
        expected_board[1][5] = std::make_unique<Pawn>('P', "pawn", "white", 1, 5);
        expected_board[1][6] = std::make_unique<Pawn>('P', "pawn", "white", 1, 6);
        expected_board[1][7] = std::make_unique<Pawn>('P', "pawn", "white", 1, 7);

        expected_board[6][0] = std::make_unique<Pawn>('p', "pawn", "black", 6, 0);
        expected_board[6][1] = std::make_unique<Pawn>('p', "pawn", "black", 6, 1);
        expected_board[6][2] = std::make_unique<Pawn>('p', "pawn", "black", 6, 2);
        expected_board[6][3] = std::make_unique<Pawn>('p', "pawn", "black", 6, 3);
        expected_board[6][4] = std::make_unique<Pawn>('p', "pawn", "black", 6, 4);
        expected_board[6][5] = std::make_unique<Pawn>('p', "pawn", "black", 6, 5);
        expected_board[6][6] = std::make_unique<Pawn>('p', "pawn", "black", 6, 6);
        expected_board[6][7] = std::make_unique<Pawn>('p', "pawn", "black", 6, 7);

        expected_board[7][0] = std::make_unique<Rook>('r', "rook", "black", 7, 0);
        expected_board[7][1] = std::make_unique<Knight>('n', "knight", "black", 7, 1);
        expected_board[7][2] = std::make_unique<Bishop>('b', "bishop", "black", 7, 2);
        expected_board[7][3] = std::make_unique<King>('k', "king", "black", 7, 3);
        expected_board[7][4] = std::make_unique<Queen>('q', "queen", "black", 7, 4);
        expected_board[7][5] = std::make_unique<Bishop>('b', "bishop", "black", 7, 5);
        expected_board[7][6] = std::make_unique<Knight>('n', "knight", "black", 7, 6);
        expected_board[7][7] = std::make_unique<Rook>('r', "rook", "black", 7, 7);

        // Compare each element in the board manually
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (actual_board[i][j].get() || expected_board[i][j]) {
                    EXPECT_EQ(*(actual_board[i][j].get()), *(expected_board[i][j].get()));
                }
            }
        }

        // EXPECT_EQ(*(actual_board[i][j].get()), *(expected_board[i][j].get()));
    }

    TEST(CreateBoardMethodWithArg, Correct) {
        Board board;
        auto actual_board = board.create_board({{
            {'R', ' ', 'B', 'K', 'Q', ' ', 'N', ' '},
            {'P', ' ', 'P', ' ', 'P', ' ', 'P', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', 'p', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'p', ' ', 'p', ' ', 'p', ' ', 'p'},
            {' ', 'n', ' ', 'k', ' ', 'b', ' ', 'r'}
        }});
        
        std::array<std::array<std::unique_ptr<Piece>, 8>, 8> expected_board;

        expected_board[0][0] = std::make_unique<Rook>('R', "rook", "white", 0, 0);
        expected_board[0][2] = std::make_unique<Bishop>('B', "bishop", "white", 0, 2);
        expected_board[0][3] = std::make_unique<King>('K', "king", "white", 0, 3);
        expected_board[0][4] = std::make_unique<Queen>('Q', "queen", "white", 0, 4);
        expected_board[0][6] = std::make_unique<Knight>('N', "knight", "white", 0, 6);

        expected_board[1][0] = std::make_unique<Pawn>('P', "pawn", "white", 1, 0);
        expected_board[1][2] = std::make_unique<Pawn>('P', "pawn", "white",  1, 2);
        expected_board[1][4] = std::make_unique<Pawn>('P', "pawn", "white",  1, 4);
        expected_board[1][6] = std::make_unique<Pawn>('P', "pawn", "white",  1, 6);

        expected_board[4][4] = std::make_unique<Pawn>('p', "pawn", "black", 4, 4);
        expected_board[6][1] = std::make_unique<Pawn>('p', "pawn", "black",  6, 1);
        expected_board[6][3] = std::make_unique<Pawn>('p', "pawn", "black",  6, 3);
        expected_board[6][5] = std::make_unique<Pawn>('p', "pawn", "black",  6, 5);
        expected_board[6][7] = std::make_unique<Pawn>('p', "pawn", "black",  6, 7);

        expected_board[7][1] = std::make_unique<Knight>('n', "knight", "black",  7, 1);
        expected_board[7][3] = std::make_unique<King>('k', "king", "black",  7, 3);
        expected_board[7][5] = std::make_unique<Bishop>('b', "bishop", "black",  7, 5);
        expected_board[7][7] = std::make_unique<Rook>('r', "rook", "black",  7, 7);

        // Compare each element in the board manually
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (actual_board[i][j].get() || expected_board[i][j]) {
                    EXPECT_EQ(*(actual_board[i][j].get()), *(expected_board[i][j].get()));
                }
            }
        }
    }

    TEST(CreateBoardClass, Correct) {
        Board board;
        
        Board test_board;
        auto expected_board = test_board.create_board({{
            {'R', 'N', 'B', 'K', 'Q', 'B', 'N', 'R'},
            {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
            {'r', 'n', 'b', 'k', 'q', 'b', 'n', 'r'}
        }});

        EXPECT_EQ(board.turn, "white");
        EXPECT_EQ(board.castling, "KQkq");

        // Compare each element in the board manually
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (board.board[i][j].get() || expected_board[i][j]) {
                    EXPECT_EQ(*(board.board[i][j].get()), *(expected_board[i][j].get()));
                }
            }
        }
    }

    TEST(CreateBoardClassWithArgs, Correct) {
        Board board("black", "KQkq", {{
            {'R', ' ', 'B', 'K', 'Q', ' ', 'N', ' '},
            {'P', ' ', 'P', ' ', 'P', ' ', 'P', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', 'p', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'p', ' ', 'p', ' ', 'p', ' ', 'p'},
            {' ', 'n', ' ', 'k', ' ', 'b', ' ', 'r'}
        }});
        
        Board test_board;
        auto expected_board = test_board.create_board({{
            {'R', ' ', 'B', 'K', 'Q', ' ', 'N', ' '},
            {'P', ' ', 'P', ' ', 'P', ' ', 'P', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', 'p', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'p', ' ', 'p', ' ', 'p', ' ', 'p'},
            {' ', 'n', ' ', 'k', ' ', 'b', ' ', 'r'}
        }});

        EXPECT_EQ(board.turn, "black");
        EXPECT_EQ(board.castling, "KQkq");

        std::array<int, 2> enpassant = {NULL, NULL};
        EXPECT_EQ(board.enpassant, enpassant);

        // Compare each element in the board manually
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (board.board[i][j].get() || expected_board[i][j]) {
                    EXPECT_EQ(*(board.board[i][j].get()), *(expected_board[i][j].get()));
                }
            }
        }
    }

    TEST(CreateBoardClassWithArgs2, Correct) {
        Board board("black", "KQ__", {2, 2}, {{
            {'R', ' ', 'B', 'K', 'Q', ' ', 'N', ' '},
            {'P', ' ', 'P', ' ', 'P', ' ', 'P', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', 'p', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'p', ' ', 'p', ' ', 'p', ' ', 'p'},
            {' ', 'n', ' ', 'k', ' ', 'b', ' ', 'r'}
        }});
        
        Board test_board;
        auto expected_board = test_board.create_board({{
            {'R', ' ', 'B', 'K', 'Q', ' ', 'N', ' '},
            {'P', ' ', 'P', ' ', 'P', ' ', 'P', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', 'p', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'p', ' ', 'p', ' ', 'p', ' ', 'p'},
            {' ', 'n', ' ', 'k', ' ', 'b', ' ', 'r'}
        }});

        EXPECT_EQ(board.turn, "black");
        EXPECT_EQ(board.castling, "KQ__");

        std::array<int, 2> enpassant = {2, 2};
        EXPECT_EQ(board.enpassant, enpassant);

        // Compare each element in the board manually
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (board.board[i][j].get() || expected_board[i][j]) {
                    EXPECT_EQ(*(board.board[i][j].get()), *(expected_board[i][j].get()));
                }
            }
        }
    }
}

// Main function for GoogleTest
// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }