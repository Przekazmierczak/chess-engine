#include "Board.h"
#include "Piece.h"
#include "Types.h"

// Constructor
Piece::Piece(
    const char& input_symbol,
    const std::string& input_piece,
    const std::string& input_player,
    const int& input_row,
    const int& input_column)
    : symbol(input_symbol),
      piece(input_piece),
      player(input_player),
      row(input_row),
      column(input_column),
      possible_actions() {
}

// Comparison operators for equality
bool Piece::operator==(const Piece& other) const {
    return (typeid(*this) == typeid(other) &&
            this->symbol == other.symbol &&
            this->piece == other.piece &&
            this->player == other.player &&
            this->row == other.row &&
            this->column == other.column);
}

// Comparison operators for inequality
bool Piece::operator!=(const Piece& other) const {
    return !(*this == other);
}

// Overloaded output stream operator for Piece
std::ostream& operator<<(std::ostream& out, const Piece& piece) {
    out << "Symbol: " << piece.symbol << ", ";
    out << "Piece: " << piece.piece << ", ";
    out << "Player: " << piece.player << ", ";
    out << "Position: (" << piece.row << ", " << piece.column << ")";
    return out;
};

// Check if a position is within the board bounds
bool Piece::is_valid_position(const Board& board, const int& row, const int& column) const{
    return row >= 0 && row < board.ROWS && column >= 0 && column < board.COLS;
}

// Checks if moving the piece will not expose the king to a check (i.e., the piece is not pinned)
bool Piece::is_not_pinned(
    const std::array<int, 2>& piece_position,
    const std::array<int, 2>& move,
    const Board& board_class,
    const PositionMap& pinned_pieces
) const {
    return (
        player != board_class.turn ||
        !pinned_pieces.count(piece_position) ||
        (
            pinned_pieces.count(piece_position) &&
            pinned_pieces.at(piece_position).count(move)
        )
    );
}

