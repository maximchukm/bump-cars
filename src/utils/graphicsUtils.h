
//
// Created by Maxim Maximchuk on 1/3/24.
//

#ifndef BUMP_CARS_GRAPHICSUTILS_H
#define BUMP_CARS_GRAPHICSUTILS_H

#include "../objects/visual.h"

LCDSprite *createSprite(PlaydateAPI *pd, Visual *visual);

LCDBitmap *createBitmap(PlaydateAPI *pd, const char* imagePath);

LCDBitmapTable *loadBitmapTable(PlaydateAPI *pd, const char* path);

#endif //BUMP_CARS_GRAPHICSUTILS_H
