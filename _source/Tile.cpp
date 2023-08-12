//
// Created by Ernesto Rendon on 4/25/23.
//
#include "Tile.h"

Tile::Tile(sf::Texture& hidden, float x, float y, int row, int num) {

    _tile_sprite.setTexture(hidden);
    _tile_x_pos = x;
    _tile_y_pos = y;
    _tile_sprite.setPosition(_tile_x_pos, _tile_y_pos);
    _is_a_mine = false;
    _is_flagged = false;
    _is_revealed = false;
    tile_row = row;
    tile_number = num;
}

sf::Vector2<float> Tile::GetPosition() {

    return _tile_sprite.getPosition();
}

bool Tile::Contains(int x, int y) {

    return(_tile_sprite.getGlobalBounds().contains(x, y));
}

void Tile::RevealTile(sf::Texture& revealed) {

    if (!_is_flagged) {
        SetTexture(revealed);
        _is_revealed = true;
    }
}

void Tile::SetRedColor() {

    _tile_sprite.setColor(sf::Color(250,0,0));
}

void Tile::SetTexture(sf::Texture& new_texture) {

    _tile_sprite.setTexture(new_texture);
}

bool Tile::is_complete() {

    // if this tile is either
    // 1.) a hidden mine OR
    // 2.) a revealed non-mine
    // then the tile is "complete"
    if ((_is_revealed && !_is_a_mine) || (!_is_revealed && _is_a_mine)) {
        return true;
    }
    else {
        return false;
    }
}