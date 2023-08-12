//
// Created by Ernesto Rendon on 4/25/23.
//
#include "Game.h"

void Game::launch() {

    read_config_file();
    config();

    // create game window
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Minesweeper!");

    // render loop...
    while (window.isOpen())
    {
        sf::Event event;

        flagged_tile_count = 0;

        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
                window.close();

            /* =============================== ALL CLICKING =============================== */
            if (event.type == sf::Event::MouseButtonPressed) {

                /* =============================== LEFT CLICKING =============================== */
                if (event.mouseButton.button == sf::Mouse::Left) {

                    // retrieve position of mouse click...
                    auto mouse_position = sf::Mouse::getPosition(window);

                    // new game button...
                    if (buttons["face_button"].Contains(mouse_position.x, mouse_position.y)) {
                        read_config_file();
                        board_reset();
                        initialize_mines();
                    }

                    if (buttons["test_1"].Contains(mouse_position.x, mouse_position.y)) {
                        read_test_board("testboard1");
                    }

                    if (buttons["test_2"].Contains(mouse_position.x, mouse_position.y)) {
                        read_test_board("testboard2");
                    }

                    if (buttons["test_3"].Contains(mouse_position.x, mouse_position.y)) {
                        read_test_board("testboard3");
                    }

                    // as long as game isn't won yet...
                    if (!game_victory_state && !game_loss_state) {

                        // if a click is contained with the bounds of a tile
                        for (int i = 0; i < tiles.size(); i++) {
                            if (tiles[i].Contains(mouse_position.x, mouse_position.y) && !tiles[i]._is_flagged) {
                                if (tiles[i]._is_a_mine) {
                                    tiles[i].SetRedColor();
                                    game_loss();
                                }
                                else {
                                    game_reveal(tiles[i]);
                                }
                            }
                        }

                        // Toggle mine view (debuggin')
                        if (buttons["debug"].Contains(mouse_position.x, mouse_position.y)) {
                            mine_debugging_state = !mine_debugging_state;
                        }
                    }
                }

                /* =============================== RIGHT CLICKING =============================== */
                if (event.mouseButton.button == sf::Mouse::Right) {
                    auto mouse_position = sf::Mouse::getPosition(window);

                    // as long as game isn't won or lost yet...
                    if (!game_victory_state && !game_loss_state ) {

                        // check the right click against the rects of each tile object
                        for (int i = 0; i < overlays.size(); i++) {

                            // if a click is contained with the bounds of a hidden tile, toggle that tile's flag
                            if (tiles[i].Contains(mouse_position.x, mouse_position.y) && !(tiles[i]._is_revealed)) {
                                tiles[i]._is_flagged = !tiles[i]._is_flagged;
                                overlays[i].Toggle();
                                overlays[i].SetTexture(textures["flag"]);
                            }
                        }
                    }
                }
            }
        }

        // clear anything that was previously rendered -- do this only once!
        window.clear();

        // drawing tiles to memory
        for (int i = 0; i < tiles.size(); i++) {
            window.draw(tiles[i].GetSprite());
            if (tiles[i]._is_flagged) {
                flagged_tile_count++;
            }
        }

        // if a given overlay (mine, number, flag) is active, draw it to screen
        for (int i = 0; i < overlays.size(); i++) {
            if (overlays[i]._is_active) {
                window.draw(overlays[i].GetSprite());
            }
        }

        // if the game has been lost, print out the mines ON TOP OF the flags
        for (int i = 0; i < loss_mines.size(); i++) {
            if (loss_mines[i]._is_active) {
                window.draw(loss_mines[i].GetSprite());
            }
        }

        // if mine debugger is turned on...
        if (mine_debugging_state) {
            for (int i = 0; i < mine_debugging.size(); i++) {
                if (tiles[i]._is_a_mine) {
                    mine_debugging[i].SetTexture(textures["mine"]);
                }
                window.draw(mine_debugging[i].GetSprite());
            }
        }

        // currently, what is number of mines minus the number of flags?
        calculate_counter_value();

        // after calculated, display this number in the counter
        for (auto iter = counter_digits.begin(); iter != counter_digits.end(); ++iter) {
            if (iter->second._is_active) {
                window.draw(iter->second.GetSprite());
            }
        }

        // check each tile to see if it's "complete" (see Tile::is_complete for details)
        for (int i = 0; i < tiles.size(); i++) {
            if(tiles[i].is_complete()) {
                // if all tiles have been completed, then game has been won
                if (i == tiles.size()-1) {
                    game_victory_state = true;
                }
                continue;
            }
                // if a single tile isn't complete, stop searching (game isn't won)
            else {
                break;
            }
        }

        // win condition
        if (game_victory_state) {
            buttons["face_button"].SetTexture(textures["face_win"]);
            for (int i = 0; i < overlays.size(); i++) {
                if (tiles[i]._is_a_mine) {
                    overlays[i]._is_active = true;
                    overlays[i].SetTexture(textures["flag"]);
                }
            }
        }

        // drawing buttons to memory
        window.draw(buttons.at("face_button").GetSprite());
        window.draw(buttons.at("debug").GetSprite());
        window.draw(buttons.at("test_1").GetSprite());
        window.draw(buttons.at("test_2").GetSprite());
        window.draw(buttons.at("test_3").GetSprite());

        // copy anything that was drawn to the display device -- do this only once!!
        window.display();
    }

}

