//
// Created by Maxim Maximchuk on 1/3/24.
//

#ifndef BUMP_CARS_CAR_H
#define BUMP_CARS_CAR_H

#include "visual.h"

typedef struct {
    const struct visual *visual;
    float angle;
} Car;

struct carFunctions {
    Car* (*create)(float, float);
    void (*rotate)(Car *car, float angle);
};

struct carFunctions *initCarModule(PlaydateAPI *playdate);

#endif //BUMP_CARS_CAR_H
