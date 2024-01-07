//
// Created by Maxim Maximchuk on 1/3/24.
//

#ifndef BUMP_CARS_ARENA_H
#define BUMP_CARS_ARENA_H

#include "visual.h"

typedef struct {
    const struct visual *visual;
} Arena;

struct arena_functions {
    Arena* (*create)(void);
};

struct arena_functions *initArenaModule(PlaydateAPI *playdate);

#endif //BUMP_CARS_ARENA_H
