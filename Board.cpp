#include "Board.h"
#include "Piece.h"
#include "Types.h"

// Default constructor initializes the board to the standard starting position
Board::Board()
    : turn("white"),
      castling("KQkq"),
      enpassant({NULL, NULL}),
      board(create_board()) {
    get_possible_actions();
}

// Constructor to initialize board with a given turn, castling rights, and custom board state
Board::Board(
    const std::string& input_turn,
    const std::string& input_castling,
    const std::array<std::array<char, 8>, 8>& simplify_board)
    : turn(input_turn),
      castling(input_castling),
      enpassant({NULL, NULL}),
      board(create_board(simplify_board)) {
    get_possible_actions();
}

// Constructor to initialize board with custom en passant state and board state
Board::Board(
    const std::string& input_turn,
    const std::string& input_castling,
    const std::array<int, 2>& input_enpassant,
    const std::array<std::array<char, 8>, 8>& simplify_board)
    : turn(input_turn),
      castling(input_castling),
      enpassant(input_enpassant),
      board(create_board(simplify_board)) {
    get_possible_actions();
}

// Compare two boards for equality
bool Board::operator==(const Board& other) const {
    if (this->turn != other.turn) return false;
    if (this->castling != other.castling) return false;
    if (this->enpassant != other.enpassant) return false;

    // Compare individual pieces on the board
    for (int i = 0; i < this->ROWS; i++) {
        for (int j = 0; j < this->COLS; j++) {
            // Check if either position is non-empty
            if (this->board[i][j] || other.board[i][j]) {
                // Check if mismatch is found
                if (*(this->board[i][j]) != *(other.board[i][j])) {
                    return false;
                }
            }
        }
    }
    return true;
}

// Output the current state of the board
std::ostream& operator<<(std::ostream& out, const Board& board_class) {
    out << "Turn: " << board_class.turn << ", ";
    out << "Castling: " << board_class.castling << ", ";
    out << "State: " << std::endl;

    for (int row = 0; row < board_class.ROWS; row++) {
        // Print row numbers
        std::cout << row + 1 << " ";
        for (int col = 0; col < board_class.COLS; col++) {
            if (board_class.board[row][col]) {
                // Display piece symbol
                std::cout << "[" << board_class.board[row][col]->symbol << "]";
            } else {
                // Empty square
                std::cout << "[ ]";
            }
        }
        std::cout << std::endl;
    }
    // Column labels
    std::cout << "   a  b  c  d  e  f  g  h " << std::endl;
    
    return out;
};

void Board::create_add_piece(
    const char& symbol,
    std::array<std::array<std::unique_ptr<Piece>, 8>, 8>& board,
    const int& row,
    const int& col
) {
    // Determine player color
    std:: string player = isupper(symbol) ? "white" : "black";

    // Create the appropriate piece based on the symbol
    if (toupper(symbol) == 'R') {
        board[row][col] = std::make_unique<Rook>(symbol, "rook", player, row, col);
    } else if (toupper(symbol) == 'N') {
        board[row][col] = std::make_unique<Knight>(symbol, "knight", player, row, col);
    } else if (toupper(symbol) == 'B') {
        board[row][col] = std::make_unique<Bishop>(symbol, "bishop", player, row, col);
    } else if (toupper(symbol) == 'K') {
        board[row][col] = std::make_unique<King>(symbol, "king", player, row, col);
    } else if (toupper(symbol) == 'Q') {
        board[row][col] = std::make_unique<Queen>(symbol, "queen", player, row, col);
    } else if (toupper(symbol) == 'P') {
        board[row][col] = std::make_unique<Pawn>(symbol, "pawn", player, row, col);
    }
}

// Create a board with the standard initial position
std::array<std::array<std::unique_ptr<Piece>, 8>, 8> Board::create_board() {
    std::array<std::array<char, 8>, 8> simplify_board = {{
        {'R', 'N', 'B', 'K', 'Q', 'B', 'N', 'R'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'r', 'n', 'b', 'k', 'q', 'b', 'n', 'r'}
    }};

    std::array<std::array<std::unique_ptr<Piece>, 8>, 8> board;

    // Populate the board with pieces
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (simplify_board[row][col] != ' ') {
                create_add_piece(simplify_board[row][col], board, row, col);
            }
        }
    }

    return board;
}

// Create a board from a custom configuration
std::array<std::array<std::unique_ptr<Piece>, 8>, 8> Board::create_board(const std::array<std::array<char, 8>, 8>& simplify_board) {
    std::array<std::array<std::unique_ptr<Piece>, 8>, 8> board;

    // Populate the board with pieces
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (simplify_board[row][col] != ' ') {
                create_add_piece(simplify_board[row][col], board, row, col);
            }
        }
    }

    return board;
}

// Calculate possible moves for the current player
void Board::get_possible_actions() {
    std::array<std::array<Piece::Actions, 8>, 8> possible_moves;
    bool end = true;
    // std::string winner;

    // Check moves for the opponent's pieces
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (board[row][col] && board[row][col]->player != turn) {
                board[row][col]->check_piece_possible_moves(*this);
            }
        }
    }

    // Check moves for the current player's pieces
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (board[row][col] && board[row][col]->player == turn) {
                board[row][col]->check_piece_possible_moves(*this);

                // Determine if the game should continue
                if (end) {
                    Piece::Actions curr_res = possible_moves[row][col];
                    if (curr_res.moves.size() || curr_res.attacks.size()) {
                        end = false;
                    }
                }
            }
        }
    }
}