void Game::read_config_file() {

    ifstream config_file;
    config_file.open("boards/config.cfg",ios::in);

    if (config_file.is_open()) {
        string token;
        getline(config_file, token);
        tile_cols = stoi(token);
        getline(config_file, token);
        tile_rows = stoi(token);
        getline(config_file, token);
        mine_count = stoi(token);
        config_file.close();
    }

    window_width = tile_cols * 32;
    window_height = (tile_rows * 32) + 100;
    tile_count = tile_cols * tile_rows;
    BUTTON_STARTING_HEIGHT = (tile_rows*32);
    load_textures();
}

void Game::config() {

    initialize_buttons();
    initialize_tiles();
    initialize_overlays();
    initialize_mines();
    initialize_counter_digits();
    calculate_counter_value();
}

void Game::load_textures() {

    sf::Texture debug;
    sf::Texture digits;
    sf::Texture face_happy;
    sf::Texture face_lose;
    sf::Texture face_win;
    sf::Texture flag;
    sf::Texture mine;
    sf::Texture number_1;
    sf::Texture number_2;
    sf::Texture number_3;
    sf::Texture number_4;
    sf::Texture number_5;
    sf::Texture number_6;
    sf::Texture number_7;
    sf::Texture number_8;
    sf::Texture test_1;
    sf::Texture test_2;
    sf::Texture test_3;
    sf::Texture tile_hidden;
    sf::Texture tile_revealed;

    debug.loadFromFile("images/debug.png");
    digits.loadFromFile("images/digits.png");
    face_happy.loadFromFile("images/face_happy.png");
    face_lose.loadFromFile("images/face_lose.png");
    face_win.loadFromFile("images/face_win.png");
    flag.loadFromFile("images/flag.png");
    mine.loadFromFile("images/mine.png");
    number_1.loadFromFile("images/number_1.png");
    number_2.loadFromFile("images/number_2.png");
    number_3.loadFromFile("images/number_3.png");
    number_4.loadFromFile("images/number_4.png");
    number_5.loadFromFile("images/number_5.png");
    number_6.loadFromFile("images/number_6.png");
    number_7.loadFromFile("images/number_7.png");
    number_8.loadFromFile("images/number_8.png");
    test_1.loadFromFile("images/test_1.png");
    test_2.loadFromFile("images/test_2.png");
    test_3.loadFromFile("images/test_3.png");
    tile_hidden.loadFromFile("images/tile_hidden.png");
    tile_revealed.loadFromFile("images/tile_revealed.png");

    textures.emplace("debug", debug);
    textures.emplace("digits", digits);
    textures.emplace("face_happy", face_happy);
    textures.emplace("face_lose", face_lose);
    textures.emplace("face_win", face_win);
    textures.emplace("flag", flag);
    textures.emplace("mine", mine);
    textures.emplace("number_1", number_1);
    textures.emplace("number_2", number_2);
    textures.emplace("number_3", number_3);
    textures.emplace("number_4", number_4);
    textures.emplace("number_5", number_5);
    textures.emplace("number_6", number_6);
    textures.emplace("number_7", number_7);
    textures.emplace("number_8", number_8);
    textures.emplace("test_1", test_1);
    textures.emplace("test_2", test_2);
    textures.emplace("test_3", test_3);
    textures.emplace("tile_hidden", tile_hidden);
    textures.emplace("tile_revealed", tile_revealed);
}

