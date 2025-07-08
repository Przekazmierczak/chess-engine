#include "Board.h"
#include "Piece.h"
#include "Types.h"
#include "Game.h"

// Default constructor initializes the board to the standard starting position
Board::Board()
    : turn(white),
      castling("KQkq"),
      enpassant({8, 8}),
      board(create_board()),
      winner(notFinished) {
    get_possible_actions();
}

// Constructor to initialize board with a given turn, castling rights, and custom board state
Board::Board(
    const PlayerColor& input_turn,
    const std::string& input_castling,
    const std::array<std::array<char, 8>, 8>& simplify_board)
    : turn(input_turn),
      castling(input_castling),
      enpassant({8, 8}),
      board(create_board(simplify_board)),
      winner(notFinished) {
    get_possible_actions();
}

// Constructor to initialize board with custom en passant state and board state
Board::Board(
    const PlayerColor& input_turn,
    const std::string& input_castling,
    const std::array<int, 2>& input_enpassant,
    const std::array<std::array<char, 8>, 8>& simplify_board)
    : turn(input_turn),
      castling(input_castling),
      enpassant(input_enpassant),
      board(create_board(simplify_board)),
      winner(notFinished) {
    get_possible_actions();
}

// Copy constructor
Board::Board(const Board& other_board) {
    turn = other_board.turn;
    castling = other_board.castling;
    enpassant = other_board.enpassant;
    winner = other_board.winner;

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (other_board.board[row][col]) {
                board[row][col] = create_piece(other_board.board[row][col]->symbol, row, col);
            }
        }
    }
}

// Assignment operator
Board& Board::operator=(const Board& other_board) {
    if (this == &other_board) return *this;

    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            board[row][col].reset();
        }
    }

    turn = other_board.turn;
    castling = other_board.castling;
    enpassant = other_board.enpassant;
    winner = other_board.winner;

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (other_board.board[row][col]) {
                board[row][col] = create_piece(other_board.board[row][col]->symbol, row, col);
            } else {
                board[row][col] = nullptr;
            }
        }
    }
    get_possible_actions();

    return *this;
}

// Reset the board to its initial state
void Board::reset() {
    turn = white;
    castling = "KQkq";
    enpassant = {8, 8};
    board = create_board();
    winner = notFinished;
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
        out << row + 1 << " ";
        for (int col = 0; col < board_class.COLS; col++) {
            if (board_class.board[row][col]) {
                // Display piece symbol
                out << "[" << board_class.board[row][col]->symbol << "]";
            } else {
                // Empty square
                out << "[ ]";
            }
        }
        out << std::endl;
    }
    // Column labels
    out << "   h  g  f  e  d  c  b  a " << std::endl;
    
    return out;
};

