//
// Created by Maxim Maximchuk on 1/3/24.
//

#ifndef BUMP_CARS_VISUAL_H
#define BUMP_CARS_VISUAL_H

#include <pd_api.h>

typedef struct visual Visual;

struct visual {
    const char* image_path;
    PDRect bounds;
    LCDSprite *sprite;
    short z_index;
};

#endif //BUMP_CARS_VISUAL_H