void Game::initialize_overlays() {

    for (int i = 0; i < tiles.size(); i++) {
        Overlay temp_overlay(tiles[i].GetPosition());
        overlays.push_back(temp_overlay);
        mine_debugging.push_back(temp_overlay);
        loss_mines.push_back(temp_overlay);
    }
}

void Game::initialize_buttons() {

    // calculating x-positions for buttons...
    float SMILEY_BUTTON_CENTER_X_POSITION = window_width/2.0 - 32;
    float DEBUG_BUTTON_X_POSITION = SMILEY_BUTTON_CENTER_X_POSITION + 175;
    float TEST_1_BUTTON_X_POSITION = DEBUG_BUTTON_X_POSITION + 64;
    float TEST_2_BUTTON_X_POSITION = TEST_1_BUTTON_X_POSITION + 64;
    float TEST_3_BUTTON_X_POSITION = TEST_2_BUTTON_X_POSITION + 64;

    // generating buttons with textures and (x,y) coordinates
    buttons.emplace("face_button", Button(textures["face_happy"], SMILEY_BUTTON_CENTER_X_POSITION, BUTTON_STARTING_HEIGHT));
    buttons.emplace("debug", Button(textures["debug"], DEBUG_BUTTON_X_POSITION, BUTTON_STARTING_HEIGHT ));
    buttons.emplace("test_1", Button(textures["test_1"], TEST_1_BUTTON_X_POSITION, BUTTON_STARTING_HEIGHT ));
    buttons.emplace("test_2", Button(textures["test_2"], TEST_2_BUTTON_X_POSITION, BUTTON_STARTING_HEIGHT ));
    buttons.emplace("test_3", Button(textures["test_3"], TEST_3_BUTTON_X_POSITION, BUTTON_STARTING_HEIGHT ));
}

void Game::initialize_tiles() {

    int tile_counter = 0;
    // create initial hidden tiles
    for (int i = 0; i < tile_rows; i++) {
        for (int j = 0; j < tile_cols; j++) {
            Tile temp_tile(textures["tile_hidden"], j*32, i*32, i, tile_counter);
            calculate_adjacent_tiles(temp_tile);
            tiles.push_back(temp_tile);
            tile_counter++;
        }
    }
}

void Game::initialize_counter_digits() {

    Digit negative(textures["digits"], 10, (21*0), BUTTON_STARTING_HEIGHT);
    Digit hundreds(textures["digits"], 0, (21*1), BUTTON_STARTING_HEIGHT);
    Digit tens(textures["digits"], 0, (21*2), BUTTON_STARTING_HEIGHT);
    Digit ones(textures["digits"], 0, (21*3), BUTTON_STARTING_HEIGHT);

    counter_digits.emplace("negative", negative);
    counter_digits.at("negative")._is_active = false;
    counter_digits.emplace("hundreds", hundreds);
    counter_digits.emplace("tens", tens);
    counter_digits.emplace("ones", ones);
}

void Game::initialize_mines() {

    int mine_counter = 0;

    // create needed number of random indeces to turn into mine tiles
    while (mine_counter < mine_count) {
        // max index of container of size n is (n-1)
        int random_index = Random::Int(0, tile_count-1);

        // if tile is not already a mine, then make it a mine
        if (!tiles[random_index]._is_a_mine) {
            tiles[random_index]._is_a_mine = true;
            mine_counter++;
        }
            // if tile was already a mine (possible with random num generator)
        else {
            continue;
        }
    }
}

