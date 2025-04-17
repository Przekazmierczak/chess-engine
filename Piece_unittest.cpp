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
        Board board;
        
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                board.board[i][j] = nullptr;
            }
        }

        // ("pawn", "white", (1, 0)),
        board.board[1][0] = std::make_unique<Piece>('P', 1, 0);
        // ("pawn", "white", (2, 4)),
        board.board[2][4] = std::make_unique<Piece>('P', 2, 4);
        // ("pawn", "white", (1, 6)),
        board.board[1][6] = std::make_unique<Piece>('P', 1, 6);
        // ("pawn", "black", (6, 3)),
        board.board[6][3] = std::make_unique<Piece>('p', 6, 3);
        // ("pawn", "black", (5, 4)),
        board.board[5][4] = std::make_unique<Piece>('p', 5, 4);
        // ("pawn", "black", (6, 7)),
        board.board[6][7] = std::make_unique<Piece>('p', 6, 7);
        // ("rook", "white", (0, 0)),
        board.board[0][0] = std::make_unique<Piece>('R', 0, 0);
        // ("rook", "white", (5, 7)),
        board.board[5][7] = std::make_unique<Piece>('R', 5, 7);
        // ("rook", "black", (7, 0)),
        board.board[7][0] = std::make_unique<Piece>('r', 7, 0);
        // ("rook", "black", (7, 4)),
        board.board[7][4] = std::make_unique<Piece>('r', 7, 4);
        // ("knight", "white", (2, 0)),
        board.board[2][0] = std::make_unique<Piece>('N', 2, 0);
        // ("knight", "white", (6, 4)),
        board.board[6][4] = std::make_unique<Piece>('N', 6, 4);
        // ("knight", "black", (7, 1)),
        board.board[7][1] = std::make_unique<Piece>('n', 7, 1);
        // ("knight", "black", (5, 5)),
        board.board[5][5] = std::make_unique<Piece>('n', 5, 5);
        // ("bishop", "white", (4, 1)),
        board.board[4][1] = std::make_unique<Piece>('B', 4, 1);
        // ("bishop", "black", (7, 2)),
        board.board[7][2] = std::make_unique<Piece>('b', 7, 2);
        // ("queen", "white", (4, 3)),
        board.board[4][3] = std::make_unique<Piece>('Q', 4, 3);
        // ("king", "white", (0, 4)),
        board.board[0][4] = std::make_unique<Piece>('K', 0, 4);
        // ("king", "black", (7, 3))
        board.board[7][3] = std::make_unique<Piece>('k', 7, 3);

        PositionSet attacked_positions;
        PositionMap checkin_pieces;
        PositionMap pinned_pieces;


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

        Piece::Result result = board.board[0][4]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces);

        // for (auto move : result.moves) {
        //     std::cout << move[0] << "-" << move[1] << std::endl;
        // }

        EXPECT_EQ(
            board.board[0][4]->check_piece_possible_moves(board, attacked_positions, checkin_pieces, pinned_pieces),
            create_expected_result(
                {{1, 4}, {0, 5}, {0, 3}, {1, 5}, {1, 3}, {0, 2}},
                {},
                false
            )
        );

    }



    TEST(TestTest, HandlesPositiveInput) {
        EXPECT_EQ(1, 1);
    }
}

// Main function for GoogleTest
// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }