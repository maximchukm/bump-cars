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

static int normalize_angle(int angle) {
    angle = 45 * div((int) angle, 45).quot;
    if (angle < 0) {
        return angle + 360;
    }
    if (angle >= 360) {
        return angle - 360;
    }
    return angle;
}

static Car *get_car(LCDSprite *sprite) {
    if (pd->sprite->getTag(sprite) == 'c') {
        return (Car *) pd->sprite->getUserdata(sprite);
    } else {
        return NULL;
    }
}

static enum car_orientation get_car_orientation(Car *car) {
    int angle = car->propulsion_angle;
    if (angle == 270 || angle == 90) {
        return kHorizontal;
    }
    if (angle == 0 || angle == 180) {
        return kVertical;
    }
    return kDiagonal;
}

static MovementVector *get_forward_movement_vector(Car *car) {
    for (int i = 0; i < 8; i++) {
        MovementVector *vector = car->movement_vectors[i];
        if (vector->direction_angle == car->propulsion_angle) {
            return vector;
        }
    }
    return NULL;
}

static MovementVector *get_backward_movement_vector(Car *car) {
    int opposite_angle = car->propulsion_angle + 180;
    opposite_angle = opposite_angle >= 360 ? opposite_angle - 360 : opposite_angle;

    for (int i = 0; i < 8; i++) {
        MovementVector *vector = car->movement_vectors[i];
        if (vector->direction_angle == opposite_angle) {
            return vector;
        }
    }
    return NULL;
}

static void rotateCar(Car *car, float angle) {

    // check if car moving
    MovementVector *forward_vector  = get_forward_movement_vector(car);
    MovementVector *backward_vector  = get_backward_movement_vector(car);
    if ((forward_vector == NULL || forward_vector->force < 0.01) && (backward_vector == NULL || backward_vector->force  < 0.01)) {
        return;
    }

    car->target_propulsion_angle = normalize_angle(angle);

    pd->sprite->markDirty(car->visual->sprite);
}

static void carDrawFunction(LCDSprite *sprite, PDRect bounds, PDRect drawrect) {
    Car *car = (Car *) pd->sprite->getUserdata(sprite);

    float px, py;
    pd->sprite->getPosition(sprite, &px, &py);

    int x = (int) floorf(px) - 18;
    int y = (int) floorf(py) - 18;

    int bitmapIndex = car->propulsion_angle > 0 ? 8 - (int) (car->propulsion_angle / 45) : 0;

    LCDBitmap *bitmap = pd->graphics->getTableBitmap(car1BitmapTable, bitmapIndex);
    pd->graphics->drawBitmap(bitmap, x, y, kBitmapUnflipped);
}

static void updateCar(LCDSprite *sprite) {
    Car *car = get_car(sprite);

    // rotate car if needed
    int current_angle = car->propulsion_angle;
    int target_angle = car->target_propulsion_angle;

    int diff = current_angle - target_angle;
    if (diff != 0) {
        if ((abs(diff) < 180 && diff < 0) || (abs(diff) > 180 && diff > 0)) {
            car->propulsion_angle = normalize_angle(current_angle + 45);
        } else {
            car->propulsion_angle = normalize_angle(current_angle - 45);
        }
    }

    //update collision rect
    switch (get_car_orientation(car)) {
        case kHorizontal:
            pd->sprite->setCollideRect(sprite, PDRectMake(2, 8, 32, 20));
            break;
        case kVertical:
            pd->sprite->setCollideRect(sprite, PDRectMake(8, 2, 20, 32));
            break;
        default:
            pd->sprite->setCollideRect(sprite, PDRectMake(0, 0, 36, 36));
    }

    PDButtons current, pushed, released = 0;
    pd->system->getButtonState(&current, &pushed, &released);

    if (current == kButtonB) {
        physics->apply_force_to_movement_vectors(car, car->propulsion_angle, car->propulsion_force);
    }

    float x, y;
    pd->sprite->getPosition(sprite, &x, &y);

    float goalX = x, goalY = y;

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
                                                         movement_vector->force * 2 * car->mass / otherCar->mass);
            } else {
                physics->apply_force_to_movement_vectors(car, impact_angle,
                                                         movement_vector->force * 2);
                movement_vector->force = 0;
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

static void add_car(Car *car, float x, float y, int angle) {
    car->target_propulsion_angle = normalize_angle(angle);
    car->propulsion_angle = car->target_propulsion_angle;

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
    pd->sprite->setCollisionResponseFunction(sprite, checkCollision);
    pd->sprite->setTag(sprite, 'c');
    pd->sprite->setUserdata(sprite, car);


    pd->sprite->addSprite(sprite);
    car->visual = visual;

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