void Game::calculate_counter_value() {

    int game_counter_value = mine_count - flagged_tile_count;

    // display negative symbol if count is negative
    if (game_counter_value < 0) {
        counter_digits.at("negative")._is_active = true;
        game_counter_value *= -1;
    }
    else {
        counter_digits.at("negative")._is_active = false;
    }

    // if game has been either won OR lost, counter num will become zero
    int temp_hundred = 0;
    int temp_tens = 0;
    int temp_ones = 0;

    // while game isn't won or lost yet, break number down into hundreds, tens, and ones places
    if (!game_victory_state && !game_loss_state) {
        temp_hundred = game_counter_value / 100;
        temp_tens = (game_counter_value % 100) / 10;
        temp_ones = game_counter_value % 10;
    }

    // update each counter digit accordingly (see Digit::update_count for deetz)
    counter_digits.at("hundreds").update_count(temp_hundred);
    counter_digits.at("tens").update_count(temp_tens);
    counter_digits.at("ones").update_count(temp_ones);
}

void Game::game_reveal(Tile &t) {

    // if the given tile is not flagged and not revealed...
    if (!(t._is_flagged) && !(t._is_revealed)){

        // count how many mines in neighbors to tile
        int adj_counter = 0;
        for (auto iter = t.adjacent_tiles.begin(); iter != t.adjacent_tiles.end(); ++iter) {
            if (tiles[iter->second]._is_a_mine) {
                adj_counter++;
            }
        }

        // if no mines in neighbors (zero mine)
        if (adj_counter == 0) {

            // reveal the zero mine
            t.RevealTile(textures["tile_revealed"]);

            // Recursively reveal each zero mine's neighbor tiles
            for (auto iter = t.adjacent_tiles.begin(); iter != t.adjacent_tiles.end(); ++iter) {
                game_reveal(tiles[iter->second]);
            }
        }

        // if the tile has at least one mine in neighbors (no recursion after reveal)
        else {

            // reveal that tile and set overlay equal to the total number of mines in neighbors
            t.RevealTile(textures["tile_revealed"]);
            string num_prefix = "number_";
            string num_suffix = to_string(adj_counter);
            string number_text = num_prefix + num_suffix;

            overlays[t.GetNumber()]._is_active = true;
            overlays[t.GetNumber()].SetTexture(textures[number_text]);
        }
    }
}

void Game::game_loss() {

    game_loss_state = true;
    buttons["face_button"].SetTexture(textures["face_lose"]);
    // for all tiles...
    for (int i = 0; i < tiles.size(); i++) {
        if (tiles[i]._is_a_mine) {
            // remove flag and reveal the MINE
            tiles[i]._is_flagged = false;
            tiles[i].RevealTile(textures["tile_revealed"]);
            loss_mines[i]._is_active = true;
            loss_mines[i].SetTexture(textures["mine"]);
        }

        // if a hidden, flagged tile was NOT a mine (incorrect flag)
        else if (!tiles[i]._is_a_mine && tiles[i]._is_flagged) {
            tiles[i]._is_flagged = false;
            tiles[i].SetRedColor();
            overlays[i]._is_active = false;
        }
    }
}

