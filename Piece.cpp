#include "Board.h"
#include "Piece.h"
#include "Types.h"

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

bool Piece::operator==(const Piece& other) const {
    return (typeid(*this) == typeid(other) &&
            this->symbol == other.symbol &&
            this->piece == other.piece &&
            this->player == other.player &&
            this->row == other.row &&
            this->column == other.column);
}

bool Piece::operator!=(const Piece& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& out, const Piece& piece) {
    out << "Symbol: " << piece.symbol << ", ";
    out << "Piece: " << piece.piece << ", ";
    out << "Player: " << piece.player << ", ";
    out << "Position: (" << piece.row << ", " << piece.column << ")";
    return out;
};

std::ostream& operator<<(std::ostream& out, const Piece::Actions& res) {
    bool first = true;
    out << "Moves: {";
    for (auto move : res.moves) {
        if (first) {
            first = false;
        } else {
            out << ", ";}
        out << "{" << move[0] << ", " << move[1] << "}";
    }
    out << "}, ";

    first = true;
    out << "Attacks: {";
    for (auto attack : res.attacks) {
        if (first) {
            first = false;
        } else {
            out << ", ";}
        out << "{" << attack[0] << ", " << attack[1] << "}, ";
    }
    out << "}, ";

    out << "Promotion: " << (res.promotion ? "true":"false");

    return out;
};
bool Piece::is_valid_position(const Board& board, const int& row, const int& column) {
    return row >= 0 && row < board.ROWS && column >= 0 && column < board.COLS;
}

bool Piece::is_not_pinned(
    const std::array<int, 2>& piece_position,
    const std::array<int, 2>& move,
    const Board& board_class,
    const PositionMap& pinned_pieces
) {
    return (
        player != board_class.turn ||
        !pinned_pieces.count(piece_position) ||
        (
            pinned_pieces.count(piece_position) &&
            pinned_pieces.at(piece_position).count(move)
        )
    );
}

