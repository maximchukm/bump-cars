//
// Created by Maxim Maximchuk on 1/6/24.
//

#ifndef BUMP_CARS_PHYSICSFUNCTIONS_H
#define BUMP_CARS_PHYSICSFUNCTIONS_H

#include "vectors.h"

#include "../objects/car.h"

typedef struct {
    MovementVector* (*create_MovementVector)(int direction_angle, double speed);
    void (*apply_force_to_movement_vectors)(Car *car, int forceDirectionAngle, double force);
    void (*calculate_new_position)(Car *car, float *px, float *py);
} Physics;

Physics *init_physics(void);


#endif //BUMP_CARS_PHYSICSFUNCTIONS_H
