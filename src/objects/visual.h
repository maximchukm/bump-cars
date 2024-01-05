//
// Created by Maxim Maximchuk on 1/3/24.
//

#ifndef BUMP_CARS_VISUAL_H
#define BUMP_CARS_VISUAL_H

#include <pd_api.h>

typedef struct visual Visual;

struct visual {
    const char* imagePath;
    PDRect bounds;
    LCDSprite *sprite;
    short zIndex;
};

#endif //BUMP_CARS_VISUAL_H
