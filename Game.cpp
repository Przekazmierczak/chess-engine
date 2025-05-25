#include "Game.h"
#include "Board.h"
#include "Piece.h"

#include <thread>
#include <chrono>

Game::Game() 
    : current_board() {
}

int Game::menu() {
    char symbol;
    while (true) {
        std::cout << "\x1B[2J\x1B[H";
        // std::cout << current_board << std::endl;
        // current_board.print_possible_actions();
        // std::cout << current_board.active_pieces << std::endl;

        current_board.print_white_perspective();
        
        if (current_board.winner != "") {
            if (current_board.winner == "draw") {
                std::cout << "It is the draw!" << std::endl;
            } else {
                std::cout << current_board.winner << " is the winner!" << std::endl;
            }
            break;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));

        if (current_board.turn == "white") {
            std::string current_positon;
            std::cout << "Pick a piece to move: ";
            std::cin >> current_positon;

            Notation curr_notation(current_positon);
            
            std::string next_position;
            std::cout << "Pick a destination: ";
            std::cin >> next_position;

            Notation next_notation(next_position);
            
            std::array<int, 2> curr_row_col = curr_notation.parse_square_notation();
            std::array<int, 2> next_row_col = next_notation.parse_square_notation();

            if (current_board.board[curr_row_col[0]][curr_row_col[1]]->possible_actions.promotion) {
                std::cout << "Pick a promotion [Q, R, N, B, P]: ";
                std::cin >> symbol;
            } else {
                symbol = ' ';
            }
            
            current_board.make_action(curr_row_col[0], curr_row_col[1], next_row_col[0], next_row_col[1], symbol);
        } else {
            current_board.computer_action();
        }

    }
    return 0;
}