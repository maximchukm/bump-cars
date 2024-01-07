//
// Created by Maxim Maximchuk on 1/3/24.
//
#include <math.h>
#include "car.h"
#include "../physics/physicsFunctions.h"
#include "../utils/graphicsUtils.h"

const char *carPath = "images/bump-car";
const char *car1Path = "images/car1";

static LCDBitmapTable *car1BitmapTable = NULL;

static PlaydateAPI *pd = NULL;

static Physics *physics = NULL;

static Car* get_car(LCDSprite *sprite) {
    if (pd->sprite->getTag(sprite) == 'c') {
        return (Car *) pd->sprite->getUserdata(sprite);
    } else {
        return NULL;
    }
}

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
    Car *car = get_car(sprite);

    float x, y;
    pd->sprite->getPosition(sprite, &x, &y);

    float goalX = x, goalY = y;

    physics->apply_force_to_movement_vectors(car, car->angle, car->propulsion_force);
    physics->calculate_new_position(car, &goalX, &goalY);

    float actualX, actualY;
    int len;
    SpriteCollisionInfo *collisionInfo = pd->sprite->moveWithCollisions(sprite, goalX, goalY, &actualX, &actualY,
                                                                        &len);
    if (collisionInfo != NULL) {
        int impact_angle = 0;
        if (collisionInfo->normal.x < 0) {
            impact_angle = 270;
        }
        if (collisionInfo->normal.x > 0) {
            impact_angle = 90;
        }

        if (collisionInfo->normal.y < 0) {
            impact_angle = 0;
        }
        if (collisionInfo->normal.y > 0) {
            impact_angle = 180;
        }

        MovementVector *movement_vector = NULL;
        int impact_car_angle = impact_angle + 180;
        impact_car_angle = impact_car_angle >= 360 ? impact_car_angle - 360 : impact_car_angle;
        for (int i = 0; i < 8; i++) {
            if (car->movement_vectors[i]->direction_angle == impact_car_angle) {
                movement_vector = car->movement_vectors[i];
                break;
            }
        }

        if (movement_vector != NULL) {
            Car *otherCar = get_car(collisionInfo->other);
            if (otherCar != NULL) {
                physics->apply_force_to_movement_vectors(otherCar, movement_vector->direction_angle,
                                                         movement_vector->speed + car->mass);
            } else {
                physics->apply_force_to_movement_vectors(car, impact_angle,
                                                         movement_vector->speed * 2);
                movement_vector->speed = 0;
            }
        }

        free(collisionInfo);
    }
}

static SpriteCollisionResponseType checkCollision(LCDSprite *first, LCDSprite *second) {
    return kCollisionTypeBounce;
}

static Car *create_car(double propulsion_force, double mass) {
    Car *car = malloc(sizeof(Car));
    car->propulsion_force = propulsion_force;
    car->mass = mass;

    int movement_angle = 0;
    for (int i = 0; i < 8; i++) {
        MovementVector *movementVector = physics->create_MovementVector(movement_angle, 0);
        car->movement_vectors[i] = movementVector;
        movement_angle += 45;
    }

    return car;
}

static void add_car(Car *car, float x, float y, float angle) {
    Visual *visual = malloc(sizeof(Visual));
    visual->image_path = carPath;
    visual->bounds = PDRectMake(0, 0, 36, 36);
    visual->z_index = 1;

    LCDSprite *sprite = create_sprite(pd, visual);
    visual->sprite = sprite;

    pd->sprite->moveTo(sprite, x, y);
    pd->sprite->setDrawFunction(sprite, carDrawFunction);
    pd->sprite->setUpdateFunction(sprite, updateCar);
    pd->sprite->setUpdatesEnabled(sprite, 1);

    pd->sprite->setCollisionsEnabled(sprite, 1);
    pd->sprite->setCollideRect(sprite, PDRectMake(0, 0, 36, 36));
    pd->sprite->setCollisionResponseFunction(sprite, checkCollision);
    pd->sprite->setTag(sprite, 'c');
    pd->sprite->setUserdata(sprite, car);


    pd->sprite->addSprite(sprite);
    car->visual = visual;

    rotateCar(car, angle);

}

struct car_functions *initCarModule(PlaydateAPI *playdate) {
    pd = playdate;
    physics = init_physics();
    car1BitmapTable = load_bitmap_table(pd, car1Path);

    struct car_functions *functions = malloc(sizeof(struct car_functions));
    functions->create = create_car;
    functions->get = get_car;
    functions->add = add_car;
    functions->rotate = rotateCar;
    return functions;
}