//
// Created by Ernesto Rendon on 4/25/23.
//
#include "Digit.h"

Digit::Digit(sf::Texture& num_image, int value, int x_pos, int y_pos) {

    _digit_sprite.setTexture(num_image);
    _digit_sprite.setTextureRect(sf::IntRect((21*value), 0, 21, 32));
    _digit_sprite.setPosition(x_pos, y_pos);
    _is_active = true;
}

void Digit::update_count(int multiplier) {

    // multiply 21 by (the number you want displayed) to change the rect of the sprite, which displays number you want
    _digit_sprite.setTextureRect(sf::IntRect((21*multiplier), 0, 21, 32));
}