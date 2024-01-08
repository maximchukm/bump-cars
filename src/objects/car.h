//
// Created by Maxim Maximchuk on 1/3/24.
//

#ifndef BUMP_CARS_CAR_H
#define BUMP_CARS_CAR_H

#include "visual.h"
#include "../physics/vectors.h"

typedef struct {
    const struct visual *visual;
    double mass;
    double propulsion_force;
    int angle;
    MovementVector *movement_vectors[8];
} Car;

enum car_orientation {
    kHorizontal,
    kVertical,
    kDiagonal
};

struct car_functions {
    Car* (*create)(double propulsion_force, double mass);
    Car* (*get)(LCDSprite *sprite);
    void (*add)(Car *car, float x, float y, float angle);
    void (*rotate)(Car *car, float angle);
};

struct car_functions *initCarModule(PlaydateAPI *playdate);

#endif //BUMP_CARS_CAR_H
