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


static PlaydateAPI* pd = NULL;

static Objects *objects;

static Car *playerCar;

void setPDPtr(PlaydateAPI* p) {
    pd = p;
}

int update(void* userdata)
{
    float crankAngle = pd->system->getCrankAngle();
    objects->car->rotate(playerCar, crankAngle);
    pd->sprite->updateAndDrawSprites();
    return 1;
}

void setupGame(void) {
    objects = initObjects(pd);
    objects->arena->create();

    playerCar = objects->car->create(0.5, 3);
//    playerCar = objects->car->create(0.7, 20);
    objects->car->add(playerCar, 100, 100, 90);

    Car *otherCar = objects->car->create(0, 3);
    objects->car->add(otherCar, 250, 150, 270);

}