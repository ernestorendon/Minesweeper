//
// Created by Ernesto Rendon on 4/13/23.
//
#include <SFML/Graphics.hpp>

#ifndef PROJECT_3_OVERLAYS_H
#define PROJECT_3_OVERLAYS_H

class Overlay {

public:

    Overlay(sf::Vector2<float> position); // full constructor

    void SetTexture(sf::Texture& input_texture); // set Sprite object's texture

    sf::Sprite GetSprite() {return _overlay_sprite;}; // return Sprite object
    
    void Toggle(); // turn "active" on or off
    bool _is_active; // whether overlay is to be printed

private:

    sf::Sprite _overlay_sprite;
};

#endif //PROJECT_3_OVERLAYS_H