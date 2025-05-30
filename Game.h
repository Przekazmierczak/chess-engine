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
    Board current_board; // Current game state
    const std::regex valid_format; // Regex for validating input positions
    std::optional<std::string> message; // Optional message for user feedback
    std::array<int, 2> last_move_starting; // Starting position of the last move
    std::array<int, 2> last_move_ending; // Ending position of the last move

    AlfaBetaPruning alfa_beta_pruning; // AI logic

    // Singleton instance access
    static Game& get_instance() {
        static Game instance;
        return instance;
    }

    // Menu to start the game
    int menu();

private:
    // Constructor
    Game();

    // Delete copy and assignment constructors to enforce singleton pattern
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    // Game flow methods
    void game_options();
    void game_simulator_player();
    void game_simulator_AI();

    // Utility functions for player interaction
    void player_pick_destination(
        const std::expected<std::string, std::string>& current_position_result
    );
    std::expected<char, std::string> get_symbol(const std::array<int, 2>& curr_row_col) const;

    // Display utilities
    void clear_screen() const;
    void show_winner();
    void print_logo() const;

    // Winner display helpers
    void print_white_winner() const;
    void print_black_winner() const;
    void print_draw() const;

    // Save/load board state
    void save_board() const;
    std::expected<Board, std::string> load_board() const;

    // Input validation methods
    std::expected<size_t, std::string> validate_menu_input(const size_t& option, const size_t& first, const size_t& last) const;
    std::expected<std::string, std::string> validate_position(const std::string& position, const std::regex& valid_format) const;
    std::expected<char, std::string> validate_promotion(const char& symbol) const;
};

#endif