//
// Created by Maxim Maximchuk on 1/3/24.
//

//
//  main.c
//  Extension
//
//  Created by Dave Hayden on 7/30/14.
//  Copyright (c) 2014 Panic, Inc. All rights reserved.
//

#include "game.h"
#include "objects/objects.h"
#include "physics/physicsFunctions.h"


static PlaydateAPI *pd = NULL;

static Objects *objects = NULL;

static Car *playerCar = NULL;

static Car *otherCar = NULL;

void setPDPtr(PlaydateAPI *p) {
    pd = p;
}

int update(void *userdata) {
    float crankAngle = pd->system->getCrankAngle();
    objects->car->direction(playerCar, crankAngle);
    pd->sprite->updateAndDrawSprites();

    PDButtons current, pushed, released = 0;
    pd->system->getButtonState(&current, &pushed, &released);

    if (current == kButtonA) {
        objects->car->move(otherCar, 250, 150, 270);
    }

    return 1;
}

void setupGame(void) {
    Logging *logging = initLogging(pd);
    objects = initObjects(pd, logging);
    objects->arena->create();
    init_physics();

    playerCar = objects->car->create(0.5, 3);
//    playerCar = objects->car->create(0.7, 20);
    objects->car->add(playerCar, 100, 100, (int) pd->system->getCrankAngle());

    otherCar = objects->car->create(0, 3);
    objects->car->add(otherCar, 250, 150, 270);

}