void Game::calculate_adjacent_tiles(Tile& t) {

    int tile_number = t.GetNumber();

    // using these integer values, we can create "2d relationships" between indices in a 1d array
    int MAX_COL = tile_cols-1;
    int MAX_ROW = tile_rows-1;

    int col = (tile_number % tile_cols);
    int row = t.GetRow();

    int left = tile_number - 1;
    int right = tile_number + 1;
    int upper = tile_number - tile_cols;
    int lower = tile_number + tile_cols;
    int upper_left = tile_number - (tile_cols + 1);
    int upper_right = tile_number - (tile_cols - 1);
    int lower_left = tile_number + (tile_cols - 1);
    int lower_right = tile_number + (tile_cols + 1);


    /* =============================== CORNERS =============================== */
    // upper left corner of board
    if (row == 0 && col == 0) {
        t.adjacent_tiles.emplace("right", right);
        t.adjacent_tiles.emplace("lower", lower);
        t.adjacent_tiles.emplace("lower_right", lower_right);
    }
    // upper right corner of board
    else if (row == 0 && col == (MAX_COL)) {
        t.adjacent_tiles.emplace("left", left);
        t.adjacent_tiles.emplace("lower", lower);
        t.adjacent_tiles.emplace("lower_left", lower_left);
    }
    // lower left corner of board
    else if (row == (MAX_ROW) && col == 0) {
        t.adjacent_tiles.emplace("upper", upper);
        t.adjacent_tiles.emplace("right", right);
        t.adjacent_tiles.emplace("upper_right", upper_right);
    }
    // lower right corner of board
    else if (row == (MAX_ROW) && col == (MAX_COL)) {
        t.adjacent_tiles.emplace("left", left);
        t.adjacent_tiles.emplace("upper", upper);
        t.adjacent_tiles.emplace("upper_left", upper_left);
    }

    /* =============================== TOP & BOTTOM ROWS =============================== */
    // all tiles in first row
    else if (row == 0 ) {
        t.adjacent_tiles.emplace("left", left);
        t.adjacent_tiles.emplace("right", right);
        t.adjacent_tiles.emplace("lower", lower);
        t.adjacent_tiles.emplace("lower_left", lower_left);
        t.adjacent_tiles.emplace("lower_right", lower_right);
    }
    // all tiles in bottom row
    else if (row == (MAX_ROW)) {
        t.adjacent_tiles.emplace("left", left);
        t.adjacent_tiles.emplace("right", right);
        t.adjacent_tiles.emplace("upper", upper);
        t.adjacent_tiles.emplace("upper_left", upper_left);
        t.adjacent_tiles.emplace("upper_right", upper_right);
    }

    /* =============================== LEFT & RIGHT EDGES =============================== */
    // all tiles on left edge
    else if (col == 0) {
        t.adjacent_tiles.emplace("upper", upper);
        t.adjacent_tiles.emplace("upper_right", upper_right);
        t.adjacent_tiles.emplace("right", right);
        t.adjacent_tiles.emplace("lower_right", lower_right);
        t.adjacent_tiles.emplace("lower", lower);
    }
    // all tiles on right edge
    else if (col == MAX_COL) {
        t.adjacent_tiles.emplace("upper", upper);
        t.adjacent_tiles.emplace("upper_left", upper_left);
        t.adjacent_tiles.emplace("left", left);
        t.adjacent_tiles.emplace("lower_left", lower_left);
        t.adjacent_tiles.emplace("lower", lower);
    }

    /* =============================== ALL OTHER TILES =============================== */
    else {
        t.adjacent_tiles.emplace("upper", upper);
        t.adjacent_tiles.emplace("lower", lower);
        t.adjacent_tiles.emplace("left", left);
        t.adjacent_tiles.emplace("right", right);
        t.adjacent_tiles.emplace("upper_left", upper_left);
        t.adjacent_tiles.emplace("upper_right", upper_right);
        t.adjacent_tiles.emplace("lower_left", lower_left);
        t.adjacent_tiles.emplace("lower_right", lower_right);
    }
}

void Game::board_reset() {

    game_victory_state = false;
    game_loss_state = false;
    tiles.clear();
    overlays.clear();
    mine_debugging.clear();
    loss_mines.clear();
    counter_digits.clear();
    buttons["face_button"].SetTexture(textures["face_happy"]);
    initialize_tiles();
    initialize_overlays();
    initialize_counter_digits();
}

void Game::read_test_board(string boardname) {

    mine_count = 0;
    board_reset();

    string file_prefix = "boards/";
    string file_suffix = ".brd";
    string file_name = file_prefix + boardname + file_suffix;
    ifstream file;

    file.open(file_name,ios::in);

    char c;
    int i = 0;

    while (file.get(c)) {

        // if 1 in "tile", make it bomb
        if (c == '1') {
            tiles[i]._is_a_mine = true;
            mine_count++;
            i++;
        }
        // if 0 in "tile", make it empty
        else if (c == '0') {
            i++;
            continue;
        }
        // else, we've read in a "/n" char, skip
        else {
            continue;
        }
    }

    file.close();
}