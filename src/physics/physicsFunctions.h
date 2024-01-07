//
// Created by Maxim Maximchuk on 1/6/24.
//

#ifndef BUMP_CARS_PHYSICSFUNCTIONS_H
#define BUMP_CARS_PHYSICSFUNCTIONS_H

typedef struct {
    int direction_angle;
    double speed;
} MovementVector;

typedef struct {
    MovementVector* (*create_MovementVector)(int direction_angle, double speed);
    void (*apply_force_to_movement_vectors)(MovementVector **movementVectors, double mass, int forceDirectionAngle, double force);
    void (*calculate_new_position)(float *px, float *py, double mass, MovementVector **movementVectors);
} Physics;

Physics *init_physics(void);


#endif //BUMP_CARS_PHYSICSFUNCTIONS_H
