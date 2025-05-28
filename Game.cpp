#include "Game.h"
#include "Board.h"
#include "Piece.h"

#include <thread>
#include <chrono>

Game::Game() 
    : current_board(),
      valid_format("[1-8][A-Ha-h]") {
}

int Game::menu() {
    while (true) {
        clear_screen();

        std::cout << "MENU" << std::endl;
        std::cout << "Pick an option:" << std::endl;
        std::cout << "1. New game" << std::endl;
        std::cout << "2. Load a game" << std::endl;
        std::cout << "3. Exit" << std::endl;

        if (message.has_value()) {
            std::cout << message.value() << std::endl;
        }

        std::size_t option;
        std::cout << "Enter your choice: ";

        if (!(std::cin >> option)) {
            std::cin.clear(); // Clear error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            message = "Invalid input. Please enter a number.";
            continue;
        }

        auto result = validate_menu_input(option, 1, 3);

        if (result) {
            switch (result.value()) {
                case 1:
                    game_options();
                    break;
                case 2:
                    break;
                case 3:
                    return 0;
            }
        } else {
            message = result.error();
        }
    }
}

void Game::game_options() {
    std::optional<std::string> message;

    while (true) {
        clear_screen();
    
        std::cout << "MENU" << std::endl;
        std::cout << "Pick an option:" << std::endl;
        std::cout << "1. Play vs AI" << std::endl;
        std::cout << "2. AI vs AI" << std::endl;
        std::cout << "3. Back" << std::endl;

        if (message.has_value()) {
            std::cout << message.value() << std::endl;
        }

        std::size_t option;
        std::cout << "Enter your choice: ";
        
        if (!(std::cin >> option)) {
            std::cin.clear(); // Clear error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            message = "Invalid input. Please enter a number.";
            continue;
        }
    
        auto result = validate_menu_input(option, 1, 3);
    
        if (result) {
            switch (result.value()) {
                case 1:
                    game_simulator_player();
                    break;
                case 2:
                    game_simulator_AI();
                    break;
                case 3:
                    return;
            }
        } else {
            message = result.error();
        }
    }
}

void Game::game_simulator_player() {
    while (true) {
        clear_screen();

        current_board.print_white_perspective();

        if (message.has_value()) {
            std::cout << message.value() << std::endl;
            message.reset();
        }
        
        if (current_board.winner != notFinished) {
            if (current_board.winner == draw) {
                std::cout << "It is the draw!" << std::endl;
            } else {
                std::cout << current_board.winner << " is the winner!" << std::endl;
            }
            break;
        }
        // std::this_thread::sleep_for(std::chrono::seconds(1));

        if (current_board.turn == white) {
            std::string current_positon;
            std::cout << "Pick a piece to move: ";

            if (!(std::cin >> current_positon)) {
                std::cin.clear(); // Clear error flag
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
                message = "Invalid input. Please try again.";
                continue;
            }

            auto res_current_position = validate_position(current_positon, valid_format);

            if (res_current_position) {
                player_pick_destination(res_current_position);
            } else {
                message = res_current_position.error();
                continue;
            }

        } else {
            current_board.computer_action();
        }
    }
}

void Game::player_pick_destination(
    std::expected<std::string, std::string>& res_current_position
) {
    clear_screen();

    Notation curr_notation(res_current_position.value());

    std::array<int, 2> curr_row_col = curr_notation.parse_square_notation();

    if (current_board.board[curr_row_col[0]][curr_row_col[1]] && current_board.board[curr_row_col[0]][curr_row_col[1]]->player == white) {
        current_board.print_white_perspective(curr_row_col, current_board.board[curr_row_col[0]][curr_row_col[1]]->possible_actions);
    } else {
        message = "You need to pick your own piece!";
        return;
    }
    
    std::string next_position;
    std::cout << "Pick a destination: ";

    if (!(std::cin >> next_position)) {
        std::cin.clear(); // Clear error flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
        message = "Invalid input. Please try again.";
        return;
    }

    auto res_next_position = validate_position(next_position, valid_format);

    if (res_next_position) {
        Notation next_notation(next_position);
        
        std::array<int, 2> next_row_col = next_notation.parse_square_notation();

        if (current_board.board[curr_row_col[0]][curr_row_col[1]]->possible_actions.moves.count({next_row_col[0], next_row_col[1]}) ||
        current_board.board[curr_row_col[0]][curr_row_col[1]]->possible_actions.attacks.count({next_row_col[0], next_row_col[1]})) {
            auto symbol = get_symbol(curr_row_col);
            if (symbol) {
                current_board.make_action(curr_row_col[0], curr_row_col[1], next_row_col[0], next_row_col[1], symbol.value());
            } else {
                message = symbol.error();
                return;
            }
        } else {
            message = "Invalid move.";
            return;
        }

    } else {
        message = res_next_position.error();
        return;
    }
}

std::expected<char, std::string> Game::get_symbol(std::array<int, 2>& curr_row_col) {
    char symbol;
    if (current_board.board[curr_row_col[0]][curr_row_col[1]]->possible_actions.promotion) {
        std::cout << "Pick a promotion [Q, R, N, B]: ";
        if (!(std::cin >> symbol)) {
            std::cin.clear(); // Clear error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            return std::unexpected("Invalid input. Please try again.");
        }
        auto result = validate_promotion(symbol);

        if (result) {
            return result.value();
        } else {
            return std::unexpected(result.error());
        }
    }
    return ' ';
}

void Game::game_simulator_AI() {
    while (true) {
        clear_screen();

        current_board.print_white_perspective();
        
        if (current_board.winner != notFinished) {
            if (current_board.winner == draw) {
                std::cout << "It is the draw!" << std::endl;
            } else {
                std::cout << current_board.winner << " is the winner!" << std::endl;
            }
            break;
        }
        // std::this_thread::sleep_for(std::chrono::seconds(1));

        current_board.computer_action();
    }
}

void Game::clear_screen() {
    std::cout << "\x1B[2J\x1B[H";
    print_logo();
}

void Game::print_logo() {
    std::cout << "     |\\_        _____ _                   " << std::endl;
    std::cout << "     /  .\\_    / ____| |                  " << std::endl;
    std::cout << "    |   ___)  | |    | |__   ___  ___ ___ " << std::endl;
    std::cout << "    |    \\    | |    | '_ \\ / _ \\/ __/ __|" << std::endl;
    std::cout << "    |     |   | |____| | | |  __/\\__ \\__ \\" << std::endl;
    std::cout << "    /_____\\    \\_____|_| |_|\\___||___/___/" << std::endl;
    std::cout << "   [_______]                              " << std::endl;
    std::cout << "                                          " << std::endl;
}

std::expected<int, std::string> Game::validate_menu_input(size_t option, size_t first, size_t last) {
    if (option >= first && option <= last) {
        return option;
    }
    return std::unexpected("Pick the number from " + std::to_string(first) + "-" + std::to_string(last));
}

std::expected<std::string, std::string> Game::validate_position(std::string position, std::regex valid_format) {
    if (std::regex_match(position, valid_format)) {
        return position;
    }
    return std::unexpected("Input does not match the required format. Correct format: <digit><letter>");
}

std::expected<char, std::string> Game::validate_promotion(char symbol) {
    std::unordered_set<char> promotions = {'Q', 'R', 'N', 'B'};
    if (promotions.count(symbol)) {
        return symbol;
    }
    return std::unexpected("Input does not match the required format.");
}