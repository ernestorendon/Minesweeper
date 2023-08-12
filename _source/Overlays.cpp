//
// Created by Ernesto Rendon on 4/25/23.
//
#include "Overlays.h"

void Overlay::Toggle() {

    _is_active = !_is_active;
}

Overlay::Overlay(sf::Vector2<float> position) {

    _overlay_sprite.setPosition(position);
}

void Overlay::SetTexture(sf::Texture& input_texture) {

    _overlay_sprite.setTexture(input_texture);
}