// Flattens all checking positions into an unordered set for faster lookups
PositionSet Piece::flatting_checkin_pieces(
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

// Derived class representing a Pawn
Pawn::Pawn(const char& input_symbol,
    const std::string& input_piece,
    const std::string& input_player,
    const int& input_row,
    const int& input_column
): Piece(input_symbol, input_piece, input_player, input_row, input_column) {}

// Derived class representing a Knight
Knight::Knight(const char& input_symbol,
    const std::string& input_piece,
    const std::string& input_player,
    const int& input_row,
    const int& input_column
): Piece(input_symbol, input_piece, input_player, input_row, input_column) {}

// Derived class representing a King
King::King(const char& input_symbol,
    const std::string& input_piece,
    const std::string& input_player,
    const int& input_row,
    const int& input_column
): Piece(input_symbol, input_piece, input_player, input_row, input_column) {}

// Derived class representing a Rook
Rook::Rook(const char& input_symbol,
    const std::string& input_piece,
    const std::string& input_player,
    const int& input_row,
    const int& input_column
): Piece(input_symbol, input_piece, input_player, input_row, input_column) {}

// Derived class representing a Bishop
Bishop::Bishop(const char& input_symbol,
    const std::string& input_piece,
    const std::string& input_player,
    const int& input_row,
    const int& input_column
): Piece(input_symbol, input_piece, input_player, input_row, input_column) {}

// Derived class representing a Queen
Queen::Queen(const char& input_symbol,
    const std::string& input_piece,
    const std::string& input_player,
    const int& input_row,
    const int& input_column
): Piece(input_symbol, input_piece, input_player, input_row, input_column) {}

// Helper method for rook, bishop, and queen movement logic
void Piece::rook_bishop_queen_template(
    Board& board_class,
    const std::vector<std::array<int, 2>>& directions
) {
    possible_actions.reset();

    // Determine if the current piece belongs to the opponent
    bool opponent = (player == board_class.turn) ? false : true;

    // Flatten all the checking pieces for quick access
    auto checking_positions = flatting_checkin_pieces(board_class.checkin_pieces);

    if (opponent) {
        // Handles opponent's turn: updates attacked positions and pins
        for (auto direction : directions) {
            int distance = 1; // Distance increment along a direction
            PositionSet current_direction; // Tracks valid positions in the current direction
            bool absolute_pin_check = false; // Tracks if a piece is pinned
            std::array<int, 2> pinned_piece; // Coordinates of the pinned piece

            while (true) {
                int new_row = row + distance * direction[0];
                int new_column = column + distance * direction[1];

                // Break if the position is invalid (outside the board)
                if (!is_valid_position(board_class, new_row, new_column)) break;

                if (board_class.board[new_row][new_column]) {
                    // Logic for opponent pieces and pins
                    if (board_class.board[new_row][new_column]->player != player && !absolute_pin_check) {
                        pinned_piece = {new_row, new_column};
                        board_class.attacked_positions.insert({new_row, new_column});

                        // Special handling for interaction with the king
                        if (board_class.board[new_row][new_column]->piece == "king" &&
                            board_class.board[new_row][new_column]->player != player
                        ) {
                            board_class.checkin_pieces[{row, column}] = current_direction;
                            int next_row = row + (distance + 1) * direction[0];
                            int next_column = column + (distance + 1) * direction[1];

                            if (is_valid_position(board_class, new_row, new_column)) {
                                board_class.attacked_positions.insert({next_row, next_column});
                            }

                            break;
                        } else {
                            absolute_pin_check = true; // Piece encountered is pinned
                        }
                    } else if (board_class.board[new_row][new_column]->player == player && !absolute_pin_check) {
                        // If the move results in attacking its own piece
                        board_class.attacked_positions.insert({new_row, new_column});
                        break;
                    } else if (absolute_pin_check) {
                        // If the pinned piece is the king, update pin data
                        if (board_class.board[new_row][new_column]->piece == "king" &&
                            board_class.board[new_row][new_column]->player != player
                        ) {
                            board_class.pinned_pieces[pinned_piece].insert({row, column});
                            board_class.pinned_pieces[pinned_piece].insert(current_direction.begin(), current_direction.end());
                        }
                        break;
                    } else {
                        break;
                    }
                } else {
                    // Updates attacked positions if no piece is encountered
                    if (!absolute_pin_check) {
                        board_class.attacked_positions.insert({new_row, new_column});
                    }
                    current_direction.insert({new_row, new_column});
                }
                distance++;
            }
        }
    } else {
        // Handles current player's turn: adds valid moves and attacks
        for (auto direction : directions) {
            int distance = 1;
            while (true) {
                int new_row = row + distance * direction[0];
                int new_column = column + distance * direction[1];

                // Break if the position is invalid (outside the board)
                if (!is_valid_position(board_class, new_row, new_column)) break;

                if (board_class.board[new_row][new_column]) {
                    // Checks for attacks
                    if (board_class.board[new_row][new_column]->player != player &&
                        is_not_pinned({row, column}, {new_row, new_column}, board_class, board_class.pinned_pieces) &&
                        (board_class.checkin_pieces.empty() || checking_positions.count({new_row, new_column}))
                    ) {
                        possible_actions.attacks.insert({new_row, new_column});
                        board_class.active_pieces.insert({row, column});
                    }
                    break;
                } else {
                    // Adds valid moves
                    if (is_not_pinned({row, column}, {new_row, new_column}, board_class, board_class.pinned_pieces) &&
                        (board_class.checkin_pieces.empty() || checking_positions.count({new_row, new_column}))
                    ) {
                        possible_actions.moves.insert({new_row, new_column});
                        board_class.active_pieces.insert({row, column});
                    }
                }
                distance++;
            }
        }
    }
}

void Pawn::check_piece_possible_moves (
    Board& board_class
) {
    possible_actions.reset();

    // Determine if the current piece belongs to the opponent
    bool opponent = (player == board_class.turn) ? false : true;

    // Flatten all the checking pieces for quick access
    auto checking_positions = flatting_checkin_pieces(board_class.checkin_pieces);

    // Determine movement direction based on the pawn's color
    int direction_by_colour = player == "white" ? 1: -1;

    // Initialize possible forward directions for pawn movement
    std::vector<std::array<int, 2>> directions = {{direction_by_colour, 0}};

    // Add double move direction if the pawn is in its starting position
    if ((player == "white" && row == 1) || (player == "black" && row == 6)) {
        directions.push_back({direction_by_colour * 2, 0});
    }

    // Check for promotion condition if the pawn is one move away from promotion
    if ((player == "white" && row == 6) || (player == "black" && row == 1)){
        possible_actions.promotion = true;
    }

    if (opponent) {
        // For the opponent's turn, focus on attack and checking logic
        directions = {{direction_by_colour, 1}, {direction_by_colour, -1}};
        for (auto direction : directions) {
            int new_row = row + direction[0];
            int new_column = column + direction[1];

            // If the target square is valid and contains an opponent piece
            if (is_valid_position(board_class, new_row, new_column) &&
                board_class.board[new_row][new_column] &&
                board_class.board[new_row][new_column]->player != player
            ) {
                // Mark the square as a possible move
                possible_actions.attacks.insert({new_row, new_column});

                // If the piece is the opponent's king, mark it as a checking piece
                if (board_class.board[new_row][new_column]->piece == "king") {
                    board_class.checkin_pieces[{row, column}];
                }
            }
            
            // Mark the square as attacked, regardless of the target
            if (is_valid_position(board_class, new_row, new_column)) {
                board_class.attacked_positions.insert({new_row, new_column});
            }
        }
    } else {
        // For the current player's turn, focus on valid moves and attacks
        bool can_move_second_time = true; // Indicates if the pawn can move two squares forward

        // Check forward movement directions
        for (auto direction : directions) {
            int new_row = row + direction[0];
            int new_column = column + direction[1];

            // If the square is valid, unoccupied, and eligible for movement
            if (is_valid_position(board_class, new_row, new_column) &&
                !board_class.board[new_row][new_column] &&
                can_move_second_time
            ) {
                // Check if the move is not blocked by a pin or other restrictions
                if (is_not_pinned({row, column}, {new_row, new_column}, board_class, board_class.pinned_pieces) &&
                    (board_class.checkin_pieces.empty() || checking_positions.count({new_row, new_column}))
                ) {
                    possible_actions.moves.insert({new_row, new_column});
                    board_class.active_pieces.insert({row, column});
                }
            } else {
                // If the move is blocked, the pawn can no longer move two squares forward
                can_move_second_time = false;
            }
        }

        // Check attack directions
        directions = {{direction_by_colour, 1}, {direction_by_colour, -1}};
        for (auto direction : directions) {
            int new_row = row + direction[0];
            int new_column = column + direction[1];

            // If the target square is valid and contains an opponent piece
            if (is_valid_position(board_class, new_row, new_column) &&
                board_class.board[new_row][new_column] &&
                board_class.board[new_row][new_column]->player != player
            ) {
                // Ensure the move is legal and part of any check resolution
                if (is_not_pinned({row, column}, {new_row, new_column}, board_class, board_class.pinned_pieces) &&
                    (board_class.checkin_pieces.empty() || checking_positions.count({new_row, new_column}))
                ) {
                    possible_actions.attacks.insert({new_row, new_column});
                    board_class.active_pieces.insert({row, column});
                }
            }

            // Handle en passant capture
            if (is_valid_position(board_class, new_row, new_column) &&
                std::array<int, 2>{new_row, new_column} == board_class.enpassant
            ) {
                // Ensure the move is legal and resolves any check conditions
                if (is_not_pinned({row, column}, {new_row, new_column}, board_class, board_class.pinned_pieces) &&
                    (board_class.checkin_pieces.empty() || checking_positions.count({new_row, new_column}))
                ) {
                    possible_actions.attacks.insert({new_row, new_column});
                    board_class.active_pieces.insert({row, column});
                }
            }
        }
    }
}

void Knight::check_piece_possible_moves (
    Board& board_class
) {
    possible_actions.reset();

    // Determine if the current piece belongs to the opponent
    bool opponent = (player == board_class.turn) ? false : true;

    // Flatten all the checking pieces for quick access
    auto checking_positions = flatting_checkin_pieces(board_class.checkin_pieces);

    // Define all possible moves for a knight relative to its current position
    std::vector<std::array<int, 2>> directions = {{2, 1}, {-2, 1}, {2, -1}, {-2, -1}, {1, 2}, {-1, 2}, {1, -2}, {-1, -2}};

    if (opponent) {
        // If it is the opponent's turn, focus on marking attacked positions and checking pieces
        for (auto direction : directions) {
            int new_row = row + direction[0];
            int new_column = column + direction[1];

            // Check if the target square is valid
            if (is_valid_position(board_class, new_row, new_column)) {
                board_class.attacked_positions.insert({new_row, new_column});

                // If the target square contains the opponent's king, mark the knight as a checking piece
                if (board_class.board[new_row][new_column] &&
                    board_class.board[new_row][new_column]->piece == "king" &&
                    board_class.board[new_row][new_column]->player != player
                ) {
                    board_class.checkin_pieces[{row, column}];
                }
            }
        }
    } else {
        // If it is the current player's turn, calculate valid moves and attacks
        for (auto direction : directions) {
            int new_row = row + direction[0];
            int new_column = column + direction[1];

            // Check if the target square is valid
            if (is_valid_position(board_class, new_row, new_column)) {
                // If the target square is empty, it's a valid move
                if (!board_class.board[new_row][new_column]) {
                    if (is_not_pinned({row, column}, {new_row, new_column}, board_class, board_class.pinned_pieces) &&
                        (board_class.checkin_pieces.empty() || checking_positions.count({new_row, new_column}))
                    ) {
                        possible_actions.moves.insert({new_row, new_column});
                        board_class.active_pieces.insert({row, column});
                    }
                // If the target square contains an opponent piece, it's a valid attack
                } else if (board_class.board[new_row][new_column]->player != player) {
                    if (is_not_pinned({row, column}, {new_row, new_column}, board_class, board_class.pinned_pieces) &&
                        (board_class.checkin_pieces.empty() || checking_positions.count({new_row, new_column}))
                    ) {
                        possible_actions.attacks.insert({new_row, new_column});
                        board_class.active_pieces.insert({row, column});
                    }
                }
            }
        }
    }
}

void King::check_piece_possible_moves (
    Board& board_class
) {
    possible_actions.reset();

    // Determine if the current piece belongs to the opponent
    bool opponent = (player == board_class.turn) ? false : true;

    // Flatten all the checking pieces for quick access
    auto checking_positions = flatting_checkin_pieces(board_class.checkin_pieces);

    // Define all possible moves for the king, including castling directions
    std::vector<std::array<int, 2>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}, {0, -2}, {0, 2}};

    if (opponent) {
        // If it is the opponent's turn, focus on marking attacked positions
        for (auto direction : directions) {
            int new_row = row + direction[0];
            int new_column = column + direction[1];

            // Check if the target square is valid
            if (is_valid_position(board_class, new_row, new_column)) {
                // Skip castling directions as they are not valid attacks
                if (direction == std::array<int, 2> {0, -2} || direction == std::array<int, 2> {0, 2}) {
                    continue;
                }
                // Mark the square as attacked
                board_class.attacked_positions.insert({new_row, new_column});
            }
        }
    } else {
        // If it is the current player's turn, calculate valid moves and attacks
        for (auto direction : directions) {
            int new_row = row + direction[0];
            int new_column = column + direction[1];

            // Check if the target square is valid
            if (is_valid_position(board_class, new_row, new_column)) {
                if (direction == std::array<int, 2> {0, -2}) {
                    // Check for castling to the queenside
                    if (((player == "white" && board_class.castling[0] == 'K') ||
                         (player == "black" && board_class.castling[2] == 'k')) &&
                        checking_positions.empty() &&
                        !board_class.board[row][1] &&
                        !board_class.board[row][2] &&
                        !board_class.attacked_positions.count({row, 1}) &&
                        !board_class.attacked_positions.count({row, 2})
                    ) {
                        // Add queenside castling to possible moves
                        possible_actions.moves.insert({new_row, new_column});
                        board_class.active_pieces.insert({row, column});
                    }
                } else if (direction == std::array<int, 2> {0, 2}) {
                        // Check for castling to the kingside
                        if (((player == "white" && board_class.castling[1] == 'Q') ||
                             (player == "black" && board_class.castling[3] == 'q')) &&
                            checking_positions.empty() &&
                            !board_class.board[row][4] &&
                            !board_class.board[row][5] &&
                            !board_class.board[row][6] &&
                            !board_class.attacked_positions.count({row, 4}) &&
                            !board_class.attacked_positions.count({row, 5})
                        ) {
                            // Add kingside castling to possible moves
                            possible_actions.moves.insert({new_row, new_column});
                            board_class.active_pieces.insert({row, column});
                        }
                } else if (!board_class.board[new_row][new_column]) {
                    // If the target square is empty, it is a valid move
                    if (!board_class.attacked_positions.count({new_row, new_column})) {
                        possible_actions.moves.insert({new_row, new_column});
                        board_class.active_pieces.insert({row, column});
                    }
                } else if (board_class.board[new_row][new_column]->player != player) {
                    // If the target square contains an opponent piece, it is a valid attack
                    if (!board_class.attacked_positions.count({new_row, new_column})) {
                        possible_actions.attacks.insert({new_row, new_column});
                        board_class.active_pieces.insert({row, column});
                    }
                }
            }
        }
    }
}

void Rook::check_piece_possible_moves (
    Board& board_class
) {
    // Define all possible directions the rook can move (vertical and horizontal)
    std::vector<std::array<int, 2>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    // Utilize the shared logic for rook, bishop, and queen movement
    rook_bishop_queen_template(board_class, directions);
}

void Bishop::check_piece_possible_moves (
    Board& board_class
) {
    // Define all possible directions the rook can move (vertical and horizontal)
    std::vector<std::array<int, 2>> directions = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    // Utilize the shared logic for rook, bishop, and queen movement
    rook_bishop_queen_template(board_class, directions);
}

void Queen::check_piece_possible_moves (
    Board& board_class
) {
    // Define all possible directions the rook can move (vertical and horizontal)
    std::vector<std::array<int, 2>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    // Utilize the shared logic for rook, bishop, and queen movement
    rook_bishop_queen_template(board_class, directions);
}

bool Piece::check_if_legal_action(int check_row, int check_col) {
    std::array<int, 2> current = {check_row, check_col};
    if (possible_actions.moves.find(current) != possible_actions.moves.end()) return true;
    if (possible_actions.attacks.find(current) != possible_actions.attacks.end()) return true;
    return false;
}