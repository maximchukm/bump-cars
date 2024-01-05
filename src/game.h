//
// Created by Maxim Maximchuk on 1/3/24.
//

#ifndef BUMP_CARS_GAME_H
#define BUMP_CARS_GAME_H

#include <stdio.h>

#include "pd_api.h"

int update(void* ud);
void setupGame(void);
void setPDPtr(PlaydateAPI* p);

#endif //BUMP_CARS_GAME_H
