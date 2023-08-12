//
// Created by Ernesto Rendon on 4/12/23.
//
#include <unordered_map>
#include <SFML/Graphics.hpp>

#ifndef PROJECT_3_TILE_H
#define PROJECT_3_TILE_H

class Tile {

public:

    Tile(sf::Texture& hidden, float x, float y, int row, int num); // full constructor

    void SetTexture(sf::Texture& new_texture);
    void SetRedColor(); // set tile to red (used if mine triggered or if tile incorrectly flagged)

    int GetRow() const {return tile_row;}; // return the "row" that the tile is located in
    int GetNumber() const {return tile_number;}; // return the index of the given tile in the vector of tiles
    sf::Vector2<float> GetPosition(); // return position of the tile in the window
    sf::Sprite GetSprite() {return _tile_sprite;}; // return sf::Sprite object

    void RevealTile(sf::Texture& revealed); // sets tile texture to revealed (this is passed in as argument)
    bool Contains(int x, int y);

    std::unordered_map<std::string, int> adjacent_tiles;

    bool is_complete();

    bool _is_revealed;
    bool _is_a_mine;
    bool _is_flagged;

private:

    sf::Sprite _tile_sprite;
    float _tile_x_pos, _tile_y_pos;
    int tile_row;
    int tile_number;
};

#endif //PROJECT_3_TILE_H