// Create a board with the standard initial position
std::array<std::array<std::unique_ptr<Piece>, 8>, 8> Board::create_board() const {
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
std::array<std::array<std::unique_ptr<Piece>, 8>, 8> Board::create_board(
    const std::array<std::array<char, 8>, 8>& simplify_board
) const {
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
    for (auto &current_row : board) {
        for (auto &current_piece : current_row) {
            if (current_piece && current_piece->player != turn) {
                current_piece->possible_actions.reset();
                current_piece->check_piece_possible_moves_opponent(*this);
            }
        }
    }

    PositionSet checking_positions = flatting_checkin_pieces(checkin_pieces);

    // Check moves for the current player's pieces
    for (auto &current_row : board) {
        for (auto &current_piece : current_row) {
            if (current_piece && current_piece->player == turn) {
                current_piece->possible_actions.reset();
                current_piece->check_piece_possible_moves_active_player(*this, checking_positions);
            }
        }
    }

    if (!active_pieces.size()) {
        if (checkin_pieces.size()) {
            winner = (turn == white) ? blackWin : whiteWin;
        } else {
            winner = draw;
        }
    }
}

// Calculate the rating of the board
void Board::get_rating() {
    white_material_rating = 0;
    black_material_rating = 0;
    white_attack_rating = 0;
    black_attack_rating = 0;

    // Check moves for the opponent's pieces
    for (auto &current_row : board) {
        for (auto &current_piece : current_row) {
            if (current_piece && current_piece->player != turn) {
                current_piece->update_rating_opponent(*this);

                if (current_piece->piece != king) {
                    if (current_piece->player == white) {
                        white_material_rating += material_rating_weight * current_piece->get_value();
                    } else {
                        black_material_rating -= material_rating_weight * current_piece->get_value();
                    }
                }
            }
        }
    }

    PositionSet checking_positions = flatting_checkin_pieces(checkin_pieces);

    // Check moves for the current player's pieces
    for (auto &current_row : board) {
        for (auto &current_piece : current_row) {
            if (current_piece && current_piece->player == turn) {
                current_piece->update_rating_active_player(*this, checking_positions);
                
                if (current_piece->piece != king) {
                    if (current_piece->player == white) {
                        white_material_rating += material_rating_weight * current_piece->get_value();
                    } else {
                        black_material_rating -= material_rating_weight * current_piece->get_value();
                    }
                }
            }
        }
    }

    final_rating = white_material_rating + white_attack_rating + black_material_rating + black_attack_rating;
}

// Print the board from the white player's perspective
void Board::print_white_perspective(
    const std::array<int, 2>& last_move_starting,
    const std::array<int, 2>& last_move_ending
) const {
    // Helper to print a single square with ANSI colors
    auto print_square = [](std::string piece_color, std::string background_color, char mark, char piece) {
        std::cout << "\033[1;" << piece_color << ";" << background_color << "m " << mark << piece << "  \033[0m";
    };
    
    // Determine colors based on piece presence, color, square color, and last move highlight
    auto get_colors = [](bool has_piece, bool is_white_piece, bool is_light_square, bool is_last_move) {
        std::string piece_color = is_white_piece ? "34" : "35"; // Blue for white, magenta for black
        std::string background_color;
        
        if (is_last_move) {
            background_color = is_light_square ? "106" : "46"; // Highlight last move squares
        } else {
            background_color = is_light_square ? "47" : "100"; // Default light or dark square
        }
        return std::make_pair(piece_color, background_color);
    };
    
    for (int row = ROWS - 1; row >= 0; row--) {
        // Print row numbers
        std::cout << row + 1 << "  ";
        for (int col = COLS - 1; col >= 0; col--) {
            bool is_light_square = (row + col) % 2 == 0;
            bool has_piece = (board[row][col] != nullptr);
            char piece_symbol = has_piece ? board[row][col]->symbol : ' ';
            bool is_white_piece = has_piece && board[row][col]->player == white;
            bool is_last_move = (
                std::array<int, 2>{row, col} == last_move_starting ||
                std::array<int, 2>{row, col} == last_move_ending
            );
    
            auto [piece_color, background_color] = get_colors(has_piece, is_white_piece, is_light_square, is_last_move);
            if (has_piece) {
                print_square(piece_color, background_color, is_white_piece ? '^' : ' ', piece_symbol);
            } else {
                print_square("", background_color, ' ', ' ');
            }
        }
        std::cout << std::endl << "   ";
        
        // Print empty row below with appropriate colors
        for (int col = COLS - 1; col >= 0; col--) {
            bool is_light_square = (row + col) % 2 == 0;
            bool is_last_move = (
                std::array<int, 2>{row, col} == last_move_starting ||
                std::array<int, 2>{row, col} == last_move_ending
            );
            std::string background_color;
            if (is_last_move) {
                background_color = is_light_square ? "106" : "46";
            } else {
                background_color = (is_light_square ? "47" : "100");
            }
    
            print_square("", background_color, ' ', ' ');
        }
        std::cout << std::endl;
    }
    
    // Column labels
    std::cout << "     a    b    c    d    e    f    g    h  " << std::endl;
};

// Print the board with a focus on the current piece and its possible actions
void Board::print_white_perspective(
    const std::array<int, 2>& last_move_starting,
    const std::array<int, 2>& last_move_ending,
    const std::array<int, 2>& current_piece,
    const Actions& possible_actions
) const {
    // Helper to print a single square with ANSI colors
    auto print_square = [](std::string piece_color, std::string background_color, char mark, char piece) {
        std::cout << "\033[1;" << piece_color << ";" << background_color << "m " << mark << piece << "  \033[0m";
    };
    
    // Determine colors considering selection, attacks, moves, and last move highlights
    auto get_colors = [](
        bool has_piece,
        bool is_white_piece,
        bool is_selected,
        bool is_move,
        bool is_attack,
        bool is_light_square,
        bool is_last_move
    ) {
        std::string piece_color = is_white_piece ? "34" : "35"; // Blue for white, magenta for black
        std::string background_color;
        
        if (has_piece) {
            if (is_selected) {
                background_color = is_light_square ? "103" : "43"; // Light or dark selected square
            } else if (is_attack) {
                background_color = is_light_square ? "101" : "41"; // Light or dark attack square
            } else if (is_last_move) {
                background_color = is_light_square ? "106" : "46"; // Cyan highlight for last move squares
            } else {
                background_color = is_light_square ? "47" : "100"; // Default light or dark square
            }
        } else {
            if (is_move) {
                background_color = is_light_square ? "102" : "42"; // Green highlight for possible move squares
            } else if (is_last_move) {
                background_color = is_light_square ? "106" : "46"; // Cyan highlight for last move squares
            } else {
                background_color = is_light_square ? "47" : "100"; // Default light or dark square
            }
        }
        return std::make_pair(piece_color, background_color);
    };
    
    for (int row = ROWS - 1; row >= 0; row--) {
        // Print row numbers
        std::cout << row + 1 << "  ";
        for (int col = COLS - 1; col >= 0; col--) {
            bool is_light_square = (row + col) % 2 == 0;
            bool is_selected = (row == current_piece[0] && col == current_piece[1]);
            bool is_attack = possible_actions.attacks.count({row, col});
            bool is_move = possible_actions.moves.count({row, col});
            bool has_piece = (board[row][col] != nullptr);
            char piece_symbol = has_piece ? board[row][col]->symbol : ' ';
            bool is_white_piece = has_piece && board[row][col]->player == white;
            bool is_last_move = (
                std::array<int, 2>{row, col} == last_move_starting ||
                std::array<int, 2>{row, col} == last_move_ending
            );
    
            auto [piece_color, background_color] = get_colors(
                has_piece,
                is_white_piece,
                is_selected,
                is_move,
                is_attack,
                is_light_square,
                is_last_move
            );
            if (has_piece) {
                print_square(piece_color, background_color, is_white_piece ? '^' : ' ', piece_symbol);
            } else {
                print_square("", background_color, ' ', ' ');
            }
        }
        std::cout << std::endl << "   ";
        
        // Print empty row below with appropriate colors
        for (int col = COLS - 1; col >= 0; col--) {
            bool is_light_square = (row + col) % 2 == 0;
            bool is_selected = (row == current_piece[0] && col == current_piece[1]);
            bool is_attack = possible_actions.attacks.count({row, col});
            bool is_move = possible_actions.moves.count({row, col});
            bool is_last_move = (
                std::array<int, 2>{row, col} == last_move_starting ||
                std::array<int, 2>{row, col} == last_move_ending
            );
    
            std::string background_color = is_move
                ? (is_light_square ? "102" : "42")
                : is_attack
                    ? (is_light_square ? "101" : "41")
                    : is_selected
                        ? (is_light_square ? "103" : "43")
                        : is_last_move
                            ? (is_light_square ? "106" : "46")
                            : (is_light_square ? "47" : "100");
    
            print_square("", background_color, ' ', ' ');
        }
        std::cout << std::endl;
    }
    
    // Column labels
    std::cout << "     a    b    c    d    e    f    g    h  " << std::endl;
};

// Execute a move on the board
void Board::make_action(int old_row, int old_col, int new_row, int new_col, char symbol) {
    // Check if there is a piece at the old position and the move to the new position is legal
    if (board[old_row][old_col] &&
        board[old_row][old_col]->check_if_legal_action(new_row, new_col)
    ) {
        // Handle en passant capture logic
        check_enpassant(old_row, old_col, new_row);

        // Update castling rights if the castling state is not default (no castling)
        if (castling != "____") {
            check_castling(old_row, old_col);
        }

        // Handle promotion
        if (board[old_row][old_col]->possible_actions.promotion &&
            symbol != ' ') {
            board[old_row][old_col] = Board::create_piece(symbol, old_row, old_col);
        }

        // Handle castling moves
        if (board[old_row][old_col]->piece == king && (abs(new_col - old_col) == 2)) {
            if (new_col == 1) {
                board[old_row][2] = std::move(board[old_row][0]);
                board[old_row][0] = nullptr;

                board[old_row][2]->row = old_row;
                board[old_row][2]->column = 2;
            } else {
                board[old_row][4] = std::move(board[old_row][7]);
                board[old_row][7] = nullptr;

                board[old_row][4]->row = old_row;
                board[old_row][4]->column = 4;
            }
        }
        
        // Move the piece from old position to new position
        board[new_row][new_col] = std::move(board[old_row][old_col]);
        
        // Update the piece's internal position state to the new coordinates
        board[new_row][new_col]->row = new_row;
        board[new_row][new_col]->column = new_col;
        
        // Switch the turn to the other player after a successful move
        turn = (turn == white) ? black : white;
        
        // Recalculate possible move
        get_possible_actions();
    }
}

// Generate a new board after a move (AlfaBetaPruning)
Board Board::make_action_board(int old_row, int old_col, int new_row, int new_col, char symbol) const {
    Board new_board(*this);

    // Check if there is a piece at the old position and the move to the new position is legal
    if (board[old_row][old_col] &&
        board[old_row][old_col]->check_if_legal_action(new_row, new_col)
    ) {
        // Handle en passant capture logic
        new_board.check_enpassant(old_row, old_col, new_row);

        // Update castling rights if the castling state is not default (no castling)
        if (castling != "____") {
            new_board.check_castling(old_row, old_col);
        }

        // Handle promotion
        if (board[old_row][old_col]->possible_actions.promotion &&
            symbol != ' ') {
            new_board.board[old_row][old_col] = Board::create_piece(symbol, old_row, old_col);
        }

        // Handle castling moves
        if (board[old_row][old_col]->piece == king && (abs(new_col - old_col) == 2)) {
            if (new_col == 1) {
                new_board.board[old_row][2] = std::move(new_board.board[old_row][0]);
                new_board.board[old_row][0] = nullptr;

                new_board.board[old_row][2]->row = old_row;
                new_board.board[old_row][2]->column = 2;
            } else {
                new_board.board[old_row][4] = std::move(new_board.board[old_row][7]);
                new_board.board[old_row][7] = nullptr;

                new_board.board[old_row][4]->row = old_row;
                new_board.board[old_row][4]->column = 4;
            }
        }
        
        // Move the piece from old position to new position
        new_board.board[new_row][new_col] = std::move(new_board.board[old_row][old_col]);
        
        // Update the piece's internal position state to the new coordinates
        new_board.board[new_row][new_col]->row = new_row;
        new_board.board[new_row][new_col]->column = new_col;
        
        // Switch the turn to the other player after a successful move
        new_board.turn = (new_board.turn == white) ? black : white;
    }
    return new_board;
}

// Generate AI's move
void Board::computer_action(Game& game) {
    // Store all possible actions the AI can take
    std::vector<Action> actions;
    // Store possible promotion pieces for pawn promotion
    std::vector<char> symbols;

    for (auto position : active_pieces) {
        for (auto move : board[position[0]][position[1]]->possible_actions) {
            // Check if the current piece can promote
            if (board[position[0]][position[1]]->possible_actions.promotion) {
                // Define possible promotion pieces depending on the current turn color
                if (turn == white) {
                    symbols = {'Q', 'N', 'R', 'B'};
                } else {
                    symbols = {'q', 'n', 'r', 'b'};
                }

            } else {
                // No promotion
                symbols = {' '};
            }
            
            // Evaluate the move
            for (char curr_symbol : symbols) {
                // Create an Action representing the move
                Action curr_action(
                    {position[0], position[1]},
                    {move[0], move[1]},
                    curr_symbol,
                    game.alfa_beta_pruning(
                        make_action_board(
                            position[0],
                            position[1],
                            move[0],
                            move[1],
                            curr_symbol
                        ), 2, -100000, 100000)
                );

                // Add this evaluated action to the list of possible actions
                actions.push_back(curr_action);
            }
        }
    }

    // Sort the actions based on their rating, descending for white and ascending for black
    if (turn == white) {
        std::sort(actions.begin(), actions.end(), std::greater<Action>());
    } else {
        std::sort(actions.begin(), actions.end());
    }

    // Find the best rating from the top action
    int best_rating = actions[0].rating;
    std::vector<Action> best_actions;

    // Collect all actions that have a rating close to the best one
    for (const auto& curr_action : actions) {
        if (abs(best_rating - curr_action.rating) <= 10) {
            best_actions.push_back(curr_action);
        } else {
            break;
        }
    }

    // Randomly pick one action among the best-rated actions
    Action picked_action = get_random_element(best_actions);

    // Update the game with the last move positions
    game.last_move_starting = {picked_action.old_position[0], picked_action.old_position[1]};
    game.last_move_ending = {picked_action.new_position[0], picked_action.new_position[1]};

    // Execute the chosen move on the board
    make_action(
        picked_action.old_position[0],
        picked_action.old_position[1],
        picked_action.new_position[0],
        picked_action.new_position[1],
        picked_action.symbol
    );
}

// Helper functions for creating pieces
template <typename T>
std::unique_ptr<Piece> create_piece_unique(const char& symbol, PieceType piece, PlayerColor player, int row, int col) {
    return std::make_unique<T>(symbol, piece, player, row, col);
}

std::unique_ptr<Piece> Board::create_piece(
    char symbol,
    int row,
    int col
) const {
    // Determine player color
    PlayerColor player = isupper(symbol) ? white : black;

    char upperSymbol = toupper(symbol);

    // Create the appropriate piece based on the symbol
    switch (upperSymbol) {
        case 'R': return create_piece_unique<Rook>(symbol, rook, player, row, col);
        case 'N': return create_piece_unique<Knight>(symbol, knight, player, row, col);
        case 'B': return create_piece_unique<Bishop>(symbol, bishop, player, row, col);
        case 'K': return create_piece_unique<King>(symbol, king, player, row, col);
        case 'Q': return create_piece_unique<Queen>(symbol, queen, player, row, col);
        default:  return create_piece_unique<Pawn>(symbol, pawn, player, row, col);
    }
}

// Flattens all checking positions into an unordered set for faster lookups
PositionSet Board::flatting_checkin_pieces(
    const PositionMap& checkin_pieces
) const {
    PositionSet checking_positions;
    // Check if only one piece checking the king, if more moving the king is the only solution
    if (checkin_pieces.size() == 1) {
        for (auto key : checkin_pieces) {
            checking_positions.insert(key.first);
            for (auto value : key.second) {
                checking_positions.insert(value);
            }
        }
    }
    return checking_positions;
}

// Handle en passant logic during a move
void Board::check_enpassant(int old_row, int old_col, int new_row) {
    if (board[old_row][old_col]->piece == pawn &&
        ((old_row == 1 && new_row == 3) ||
        (old_row == 6 && new_row == 4))) {
            enpassant = {(old_row + new_row) / 2, old_col};
    } else {
        enpassant = {8, 8};
    }
}

// Handle castling logic during a move
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

// Promote a pawn and create the promoted piece
std::unique_ptr<Piece> Board::create_promoted_piece_player(int row, int col) const {
    char symbol;
    std::cout << "Pick a promotion [Q, R, N, B, P]: ";
    std::cin >> symbol;

    return Board::create_piece(symbol, row, col);
}

// Select a random action from a set of best possible actions
Action Board::get_random_element(std::span<const Action> best_actions) const {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<size_t> dist(0, best_actions.size() - 1);
    return best_actions[dist(gen)];
}