//
// Created by Ernesto Rendon on 4/25/23.
//
#include "Button.h"

Button::Button() {}

bool Button::Contains(int x, int y) {

    return(_button_sprite.getGlobalBounds().contains(x, y));
}

Button::Button(sf::Texture& input_texture, float x, float y) {

    _button_sprite.setTexture(input_texture);
    _button_x_pos = x;
    _button_y_pos = y;
    _button_sprite.setPosition(_button_x_pos, _button_y_pos);
}

void Button::SetTexture(sf::Texture& new_texture) {

    _button_sprite.setTexture(new_texture);
}