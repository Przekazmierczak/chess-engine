#include "Board.h"
#include "Piece.h"
#include "Types.h"

// Default constructor initializes the board to the standard starting position
Board::Board()
    : turn(white),
      castling("KQkq"),
      enpassant({NULL, NULL}),
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
      enpassant({NULL, NULL}),
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

// Board& Board::operator=(const Board& other_board) {
//     if (this == &other_board) return *this;

//     turn = other_board.turn;
//     castling = other_board.castling;
//     enpassant = other_board.enpassant;

//     for (int row = 0; row < ROWS; row++) {
//         for (int col = 0; col < COLS; col++) {
//             board[row][col] = create_piece(other_board.board[row][col]->symbol, row, col);
//         }
//     }
//     get_possible_actions();
// }

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
    auto print_square = [](std::string piece_color, std::string background_color, char mark, char piece) {
        std::cout << "\033[1;" << piece_color << ";" << background_color << "m " << mark << piece << "  \033[0m";
    };
    
    auto get_colors = [](bool has_piece, bool is_white_piece, bool is_light_square) {
        std::string piece_color = is_white_piece ? "34" : "35"; // Blue for white, magenta for black
        std::string background_color;
        
        if (has_piece) {
            background_color = is_light_square ? "47" : "100"; // Default light or dark square
        } else {
            background_color = is_light_square ? "47" : "100";
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
    
            auto [piece_color, background_color] = get_colors(has_piece, is_white_piece, is_light_square);
            if (has_piece) {
                print_square(piece_color, background_color, is_white_piece ? '^' : ' ', piece_symbol);
            } else {
                print_square("", background_color, ' ', ' ');
            }
        }
        std::cout << std::endl << "   ";
    
        for (int col = COLS - 1; col >= 0; col--) {
            bool is_light_square = (row + col) % 2 == 0;
    
            std::string background_color = (is_light_square ? "47" : "100");
    
            print_square("", background_color, ' ', ' ');
        }
        std::cout << std::endl;
    }
    
    // Column labels
    std::cout << "     a    b    c    d    e    f    g    h  " << std::endl;
};

