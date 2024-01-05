//
// Created by Maxim Maximchuk on 1/3/24.
//
#include <math.h>
#include "car.h"
#include "../utils/graphicsUtils.h"

const char* carPath = "images/bump-car";
LCDBitmap *carBitmap = NULL;

static PlaydateAPI *pd;

static void carDrawFunction(LCDSprite* sprite, PDRect bounds, PDRect drawrect)
{
    Car* car = (Car*) pd->sprite->getUserdata(sprite);
    float px, py;
    pd->sprite->getPosition(sprite, &px, &py);
    int x = (int) floorf(px) - 16;
    int y = (int) floorf(py) - 16;
//    pd->graphics->drawBitmap(carBitmap, x, y, kBitmapUnflipped);
    pd->graphics->drawRotatedBitmap(carBitmap, x, y, car->angle, 0, 0, 1, 1);
}

static void rotateCar(Car *car, float angle) {
    car->angle = angle;
    pd->sprite->markDirty(car->visual->sprite);
}

static Car *createCar(float x, float y) {
    Visual* visual = malloc(sizeof(Visual));
    visual->imagePath = carPath;
    visual->bounds = PDRectMake(0, 0, 32, 32);
    visual->zIndex = 1;

    LCDSprite *sprite = createSprite(pd, visual);
    visual->sprite = sprite;

    pd->sprite->moveTo(sprite, x, y);
//    pd->sprite->setUpdatesEnabled(sprite, 1);
    pd->sprite->setDrawFunction(sprite, carDrawFunction);
    pd->sprite->addSprite(sprite);

    Car *car = malloc(sizeof(Car));
    car->visual = visual;
    car->angle = 0;

    pd->sprite->setUserdata(car->visual->sprite, car);

    return car;
}

struct carFunctions *initCarModule(PlaydateAPI *playdate) {
    pd = playdate;
    carBitmap = createBitmap(pd, carPath);

    struct carFunctions *functions = malloc(sizeof(struct carFunctions));
    functions->create = createCar;
    functions->rotate = rotateCar;
    return functions;
}

