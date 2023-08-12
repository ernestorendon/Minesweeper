//
// Created by Ernesto Rendon on 4/14/23.
//
#include <random>

#ifndef PROJECT_3_RANDOM_H
#define PROJECT_3_RANDOM_H

class Random {

public:

    static int Int(int min, int max);

private:

    static std::mt19937 random;
};

#endif //PROJECT_3_RANDOM_H