#include "Board.h"
#include "Piece.h"

#include "gtest/gtest.h"

namespace {
    TEST(CreateBoard, Correct) {
        Board board;
        auto actual_board = board.create_board();
        
        std::array<std::array<std::unique_ptr<Piece>, 8>, 8> expected_board;

        // Fill expected_board with nullptr or actual pieces if needed
        // for (int i = 0; i < 8; i++) {
        //     for (int j = 0; j < 8; j++) {
        //         expected_board[i][j] = nullptr; // Or std::make_unique<Piece>(...)
        //     }
        // }

        expected_board[0][0] = std::make_unique<Piece>('R', 0, 0);
        expected_board[0][1] = std::make_unique<Piece>('N', 0, 1);
        expected_board[0][2] = std::make_unique<Piece>('B', 0, 2);
        expected_board[0][3] = std::make_unique<Piece>('K', 0, 3);
        expected_board[0][4] = std::make_unique<Piece>('Q', 0, 4);
        expected_board[0][5] = std::make_unique<Piece>('B', 0, 5);
        expected_board[0][6] = std::make_unique<Piece>('N', 0, 6);
        expected_board[0][7] = std::make_unique<Piece>('R', 0, 7);

        expected_board[1][0] = std::make_unique<Piece>('P', 1, 0);
        expected_board[1][1] = std::make_unique<Piece>('P', 1, 1);
        expected_board[1][2] = std::make_unique<Piece>('P', 1, 2);
        expected_board[1][3] = std::make_unique<Piece>('P', 1, 3);
        expected_board[1][4] = std::make_unique<Piece>('P', 1, 4);
        expected_board[1][5] = std::make_unique<Piece>('P', 1, 5);
        expected_board[1][6] = std::make_unique<Piece>('P', 1, 6);
        expected_board[1][7] = std::make_unique<Piece>('P', 1, 7);

        expected_board[6][0] = std::make_unique<Piece>('p', 6, 0);
        expected_board[6][1] = std::make_unique<Piece>('p', 6, 1);
        expected_board[6][2] = std::make_unique<Piece>('p', 6, 2);
        expected_board[6][3] = std::make_unique<Piece>('p', 6, 3);
        expected_board[6][4] = std::make_unique<Piece>('p', 6, 4);
        expected_board[6][5] = std::make_unique<Piece>('p', 6, 5);
        expected_board[6][6] = std::make_unique<Piece>('p', 6, 6);
        expected_board[6][7] = std::make_unique<Piece>('p', 6, 7);

        expected_board[7][0] = std::make_unique<Piece>('r', 7, 0);
        expected_board[7][1] = std::make_unique<Piece>('n', 7, 1);
        expected_board[7][2] = std::make_unique<Piece>('b', 7, 2);
        expected_board[7][3] = std::make_unique<Piece>('k', 7, 3);
        expected_board[7][4] = std::make_unique<Piece>('q', 7, 4);
        expected_board[7][5] = std::make_unique<Piece>('b', 7, 5);
        expected_board[7][6] = std::make_unique<Piece>('n', 7, 6);
        expected_board[7][7] = std::make_unique<Piece>('r', 7, 7);

        // Compare each element in the board manually
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                EXPECT_EQ(actual_board[i][j].get(), expected_board[i][j].get());  
            }
        }
    }

    TEST(AdditionTest, HandlesPositiveInput) {
        EXPECT_EQ(2 + 2, 4);
    }
}

// Main function for GoogleTest
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}