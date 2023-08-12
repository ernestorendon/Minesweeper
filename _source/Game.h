//
// Created by Ernesto Rendon on 4/12/23.
//
#include <iostream>
#include <fstream>
#include <unordered_map>
#include "Tile.h"
#include "Button.h"
#include "Overlays.h"
#include "Random.h"
#include "Digit.h"

using namespace std;

#ifndef PROJECT_3_GAME_H
#define PROJECT_3_GAME_H

class Game {

public:

    void launch(); // starts game and handles core game logic

    void config(); // set game options

    void read_config_file(); // read in config settings from file

    void read_test_board(string boardname); // read in board from .brd file

    void load_textures(); // loads in textures from images folder

    void initialize_tiles(); // set up vector of Tile objects
    void initialize_mines(); // randomly choose Tiles to become mines
    void initialize_overlays(); // set up vector of Overlay objects

    void initialize_buttons(); // set up container for Button objects
    void initialize_counter_digits(); // set up container for Counter digits

    void board_reset(); // clear out tiles, overlays, and counter digits, then re-initialize them

    void calculate_adjacent_tiles(Tile& t); // find indices of adjacent tiles to a given Tile argument

    void calculate_counter_value(); // calculate the number of mines minus number of flags on board

    void game_reveal(Tile& t); // recursive tile revealing algorithm

    void game_loss(); // handles Game during losing

private:

    int window_width, window_height, mine_count, tile_cols, tile_rows, tile_count; // game characteristics

    unordered_map<string, sf::Texture> textures; // container of sf::Texture objects

    int BUTTON_STARTING_HEIGHT; // how low should the buttons be placed

    int flagged_tile_count = 0; // how many tiles are currently flagged

    vector<Tile> tiles; // hidden or revealed square images
    vector<Overlay> overlays; // overlays are flags and numbers (drawn OVER both hidden and revealed tiles)
    vector<Overlay> loss_mines;
    vector<Overlay> mine_debugging; // separate overlay is used for mine-debugging button, drawn over everything else

    unordered_map<string, Button> buttons; // buttons for user to click
    unordered_map<string, Digit> counter_digits; // counter digits

    bool game_victory_state = false;
    bool game_loss_state = false;
    bool mine_debugging_state = false;
};

#endif //PROJECT_3_GAME_H