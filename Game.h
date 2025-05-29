#ifndef GAME_H
#define GAME_H

#include <expected>
#include <optional>
#include <regex>
#include <iostream>
#include <fstream>

#include "Board.h"
#include "AlfaBeta.h"

class Game {
public:
    Board current_board;
    const std::regex valid_format;
    std::optional<std::string> message;
    std::array<int, 2> last_move_starting;
    std::array<int, 2> last_move_ending;

    AlfaBetaPruning alfa_beta_pruning;

    static Game& get_instance() {
        static Game instance;
        return instance;
    }

    int menu();

private:
    Game();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void game_options();

    void game_simulator_player();
    void player_pick_destination(
        const std::expected<std::string, std::string>& res_current_position
    );
    std::expected<char, std::string> get_symbol(const std::array<int, 2>& curr_row_col) const;

    void game_simulator_AI();

    void clear_screen() const;
    void show_winner();

    void print_logo() const;
    void print_white_winner() const;
    void print_black_winner() const;
    void print_draw() const;

    void save_board() const;
    std::expected<Board, std::string> load_board() const;

    std::expected<size_t, std::string> validate_menu_input(const size_t& option, const size_t& first, const size_t& last) const;
    std::expected<std::string, std::string> validate_position(const std::string& position, const std::regex& valid_format) const;
    std::expected<char, std::string> validate_promotion(const char& symbol) const;
};

#endif