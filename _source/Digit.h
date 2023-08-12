//
// Created by Ernesto Rendon on 4/21/23.
//
#include <unordered_map>
#include <SFML/Graphics.hpp>

#ifndef PROJECT_3_COUNTER_H
#define PROJECT_3_COUNTER_H

class Digit {

public:

    Digit(sf::Texture& num_image, int value, int x_pos, int y_pos); // full constructor

    sf::Sprite GetSprite() {return _digit_sprite;}; // return Sprite object
    void update_count(int value); // change the number that this digit is displaying
    bool _is_active; // whether digit should be displayed

private:

    sf::Sprite _digit_sprite;
};

#endif //PROJECT_3_COUNTER_H