PositionSet Piece::flatting_checkin_pieces(
    const PositionMap& checkin_pieces
) {
    PositionSet checking_positions;
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

Pawn::Pawn(const char& input_symbol,
    const std::string& input_piece,
    const std::string& input_player,
    const int& input_row,
    const int& input_column
): Piece(input_symbol, input_piece, input_player, input_row, input_column) {}

Knight::Knight(const char& input_symbol,
    const std::string& input_piece,
    const std::string& input_player,
    const int& input_row,
    const int& input_column
): Piece(input_symbol, input_piece, input_player, input_row, input_column) {}

King::King(const char& input_symbol,
    const std::string& input_piece,
    const std::string& input_player,
    const int& input_row,
    const int& input_column
): Piece(input_symbol, input_piece, input_player, input_row, input_column) {}

Rook::Rook(const char& input_symbol,
    const std::string& input_piece,
    const std::string& input_player,
    const int& input_row,
    const int& input_column
): Piece(input_symbol, input_piece, input_player, input_row, input_column) {}

Bishop::Bishop(const char& input_symbol,
    const std::string& input_piece,
    const std::string& input_player,
    const int& input_row,
    const int& input_column
): Piece(input_symbol, input_piece, input_player, input_row, input_column) {}

Queen::Queen(const char& input_symbol,
    const std::string& input_piece,
    const std::string& input_player,
    const int& input_row,
    const int& input_column
): Piece(input_symbol, input_piece, input_player, input_row, input_column) {}

void Pawn::check_piece_possible_moves (
    Board& board_class
) {
    bool opponent = (player == board_class.turn) ? false : true;

    auto checking_positions = flatting_checkin_pieces(board_class.checkin_pieces);

    int direction_by_colour = player == "white" ? 1: -1;

    std::vector<std::array<int, 2>> directions = {{direction_by_colour, 0}};

    if ((player == "white" && row == 1) || (player == "black" && row == 6)) {
        directions.push_back({direction_by_colour * 2, 0});
    }

    if ((player == "white" && row == 6) || (player == "black" && row == 1)){
        possible_actions.promotion = true;
    }

    if (opponent) {
        directions = {{direction_by_colour, 1}, {direction_by_colour, -1}};
        for (auto direction : directions) {
            int new_row = row + direction[0];
            int new_column = column + direction[1];

            if (is_valid_position(board_class, new_row, new_column) &&
                board_class.board[new_row][new_column] &&
                board_class.board[new_row][new_column]->player != player
            ) {
                possible_actions.moves.insert({new_row, new_column});

                if (board_class.board[new_row][new_column]->piece == "king") {
                    board_class.checkin_pieces[{row, column}];
                }
            }
            
            if (is_valid_position(board_class, new_row, new_column)) {
                board_class.attacked_positions.insert({new_row, new_column});
            }
        }
    } else {
        bool can_move_second_time = true;
        for (auto direction : directions) {
            int new_row = row + direction[0];
            int new_column = column + direction[1];

            if (
                is_valid_position(board_class, new_row, new_column) &&
                !board_class.board[new_row][new_column] &&
                can_move_second_time
            ) {
                if (is_not_pinned({row, column}, {new_row, new_column}, board_class, board_class.pinned_pieces) &&
                    (board_class.checkin_pieces.empty() || checking_positions.count({new_row, new_column}))
                ) {
                    possible_actions.moves.insert({new_row, new_column});
                }
            } else {
                can_move_second_time = false;
            }
        }

        directions = {{direction_by_colour, 1}, {direction_by_colour, -1}};
        for (auto direction : directions) {
            int new_row = row + direction[0];
            int new_column = column + direction[1];

            if (is_valid_position(board_class, new_row, new_column) &&
                board_class.board[new_row][new_column] &&
                board_class.board[new_row][new_column]->player != player
            ) {
                if (is_not_pinned({row, column}, {new_row, new_column}, board_class, board_class.pinned_pieces) &&
                    (board_class.checkin_pieces.empty() || checking_positions.count({new_row, new_column}))
                ) {
                    possible_actions.attacks.insert({new_row, new_column});
                }
            }

            if (is_valid_position(board_class, new_row, new_column) &&
                std::array<int, 2>{new_row, new_column} == board_class.enpassant
            ) {
                if (is_not_pinned({row, column}, {new_row, new_column}, board_class, board_class.pinned_pieces) &&
                    (board_class.checkin_pieces.empty() || checking_positions.count({new_row, new_column}))
                ) {
                    possible_actions.attacks.insert({new_row, new_column});
                }
            }
        }
    }
}

void Knight::check_piece_possible_moves (
    Board& board_class
) {
    bool opponent = (player == board_class.turn) ? false : true;

    auto checking_positions = flatting_checkin_pieces(board_class.checkin_pieces);

    std::vector<std::array<int, 2>> directions;

    directions = {{2, 1}, {-2, 1}, {2, -1}, {-2, -1}, {1, 2}, {-1, 2}, {1, -2}, {-1, -2}};

    if (opponent) {
        for (auto direction : directions) {
            int new_row = row + direction[0];
            int new_column = column + direction[1];

            if (is_valid_position(board_class, new_row, new_column)) {
                board_class.attacked_positions.insert({new_row, new_column});

                if (board_class.board[new_row][new_column] &&
                    board_class.board[new_row][new_column]->piece == "king" &&
                    board_class.board[new_row][new_column]->player != player
                ) {
                    board_class.checkin_pieces[{row, column}];
                }
            }
        }
    } else {
        for (auto direction : directions) {
            int new_row = row + direction[0];
            int new_column = column + direction[1];

            if (is_valid_position(board_class, new_row, new_column)) {
                if (!board_class.board[new_row][new_column]) {
                    if (is_not_pinned({row, column}, {new_row, new_column}, board_class, board_class.pinned_pieces) &&
                        (board_class.checkin_pieces.empty() || checking_positions.count({new_row, new_column}))
                    ) {
                        possible_actions.moves.insert({new_row, new_column});
                    }
                } else if (board_class.board[new_row][new_column]->player != player) {
                    if (is_not_pinned({row, column}, {new_row, new_column}, board_class, board_class.pinned_pieces) &&
                        (board_class.checkin_pieces.empty() || checking_positions.count({new_row, new_column}))
                    ) {
                        possible_actions.attacks.insert({new_row, new_column});
                    }
                }
            }
        }
    }
}

void King::check_piece_possible_moves (
    Board& board_class
) {
    bool opponent = (player == board_class.turn) ? false : true;

    auto checking_positions = flatting_checkin_pieces(board_class.checkin_pieces);

    std::vector<std::array<int, 2>> directions;
    directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}, {0, -2}, {0, 2}};

    if (opponent) {
        for (auto direction : directions) {
            int new_row = row + direction[0];
            int new_column = column + direction[1];

            if (is_valid_position(board_class, new_row, new_column)) {
                if (direction == std::array<int, 2> {0, -2} || direction == std::array<int, 2> {0, 2}) {
                    continue;
                }
                board_class.attacked_positions.insert({new_row, new_column});
            }
        }
    } else {
        for (auto direction : directions) {
            int new_row = row + direction[0];
            int new_column = column + direction[1];

            if (is_valid_position(board_class, new_row, new_column)) {
                if (direction == std::array<int, 2> {0, -2}) {
                    if (
                        ((player == "white" && board_class.castling[0] == 'K') ||
                            (player == "black" && board_class.castling[2] == 'k')) &&
                        checking_positions.empty() &&
                        !board_class.board[row][1] &&
                        !board_class.board[row][2] &&
                        !board_class.attacked_positions.count({row, 1}) &&
                        !board_class.attacked_positions.count({row, 2})
                    ) {
                        possible_actions.moves.insert({new_row, new_column});
                    }
                } else if (direction == std::array<int, 2> {0, 2}) {
                        if (
                            ((player == "white" && board_class.castling[1] == 'Q') ||
                                (player == "black" && board_class.castling[3] == 'q')) &&
                            checking_positions.empty() &&
                            !board_class.board[row][4] &&
                            !board_class.board[row][5] &&
                            !board_class.board[row][6] &&
                            !board_class.attacked_positions.count({row, 4}) &&
                            !board_class.attacked_positions.count({row, 5})
                        ) {
                            possible_actions.moves.insert({new_row, new_column});
                        }
                } else if (!board_class.board[new_row][new_column]) {
                    if (!board_class.attacked_positions.count({new_row, new_column})) {
                        possible_actions.moves.insert({new_row, new_column});
                    }
                } else if (board_class.board[new_row][new_column]->player != player) {
                    if (!board_class.attacked_positions.count({new_row, new_column})) {
                        possible_actions.attacks.insert({new_row, new_column});
                    }
                }
            }
        }
    }
}

