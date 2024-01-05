//
// Created by Maxim Maximchuk on 1/4/24.
//

#include "objects.h"
#include "car.h"
#include "arena.h"

Objects *initObjects(PlaydateAPI* playdateApi) {
    Objects *objects = malloc(sizeof(Objects));
    objects->arena = initArenaModule(playdateApi);
    objects->car = initCarModule(playdateApi);
    return objects;
}



