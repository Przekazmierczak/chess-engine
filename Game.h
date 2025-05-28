#ifndef GAME_H
#define GAME_H

#include "Board.h"

#include <expected>
#include <optional>
#include <regex>

class Game {
    public:
        Board current_board;
        std::regex valid_format;
        std::optional<std::string> message;

    Game();

    int menu();
    void game_options();

    void game_simulator_player();
    void player_pick_destination(
        std::expected<std::string, std::string>& res_current_position
    );
    std::expected<char, std::string> get_symbol(std::array<int, 2>& curr_row_col);

    void game_simulator_AI();

    void clear_screen();
    void print_logo();

    std::expected<int, std::string> validate_menu_input(size_t option, size_t first, size_t last);
    std::expected<std::string, std::string> validate_position(std::string position, std::regex valid_format);
    std::expected<char, std::string> validate_promotion(char symbol);
};

#endif