//
// Created by Maxim Maximchuk on 1/3/24.
//

#include "graphicsUtils.h"

LCDSprite *createSprite(PlaydateAPI *pd, Visual *visual) {
    LCDSprite *sprite = pd->sprite->newSprite();
    pd->sprite->setBounds(sprite, visual->bounds);
    pd->sprite->setZIndex(sprite, visual->zIndex);
    return sprite;
}

LCDBitmap *createBitmap(PlaydateAPI *pd, const char* imagePath) {
    const char *outErr = NULL;
    LCDBitmap *bitmap = pd->graphics->loadBitmap(imagePath, &outErr);
    if (outErr != NULL) {
        pd->system->error("Error loading sprite bitmap '%s'", outErr);
    }
    return bitmap;
}

LCDBitmapTable *loadBitmapTable(PlaydateAPI *pd, const char* path) {
    const char *outErr = NULL;
    LCDBitmapTable *bitmapTable = pd->graphics->loadBitmapTable(path, &outErr);
    if (outErr != NULL) {
        pd->system->error("Error loading sprite bitmap '%s'", outErr);
    }
    return bitmapTable;
}