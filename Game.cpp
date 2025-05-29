#include "Game.h"
#include "Board.h"
#include "Piece.h"

Game::Game() 
    : current_board(),
      valid_format("[1-8][A-Ha-h]"),
      last_move_starting({-1, -1}),
      last_move_ending({-1, -1}) {
}

int Game::menu() {
    while (true) {
        clear_screen();

        std::cout << "MENU" << std::endl;
        std::cout << "Pick an option:" << std::endl;
        std::cout << "1. New game" << std::endl;
        std::cout << "2. Continue game" << std::endl;
        std::cout << "3. Exit" << std::endl;

        if (message.has_value()) {
            std::cout << message.value() << std::endl;
            message.reset();
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
                case 2: {
                    auto load = load_board();
                    if (load) {
                        current_board = load.value();
                        game_simulator_player();
                    } else {
                        message = load.error();
                    }
                    break;
                }
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
            message.reset();
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

        current_board.print_white_perspective(last_move_starting, last_move_ending);

        if (message.has_value()) {
            std::cout << message.value() << std::endl;
            message.reset();
        }
        
        if (current_board.winner != notFinished) {
            show_winner();
            break;
        }

        if (current_board.turn == white) {
            std::string current_positon;
            std::cout << "Pick a piece to move (q for save&quit): ";

            if (!(std::cin >> current_positon)) {
                std::cin.clear(); // Clear error flag
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
                message = "Invalid input. Please try again.";
                continue;
            }

            if (current_positon == "q") {
                current_board.reset();

                last_move_starting = {-1, -1};
                last_move_ending = {-1, -1};

                return;
            }

            auto res_current_position = validate_position(current_positon, valid_format);

            if (res_current_position) {
                player_pick_destination(res_current_position);
            } else {
                message = res_current_position.error();
                continue;
            }

        } else {
            current_board.computer_action(*this);
        }
        save_board();
    }
}

void Game::player_pick_destination(
    std::expected<std::string, std::string>& res_current_position
) {
    clear_screen();

    Notation curr_notation(res_current_position.value());

    std::array<int, 2> curr_row_col = curr_notation.parse_square_notation();

    if (current_board.board[curr_row_col[0]][curr_row_col[1]] && current_board.board[curr_row_col[0]][curr_row_col[1]]->player == white) {
        current_board.print_white_perspective(last_move_starting, last_move_ending, curr_row_col, current_board.board[curr_row_col[0]][curr_row_col[1]]->possible_actions);
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
                last_move_starting = {curr_row_col[0], curr_row_col[1]};
                last_move_ending = {next_row_col[0], next_row_col[1]};

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

        current_board.print_white_perspective(last_move_starting, last_move_ending);
        
        if (current_board.winner != notFinished) {
            show_winner();
            break;
        }

        current_board.computer_action(*this);
    }
}

void Game::clear_screen() {
    std::cout << "\x1B[2J\x1B[H";
    print_logo();
}

void Game::show_winner() {
    if (current_board.winner == whiteWin) {
        print_white_winner();
    } else if (current_board.winner == blackWin) {
        print_black_winner();
    } else {
        print_draw();
    }

    current_board.reset();

    last_move_starting = {-1, -1};
    last_move_ending = {-1, -1};

    std::cout << "Press any key to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
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


void Game::print_white_winner() {
    std::cout << "  \033[37m_/|                                  \033[90m|\\_\033[0m\n";
    std::cout << " \033[37m// o\\      White is the winner!      \033[90m/X \\\\\033[0m\n";
    std::cout << " \033[37m|| \\_)                              \033[90m(_. ||\033[0m\n";
    std::cout << " \033[37m//__\\                                \033[90m/__\\\\\033[0m\n";
    std::cout << " \033[37m)___(                                \033[90m)___(\033[0m\n";
}

void Game::print_black_winner() {
    std::cout << "  \033[37m_/|                                  \033[90m|\\_\033[0m\n";
    std::cout << " \033[37m// X\\      Black is the winner!      \033[90m/o \\\\\033[0m\n";
    std::cout << " \033[37m|| ._)                              \033[90m(_/ ||\033[0m\n";
    std::cout << " \033[37m//__\\                                \033[90m/__\\\\\033[0m\n";
    std::cout << " \033[37m)___(                                \033[90m)___(\033[0m\n";
}
void Game::print_draw() {
    std::cout << "  \033[37m_/|                                  \033[90m|\\_\033[0m\n";
    std::cout << " \033[37m// X\\         It is a draw!         \033[90m/X \\\\\033[0m\n";
    std::cout << " \033[37m|| ._)                              \033[90m(_. ||\033[0m\n";
    std::cout << " \033[37m//__\\                                \033[90m/__\\\\\033[0m\n";
    std::cout << " \033[37m)___(                                \033[90m)___(\033[0m\n";
}

void Game::save_board() {
    std::ofstream SaveFile("save.txt");

    for (auto &current_row : current_board.board) {
        for (auto &piece : current_row) {
            if (piece) {
                SaveFile << piece->symbol;
            } else {
                SaveFile << ' ';
            }
        }
        SaveFile << std::endl;
    }

    SaveFile << current_board.turn << std::endl;
    SaveFile << current_board.castling << std::endl;
    SaveFile << current_board.enpassant[0] << current_board.enpassant[1] << std::endl;

    SaveFile.close();
}

std::expected<Board, std::string> Game::load_board() {
    std::ifstream SaveFile("save.txt");

    if (SaveFile.is_open()) {
        std::string currect_line;
        std::array<std::array<char, 8>, 8> simplify_board;
        
        for (auto &current_row : simplify_board) {
            getline(SaveFile, currect_line);
            for (std::size_t i = 0; i < 8; i++) {
                current_row[i] = currect_line[i];
            }
        }
    
        getline(SaveFile, currect_line);
        PlayerColor turn = (PlayerColor)(std::stoi(currect_line));
    
        getline(SaveFile, currect_line);
        std::string castling = currect_line;
    
        getline(SaveFile, currect_line);
        std::array<int, 2> enpassant;
        int row = ((int)currect_line[0]) ? (int)currect_line[0] : NULL;
        int col = ((int)currect_line[1]) ? (int)currect_line[1] : NULL;
        enpassant = {row, col};
        
        SaveFile.close();
    
        return Board(turn, castling, enpassant, simplify_board);
    } else {
        return std::unexpected("No save file is available.");
    }
}

std::expected<size_t, std::string> Game::validate_menu_input(size_t option, size_t first, size_t last) {
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