void Board::print_white_perspective(std::array<int, 2> current_piece, Actions possible_actions) {
    auto print_square = [](std::string piece_color, std::string background_color, char mark, char piece) {
        std::cout << "\033[1;" << piece_color << ";" << background_color << "m " << mark << piece << "  \033[0m";
    };
    
    auto get_colors = [](bool has_piece, bool is_white_piece, bool is_selected, bool is_move, bool is_attack, bool is_light_square) {
        std::string piece_color = is_white_piece ? "34" : "35"; // Blue for white, magenta for black
        std::string background_color;
        
        if (has_piece) {
            if (is_selected) {
                background_color = is_light_square ? "103" : "43"; // Light or dark selected square
            } else if (is_attack) {
                background_color = is_light_square ? "101" : "41"; // Light or dark attack square
            } else {
                background_color = is_light_square ? "47" : "100"; // Default light or dark square
            }
        } else {
            if (is_move) {
                background_color = is_light_square ? "102" : "42";
            } else {
                background_color = is_light_square ? "47" : "100";
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
    
            auto [piece_color, background_color] = get_colors(has_piece, is_white_piece, is_selected, is_move, is_attack, is_light_square);
            if (has_piece) {
                print_square(piece_color, background_color, is_white_piece ? '^' : ' ', piece_symbol);
            } else {
                print_square("", background_color, ' ', ' ');
            }
        }
        std::cout << std::endl << "   ";
    
        for (int col = COLS - 1; col >= 0; col--) {
            bool is_light_square = (row + col) % 2 == 0;
            bool is_selected = (row == current_piece[0] && col == current_piece[1]);
            bool is_attack = possible_actions.attacks.count({row, col});
            bool is_move = possible_actions.moves.count({row, col});
    
            std::string background_color = is_move
                ? (is_light_square ? "102" : "42")
                : is_attack
                    ? (is_light_square ? "101" : "41")
                    : is_selected
                        ? (is_light_square ? "103" : "43")
                        : (is_light_square ? "47" : "100");
    
            print_square("", background_color, ' ', ' ');
        }
        std::cout << std::endl;
    }
    
    // Column labels
    std::cout << "     a    b    c    d    e    f    g    h  " << std::endl;
};

std::unique_ptr<Piece> Board::create_piece(
    const char& symbol,
    const int& row,
    const int& col
) {
    // Determine player color
    PlayerColor player = isupper(symbol) ? white : black;

    char upperSymbol = toupper(symbol);

    // Create the appropriate piece based on the symbol
    switch (upperSymbol) {
        case 'R': return std::make_unique<Rook>(symbol, rook, player, row, col);
        case 'N': return std::make_unique<Knight>(symbol, knight, player, row, col);
        case 'B': return std::make_unique<Bishop>(symbol, bishop, player, row, col);
        case 'K': return std::make_unique<King>(symbol, king, player, row, col);
        case 'Q': return std::make_unique<Queen>(symbol, queen, player, row, col);
        default:  return std::make_unique<Pawn>(symbol, pawn, player, row, col);
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

void Board::make_action(int old_row, int old_col, int new_row, int new_col, char symbol) {
    if (board[old_row][old_col] &&
        board[old_row][old_col]->check_if_legal_action(new_row, new_col)
    ) {
        check_enpassant(old_row, old_col, new_row);

        if (castling != "____") {
            check_castling(old_row, old_col);
        }

        if (board[old_row][old_col]->possible_actions.promotion &&
            symbol != ' ') {
            board[old_row][old_col] = Board::create_piece(symbol, old_row, old_col);
        }
        
        board[new_row][new_col] = std::move(board[old_row][old_col]);
        
        board[new_row][new_col]->row = new_row;
        board[new_row][new_col]->column = new_col;
        
        turn = (turn == white) ? black : white;
        
        get_possible_actions();
    } else {
        // std::cout << "Wrong action" << std::endl;
    }
    
}

Board Board::make_action_board(int old_row, int old_col, int new_row, int new_col, char symbol) {
    Board new_board(*this);
    if (board[old_row][old_col] &&
        board[old_row][old_col]->check_if_legal_action(new_row, new_col)
    ) {
        new_board.check_enpassant(old_row, old_col, new_row);

        if (castling != "____") {
            new_board.check_castling(old_row, old_col);
        }

        if (board[old_row][old_col]->possible_actions.promotion &&
            symbol != ' ') {
            new_board.board[old_row][old_col] = Board::create_piece(symbol, old_row, old_col);
        }
        
        new_board.board[new_row][new_col] = std::move(new_board.board[old_row][old_col]);
        
        new_board.board[new_row][new_col]->row = new_row;
        new_board.board[new_row][new_col]->column = new_col;
        
        new_board.turn = (new_board.turn == white) ? black : white;
    } else {
        // std::cout << "Wrong action" << std::endl;
    }
    return new_board;
}

void Board::check_enpassant(int old_row, int old_col, int new_row) {
    if (board[old_row][old_col]->piece == pawn &&
        (old_row == 1 && new_row == 3) ||
        (old_row == 6 && new_row == 4)) {
            enpassant = {(old_row + new_row) / 2, old_col};
    } else {
        enpassant = {NULL, NULL};
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
    std::vector<Action> actions;
    std::vector<char> symbols;

    for (auto position : active_pieces) {
        for (auto move : board[position[0]][position[1]]->possible_actions) {
            if (board[position[0]][position[1]]->possible_actions.promotion) {
                if (turn == white) {
                    symbols = {'Q', 'N', 'R', 'B'};
                } else {
                    symbols = {'q', 'n', 'r', 'b'};
                }

            } else {
                symbols = {' '};
            }
            
            for (char curr_symbol : symbols) {
                Action curr_action(
                    {position[0], position[1]},
                    {move[0], move[1]},
                    curr_symbol,
                    alfa_beta_pruning(make_action_board(position[0], position[1], move[0], move[1], curr_symbol), 2, -100000, 100000)
                );

                actions.push_back(curr_action);
            }
        }
    }

    if (turn == white) {
        std::sort(actions.begin(), actions.end(), std::greater<Action>());
    } else {
        std::sort(actions.begin(), actions.end());
    }

    make_action(actions[0].old_position[0], actions[0].old_position[1], actions[0].new_position[0], actions[0].new_position[1], actions[0].symbol);
}

int Board::alfa_beta_pruning(Board board, int depth, int alpha, int beta) {
    board.get_possible_actions();
    if (depth == 0) {
        board.get_rating();
        return board.final_rating;
    } else if (board.active_pieces.empty()) {
        if (!board.checkin_pieces.empty()) {
            if (board.turn == white) {
                return -100000 - depth;
            } else {
                return 100000 + depth;
            }
        } else {
            return 0;
        }
    } else {
        int res;
        int curr_min_max = (board.turn == white) ? -100000 : 100000;

        for (auto position : board.active_pieces) {
            for (auto move : board.board[position[0]][position[1]]->possible_actions) {
                auto make_and_minimax = [&](char promotion) {
                    return alfa_beta_pruning(board.make_action_board(position[0], position[1], move[0], move[1], promotion), depth - 1, alpha, beta);
                };

                if (board.turn == white) {
                    if (board.board[position[0]][position[1]]->possible_actions.promotion) {
                        res = std::max({
                            make_and_minimax('Q'),
                            make_and_minimax('N'),
                            make_and_minimax('B'),
                            make_and_minimax('R'),
                        });
                    } else {
                        res = make_and_minimax(' ');
                    }
                    curr_min_max = std::max(curr_min_max, res);
                    alpha = std::max(alpha, res);

                } else {
                    if (board.board[position[0]][position[1]]->possible_actions.promotion) {
                        res = std::min({
                            make_and_minimax('q'),
                            make_and_minimax('n'),
                            make_and_minimax('b'),
                            make_and_minimax('r')
                        });
                    } else {
                        res = make_and_minimax(' ');
                    }
                    curr_min_max = std::min(curr_min_max, res);
                    beta = std::min(beta, res);

                }

                if (beta <= alpha) {
                    return curr_min_max;
                }
            }
        }
        return curr_min_max;
    }
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