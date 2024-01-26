//
// Created by Maxim Maximchuk on 1/4/24.
//

#ifndef BUMP_CARS_OBJECTS_H
#define BUMP_CARS_OBJECTS_H

#include "arena.h"
#include "car.h"

typedef struct {
    const struct arena_functions* arena;
    const struct car_functions* car;
} Objects;

Objects *initObjects(PlaydateAPI *playdateApi, Logging *logging);

#endif //BUMP_CARS_OBJECTS_H