void Piece::rook_bishop_queen_template(
    Board& board_class,
    const std::vector<std::array<int, 2>>& directions,
    const bool& opponent,
    const PositionSet& checking_positions
) {
    if (opponent) {
        for (auto direction : directions) {
            int distance = 1;
            PositionSet current_direction;
            bool absolute_pin_check = false;
            std::array<int, 2> pinned_piece;

            while (true) {
                int new_row = row + distance * direction[0];
                int new_column = column + distance * direction[1];

                if (!is_valid_position(board_class, new_row, new_column)) {
                    break;
                }

                if (board_class.board[new_row][new_column]) {
                    if (board_class.board[new_row][new_column]->player != player && !absolute_pin_check) {
                        pinned_piece = {new_row, new_column};
                        board_class.attacked_positions.insert({new_row, new_column});

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
                            absolute_pin_check = true;
                        }
                    } else if (board_class.board[new_row][new_column]->player == player && !absolute_pin_check) {
                        board_class.attacked_positions.insert({new_row, new_column});
                        break;
                    } else if (absolute_pin_check) {
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
                    if (!absolute_pin_check) {
                        board_class.attacked_positions.insert({new_row, new_column});
                    }
                    current_direction.insert({new_row, new_column});
                }
                distance++;
            }
        }
    } else {
        for (auto direction : directions) {
            int distance = 1;
            while (true) {
                int new_row = row + distance * direction[0];
                int new_column = column + distance * direction[1];

                if (!is_valid_position(board_class, new_row, new_column)) {
                    break;
                }

                if (board_class.board[new_row][new_column]) {
                    if (board_class.board[new_row][new_column]->player != player &&
                        is_not_pinned({row, column}, {new_row, new_column}, board_class, board_class.pinned_pieces) &&
                        (board_class.checkin_pieces.empty() || checking_positions.count({new_row, new_column}))
                    ) {
                        possible_actions.attacks.insert({new_row, new_column});
                    }
                    break;
                } else {
                    if (is_not_pinned({row, column}, {new_row, new_column}, board_class, board_class.pinned_pieces) &&
                        (board_class.checkin_pieces.empty() || checking_positions.count({new_row, new_column}))
                    ) {
                        possible_actions.moves.insert({new_row, new_column});
                    }
                }
                distance++;
            }
        }
    }
}

void Rook::check_piece_possible_moves (
    Board& board_class
) {
    bool opponent = (player == board_class.turn) ? false : true;

    auto checking_positions = flatting_checkin_pieces(board_class.checkin_pieces);

    std::vector<std::array<int, 2>> directions;

    directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    rook_bishop_queen_template(board_class, directions, opponent, checking_positions);
}

void Bishop::check_piece_possible_moves (
    Board& board_class
) {
    bool opponent = (player == board_class.turn) ? false : true;

    auto checking_positions = flatting_checkin_pieces(board_class.checkin_pieces);

    std::vector<std::array<int, 2>> directions;

    directions = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    rook_bishop_queen_template(board_class, directions, opponent, checking_positions);
}

void Queen::check_piece_possible_moves (
    Board& board_class
) {
    bool opponent = (player == board_class.turn) ? false : true;

    auto checking_positions = flatting_checkin_pieces(board_class.checkin_pieces);

    std::vector<std::array<int, 2>> directions;

    directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    rook_bishop_queen_template(board_class, directions, opponent, checking_positions);
}