//
// Created by Maxim Maximchuk on 1/4/24.
//

#ifndef BUMP_CARS_OBJECTS_H
#define BUMP_CARS_OBJECTS_H

#include "arena.h"
#include "car.h"

typedef struct Objects Objects;

struct Objects {
    const struct arenaFunctions* arena;
    const struct carFunctions* car;
};

Objects *initObjects(PlaydateAPI *playdateApi);

#endif //BUMP_CARS_OBJECTS_H
