//
// Created by Maxim Maximchuk on 1/3/24.
//
#include <math.h>
#include "car.h"
#include "../utils/graphicsUtils.h"

const char *carPath = "images/bump-car";
const char *car1Path = "images/car1";

static LCDBitmapTable *car1BitmapTable;

static PlaydateAPI *pd;

static void rotateCar(Car *car, float angle) {
    if (angle > 0) {
        car->angle = 45 * div((int) angle, 45).quot;
    }
    pd->sprite->markDirty(car->visual->sprite);
}

static void carDrawFunction(LCDSprite *sprite, PDRect bounds, PDRect drawrect) {
    Car *car = (Car *) pd->sprite->getUserdata(sprite);

    float px, py;
    pd->sprite->getPosition(sprite, &px, &py);

    int x = (int) floorf(px) - 18;
    int y = (int) floorf(py) - 18;

    int bitmapIndex = car->angle > 0 ? 8 - (int) (car->angle / 45) : 0;

    LCDBitmap *bitmap = pd->graphics->getTableBitmap(car1BitmapTable, bitmapIndex);
    pd->graphics->drawBitmap(bitmap, x, y, kBitmapUnflipped);
}

static void updateCar(LCDSprite *sprite) {
    Car *car = (Car *) pd->sprite->getUserdata(sprite);

    float x, y;
    pd->sprite->getPosition(sprite, &x, &y);

    float speed = car->speed + car->acceleration;
    if (speed > car->maxSpeed) {
        speed = car->maxSpeed;
    } else {
        car->speed = speed;
    }

    float goalX = x, goalY = y;
    switch (car->angle) {
        case 0:
            goalY -= speed;
            break;
        case 45:
            goalX += speed;
            goalY -= speed;
            break;
        case 90:
            goalX += speed;
            break;
        case 135:
            goalX += speed;
            goalY += speed;
            break;
        case 180:
            goalY += speed;
            break;
        case 225:
            goalX -= speed;
            goalY += speed;
            break;
        case 270:
            goalX -= speed;
            break;
        case 315:
            goalX -= speed;
            goalY -= speed;
            break;
    }

    float actualX, actualY;
    int len;
    SpriteCollisionInfo *collisionInfo = pd->sprite->moveWithCollisions(sprite, goalX, goalY, &actualX, &actualY, &len);
    if (collisionInfo != NULL) {
        car->speed -= car->speed * 1.3;
        free(collisionInfo);
    }

}

static SpriteCollisionResponseType checkCollision(LCDSprite *first, LCDSprite *second) {
    return kCollisionTypeBounce;
}

static Car *createCar(float x, float y) {
    Visual *visual = malloc(sizeof(Visual));
    visual->imagePath = carPath;
    visual->bounds = PDRectMake(0, 0, 36, 36);
    visual->zIndex = 1;

    LCDSprite *sprite = createSprite(pd, visual);
    visual->sprite = sprite;

    pd->sprite->moveTo(sprite, x, y);
    pd->sprite->setDrawFunction(sprite, carDrawFunction);
    pd->sprite->setUpdateFunction(sprite, updateCar);
    pd->sprite->setUpdatesEnabled(sprite, 1);

    pd->sprite->setCollisionsEnabled(sprite, 1);
    pd->sprite->setCollideRect(sprite, PDRectMake(0, 0, 36, 36));
    pd->sprite->setCollisionResponseFunction(sprite, checkCollision);

    pd->sprite->addSprite(sprite);

    Car *car = malloc(sizeof(Car));
    car->visual = visual;
    car->angle = 0;

    pd->sprite->setUserdata(car->visual->sprite, car);

    return car;
}

struct carFunctions *initCarModule(PlaydateAPI *playdate) {
    pd = playdate;
    car1BitmapTable = loadBitmapTable(pd, car1Path);

    struct carFunctions *functions = malloc(sizeof(struct carFunctions));
    functions->create = createCar;
    functions->rotate = rotateCar;
    return functions;
}

