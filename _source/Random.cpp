//
// Created by Ernesto Rendon on 4/14/23.
//
#include "Random.h"
#include <ctime>

// static variables must be redeclared in global space !!
std::mt19937 Random::random(time(0));

int Random::Int(int min, int max) {

    std::uniform_int_distribution<int> dist(min, max);
    return dist(random);
}