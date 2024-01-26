//
// Created by Maxim Maximchuk on 1/6/24.
//

#include <stdlib.h>
#include <math.h>
#include "physicsFunctions.h"

const int MASS_REDUCTION_FACTOR = 5;
const int SPEED_REDUCTION_FACTOR = 10;
const int SLIDE_FACTOR = 3;


static double acceleration(double force, double speed, double mass) {
    return force / (exp(mass / MASS_REDUCTION_FACTOR) + exp(speed / SPEED_REDUCTION_FACTOR));
}

static double deceleration(double speed, double mass, int is_slide) {
    double slide_factor = is_slide == 1 ? SLIDE_FACTOR : 1;
    return log(speed / SPEED_REDUCTION_FACTOR + 1) / exp(mass / MASS_REDUCTION_FACTOR) * slide_factor;
}

static MovementVector *create_MovementVector(int direction_angle, double force) {
    MovementVector *vector = malloc(sizeof(MovementVector));
    vector->direction_angle = direction_angle;
    vector->force = force;
    return vector;
}

static void apply_force_to_movement_vectors(Car *car, int force_direction_angle,
                                            double force) {
    for (int i = 0; i < 8; i++) {
        MovementVector *movement_vector = car->movement_vectors[i];
        if (movement_vector->direction_angle == force_direction_angle) {
            movement_vector->force += acceleration(force, movement_vector->force, car->mass);
            break;
        }
    }
}

static void calculate_new_position(Car *car, float *px, float *py) {
    double x = (double)*px;
    double y = (double)*py;

    for (int i = 0; i < 8; i++) {
        MovementVector *movementVector = car->movement_vectors[i];
        double speed = movementVector->force;
        int angle = movementVector->direction_angle;

        int diffAngle = abs(angle - car->direction);
        int slide = diffAngle == 90 || diffAngle == 270 ? 1 : 0;

        movementVector->force -= deceleration(speed, car->mass, slide);

        switch (angle) {
            case 0:
                y -= speed;
                break;
            case 45:
                x += speed;
                y -= speed;
                break;
            case 90:
                x += speed;
                break;
            case 135:
                x += speed;
                y += speed;
                break;
            case 180:
                y += speed;
                break;
            case 225:
                x -= speed;
                y += speed;
                break;
            case 270:
                x -= speed;
                break;
            case 315:
                x -= speed;
                y -= speed;
                break;
        }
    }

    *px = (float)x;
    *py = (float)y;
}

Physics *init_physics(void) {
    Physics *physics = malloc(sizeof(Physics));
    physics->create_MovementVector = create_MovementVector;
    physics->apply_force_to_movement_vectors = apply_force_to_movement_vectors;
    physics->calculate_new_position = calculate_new_position;
    return physics;
}
