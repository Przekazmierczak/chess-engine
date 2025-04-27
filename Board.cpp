#include "Board.h"
#include "Piece.h"
#include "Types.h"

Board::Board()
    : turn("white"),
      castling("KQkq"),
      enpassant({NULL, NULL}),
      board(create_board()) {
    get_possible_actions();
}

Board::Board(
    std::string input_turn,
    std::string input_castling,
    std::array<std::array<char, 8>, 8> simplify_board)
    : turn(input_turn),
      castling(input_castling),
      enpassant({NULL, NULL}),
      board(create_board(simplify_board)) {
    get_possible_actions();
}

Board::Board(
    std::string input_turn,
    std::string input_castling,
    std::array<int, 2> input_enpassant,
    std::array<std::array<char, 8>, 8> simplify_board)
    : turn(input_turn),
      castling(input_castling),
      enpassant(input_enpassant),
      board(create_board(simplify_board)) {
    get_possible_actions();
}

bool Board::operator==(const Board& other) const {
    if (this->turn != other.turn) return false;
    if (this->castling != other.castling) return false;
    if (this->enpassant != other.enpassant) return false;

    for (int i = 0; i < this->ROWS; i++) {
        for (int j = 0; j < this->COLS; j++) {
            if (this->board[i][j] || other.board[i][j]) {
                if (*(this->board[i][j]) != *(other.board[i][j])) {
                    return false;
                }
            }
        }
    }
    return true;
}

std::ostream& operator<<(std::ostream& out, const Board& board_class) {
    out << "Turn: " << board_class.turn << ", ";
    out << "Castling: " << board_class.castling << ", ";
    out << "State: " << std::endl;

    for (int row = 0; row < board_class.ROWS; row++) {
        std::cout << row + 1 << " ";
        for (int col = 0; col < board_class.COLS; col++) {
            if (board_class.board[row][col]) {
                std::cout << "[" << board_class.board[row][col]->symbol << "]";
            } else {
                std::cout << "[ ]";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "   a  b  c  d  e  f  g  h " << std::endl;
    
    return out;
};

void Board::create_add_piece(char symbol, std::array<std::array<std::unique_ptr<Piece>, 8>, 8>& board, int row, int col) {
    if (toupper(symbol) == 'R') {
        board[row][col] = std::make_unique<Rook>(symbol, row, col);
    } else if (toupper(symbol) == 'N') {
        board[row][col] = std::make_unique<Knight>(symbol, row, col);
    } else if (toupper(symbol) == 'B') {
        board[row][col] = std::make_unique<Bishop>(symbol, row, col);
    } else if (toupper(symbol) == 'K') {
        board[row][col] = std::make_unique<King>(symbol, row, col);
    } else if (toupper(symbol) == 'Q') {
        board[row][col] = std::make_unique<Queen>(symbol, row, col);
    } else if (toupper(symbol) == 'P') {
        board[row][col] = std::make_unique<Pawn>(symbol, row, col);
    }
}

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

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (simplify_board[row][col] != ' ') {
                create_add_piece(simplify_board[row][col], board, row, col);
            }
        }
    }

    return board;
}

std::array<std::array<std::unique_ptr<Piece>, 8>, 8> Board::create_board(std::array<std::array<char, 8>, 8> simplify_board) {
    std::array<std::array<std::unique_ptr<Piece>, 8>, 8> board;

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (simplify_board[row][col] != ' ') {
                create_add_piece(simplify_board[row][col], board, row, col);
            }
        }
    }

    return board;
}

void Board::get_possible_actions() {
    std::array<std::array<Piece::Actions, 8>, 8> possible_moves;
    bool end = true;
    // std::string winner;

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (board[row][col] && board[row][col]->player != turn) {
                board[row][col]->check_piece_possible_moves(*this);
            }
        }
    }

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (board[row][col] && board[row][col]->player == turn) {
                board[row][col]->check_piece_possible_moves(*this);

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