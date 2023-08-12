//
// Created by Ernesto Rendon on 4/13/23.
//
#include <SFML/Graphics.hpp>

#ifndef PROJECT_3_BUTTON_H
#define PROJECT_3_BUTTON_H

class Button {

public:

    Button(); // empty constructor
    Button(sf::Texture& input_texture, float x, float y); // full constructor

    void SetTexture(sf::Texture& new_texture);

    sf::Sprite GetSprite() {return _button_sprite;};

    bool Contains(int x, int y); // check if a click happened within the bounds of a button

private:

    sf::Sprite _button_sprite;
    sf::Texture _button_texture;
    float _button_x_pos, _button_y_pos;
};

#endif //PROJECT_3_BUTTON_H