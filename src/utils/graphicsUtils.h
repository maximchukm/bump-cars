
//
// Created by Maxim Maximchuk on 1/3/24.
//

#ifndef BUMP_CARS_GRAPHICSUTILS_H
#define BUMP_CARS_GRAPHICSUTILS_H

#include "../objects/visual.h"

LCDSprite *create_sprite(PlaydateAPI *pd, Visual *visual);

LCDBitmap *create_bitmap(PlaydateAPI *pd, const char* imagePath);

LCDBitmapTable *load_bitmap_table(PlaydateAPI *pd, const char* path);

#endif //BUMP_CARS_GRAPHICSUTILS_H
