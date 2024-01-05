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

static Arena *arena;
static Car *playerCar;
static Car *opponentCars[2];

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
    arena = objects->arena->create();

    playerCar = objects->car->create(100, 50);
    playerCar->maxSpeed = 3;
    playerCar->acceleration = 0.05;

    objects->car->rotate(playerCar, 90);

//    opponentCars[0] = objects->car->create(150, 50);
//    objects->car->rotate(opponentCars[0], 47);

//    opponentCars[1] = objects->car->create(150, 100);

}