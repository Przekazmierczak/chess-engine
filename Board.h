#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <array>
#include <algorithm>
#include <functional>
#include <random>
#include <span>

#include "Types.h"
#include "Piece.h"

class Piece;
class Game;

class Board {
public:
    // Dimensions of the chessboard
    const int ROWS = 8;
    const int COLS = 8;

    // Board state attributes
    PlayerColor turn; // Current player's turn (white = 0 or black = 1)
    std::string castling; // Castling rights (e.g., "KQkq")
    std::array<int, 2> enpassant; // Coordinates for en passant, if available

    // 2D array of unique pointers to Piece objects
    std::array<std::array<std::unique_ptr<Piece>, 8>, 8> board;

    // Chessboard analysis data
    PositionSet attacked_positions; // Positions attacked by the opponent
    PositionMap checkin_pieces; // Pieces causing a check on the king
    PositionMap pinned_pieces; // Pieces pinned to the king
    PositionSet active_pieces;

    // Board evaluation ratings and weights
    const int material_rating_weight = 50;
    const int attack_rating_weight = 3;
    const int protecting_rating_weight = 2;

    int white_material_rating;
    int black_material_rating;
    int white_attack_rating;
    int black_attack_rating;

    int final_rating; // Combined evaluation score for the board

    // Winner of the game, if determined
    Winner winner;

    // Constructors
    Board();
    Board(
        const PlayerColor& input_turn,
        const std::string& input_castling,
        const std::array<std::array<char, 8>, 8>& simplify_board
    );
    Board(
        const PlayerColor& input_turn,
        const std::string& input_castling,
        const std::array<int, 2>& input_enpassant,
        const std::array<std::array<char, 8>, 8>& simplify_board
    );

    // Custom destructor (not necessary)
    ~Board() {
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                board[i][j].reset();
            }
        }

        attacked_positions.clear();
        checkin_pieces.clear();
        pinned_pieces.clear();
        active_pieces.clear();
    }
    
    // Copy constructor
    Board(const Board& other_board);

    // Assignment operator
    Board& operator=(const Board& other_board);

    // Reset the board to its initial state
    void reset();

    // Comparison operators for equality and inequality'
    bool operator==(const Board& other) const;
    bool operator!=(const Board& other) const;

    // Overloaded output stream operator for the board
    friend std::ostream& operator<<(std::ostream& out, const Board& board_class);

    // Initialize the board with the standard starting position
    std::array<std::array<std::unique_ptr<Piece>, 8>, 8> create_board() const;

    // Initialize the board with a custom configuration
    std::array<std::array<std::unique_ptr<Piece>, 8>, 8> create_board(const std::array<std::array<char, 8>, 8>& simplify_board) const;

    // Calculate all possible moves for the current player
    void get_possible_actions();

    // Calculate the rating of the board
    void get_rating();

    // Print the board from the white player's perspective
    void print_white_perspective(
        const std::array<int, 2>& last_move_starting,
        const std::array<int, 2>& last_move_ending
    ) const;

    // Print the board with a focus on the current piece and its possible actions
    void print_white_perspective(
        const std::array<int, 2>& last_move_starting,
        const std::array<int, 2>& last_move_ending,
        const std::array<int, 2>& current_piece,
        const Actions& possible_actions
    ) const;

    void apply_move(Board& target_board, int old_row, int old_col, int new_row, int new_col, char symbol) const;

    // Execute a move on the board
    void make_action(int old_row, int old_col, int new_row, int new_col, char symbol);

    // Generate a new board after a move (AlfaBetaPruning)
    Board make_action_board(int old_row, int old_col, int new_row, int new_col, char symbol) const;

    // Generate AI move
    void computer_action(Game& game);

private:
    // Helper functions for creating pieces
    std::unique_ptr<Piece> create_piece(
        char symbol,
        int row,
        int col
    ) const;

    // Flattens all checking positions into an unordered set for faster lookups
    PositionSet flatting_checkin_pieces(
        const PositionMap& checkin_pieces
    ) const;

    // Handle en passant logic during a move
    void check_enpassant(int old_row, int old_col, int new_row);

    // Handle castling logic during a move
    void check_castling(int old_row, int old_col);

    // Promote a pawn and create the promoted piece
    std::unique_ptr<Piece> create_promoted_piece_player(int row, int col) const;

    // Select a random action from a set of best possible actions
    Action get_random_element(std::span<const Action> best_actions) const;
};

#endif