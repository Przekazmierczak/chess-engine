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
    if (this->attacked_positions != other.attacked_positions) return false;
    if (this->checkin_pieces != other.checkin_pieces) return false;
    if (this->pinned_pieces != other.pinned_pieces) return false;
    if (this->active_pieces != other.active_pieces) return false;
    
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

bool Board::operator!=(const Board& other) const {
    return !(*this == other);
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
    std::cout << "   h  g  f  e  d  c  b  a " << std::endl;
    
    return out;
};

void Board::print_white_perspective() {
    std::cout << "Turn: " << turn << ", ";
    std::cout << "State: " << std::endl;

    for (int row = ROWS - 1; row >= 0; row--) {
        // Print row numbers
        std::cout << row + 1 << " ";
        for (int col = COLS - 1; col >= 0; col--) {
            if (board[row][col]) {
                // Display piece symbol
                std::cout << "[" << board[row][col]->symbol << "]";
            } else {
                // Empty square
                std::cout << "[ ]";
            }
        }
        std::cout << std::endl;
    }
    // Column labels
    std::cout << "   a  b  c  d  e  f  g  h " << std::endl;
};

std::unique_ptr<Piece> Board::create_piece(
    const char& symbol,
    const int& row,
    const int& col
) {
    // Determine player color
    std:: string player = isupper(symbol) ? "white" : "black";

    // Create the appropriate piece based on the symbol
    if (toupper(symbol) == 'R') {
        return std::make_unique<Rook>(symbol, "rook", player, row, col);
    } else if (toupper(symbol) == 'N') {
        return std::make_unique<Knight>(symbol, "knight", player, row, col);
    } else if (toupper(symbol) == 'B') {
        return std::make_unique<Bishop>(symbol, "bishop", player, row, col);
    } else if (toupper(symbol) == 'K') {
        return std::make_unique<King>(symbol, "king", player, row, col);
    } else if (toupper(symbol) == 'Q') {
        return std::make_unique<Queen>(symbol, "queen", player, row, col);
    } else {
        return std::make_unique<Pawn>(symbol, "pawn", player, row, col);
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
                board[row][col] = create_piece(simplify_board[row][col], row, col);
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
                board[row][col] = create_piece(simplify_board[row][col], row, col);
            }
        }
    }

    return board;
}

// Calculate possible moves for the current player
void Board::get_possible_actions() {
    attacked_positions = {};
    checkin_pieces = {};
    pinned_pieces = {};
    active_pieces = {};

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
            }
        }
    }

    if (!active_pieces.size()) {
        if (checkin_pieces.size()) {
            winner = (turn == "white") ? "black" : "white";
        } else {
            winner = "draw";
        }
    }
}

void Board::make_action(int old_row, int old_col, int new_row, int new_col) {
    if (board[old_row][old_col] &&
        board[old_row][old_col]->check_if_legal_action(new_row, new_col)
    ) {
        check_enpassant(old_row, old_col, new_row);

        if (castling != "____") {
            check_castling(old_row, old_col);
        }

        if (board[old_row][old_col]->possible_actions.promotion) {
            board[old_row][old_col] = create_promoted_piece_player(old_row, old_col);
        }
        
        board[new_row][new_col] = std::move(board[old_row][old_col]);
        
        board[new_row][new_col]->row = new_row;
        board[new_row][new_col]->column = new_col;
        
        turn = (turn == "white") ? "black" : "white";
        
        std::cout << "Correct action" << std::endl;
    } else {
        std::cout << "Wrong action" << std::endl;
    }
    
    get_possible_actions();
}

void Board::check_enpassant(int old_row, int old_col, int new_row) {
    if (board[old_row][old_col]->piece == "pawn" &&
        (old_row == 1 && new_row == 3) ||
        (old_row == 6 && new_row == 4)) {
            enpassant = {(old_row + new_row) / 2, old_col};
    }
}

void Board::check_castling(int old_row, int old_col) {
    std::unordered_map<
    std::array<int, 2>,
    std::vector<int>,
    PositionHash
    > kings_rooks_positions = {
        {{0, 3}, {0, 1}}, // The white king
        {{7, 3}, {2, 3}}, // The black king
        {{0, 0}, {0}}, // The white rook on the king side
        {{0, 7}, {1}}, // The white rook on the queen side
        {{7, 0}, {2}}, // The black rook on the king side
        {{7, 7}, {3}} // The black rook on the queen side
    };
    
    if (kings_rooks_positions.count({old_row, old_col})) {
        for (auto index : kings_rooks_positions[{old_row, old_col}]) {
            castling[index] = '_';
        }
    }
}

std::unique_ptr<Piece> Board::create_promoted_piece_player(int row, int col) {
    char symbol;
    std::cout << "Pick a promotion [Q, R, N, B, P]: ";
    std::cin >> symbol;

    return Board::create_piece(symbol, row, col);
}

void Board::computer_action() {
    std::array<int, 2> old_position = get_random_element(active_pieces);
    std::cout << "attacks: " << board[old_position[0]][old_position[1]]->possible_actions.attacks.size() << std::endl;
    std::cout << "moves: " << board[old_position[0]][old_position[1]]->possible_actions.moves.size() << std::endl;
    if (board[old_position[0]][old_position[1]]->possible_actions.attacks.size()) {
        std::array<int, 2> new_position = get_random_element(board[old_position[0]][old_position[1]]->possible_actions.attacks);
        make_action(old_position[0], old_position[1], new_position[0], new_position[1]);
        return;
    }

    if (board[old_position[0]][old_position[1]]->possible_actions.moves.size()) {
        std::array<int, 2> new_position = get_random_element(board[old_position[0]][old_position[1]]->possible_actions.moves);
        make_action(old_position[0], old_position[1], new_position[0], new_position[1]);
        return;
    }
    std::cerr << "Error: No valid moves or attacks for piece at position [" << old_position[0] << ", " << old_position[1] << "]." << std::endl;
}

// void Board::print_possible_actions() {
//     for (int row = 0; row < ROWS; row++) {
//         for (int col = 0; col < COLS; col++) {
//             if (board[row][col] && board[row][col]->player == turn) {
//                 print_position(row, col);
//                 std::cout << ": ";

//                 bool first = true;
//                 for (auto move : board[row][col]->possible_actions.moves) {
//                     if (first) {
//                         first = false;
//                     } else {
//                         std::cout << ", ";
//                     }

//                     print_position(move[0], move[1]);
//                 }

//                 for (auto move : board[row][col]->possible_actions.attacks) {
//                     if (first) {
//                         first = false;
//                     } else {
//                         std::cout << ", ";
//                     }

//                     print_position(move[0], move[1]);
//                 }
//                 std::cout << std::endl;
//             }
//         }
//     }
// }