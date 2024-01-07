//
// Created by Maxim Maximchuk on 1/3/24.
//

#include "arena.h"
#include "../utils/graphicsUtils.h"

const char* arenaPath = "images/arena";

static PlaydateAPI *pd;
static LCDBitmap *bitmap;

void arenaDrawFunction(LCDSprite* sprite, PDRect bounds, PDRect drawrect) {
    pd->graphics->drawBitmap(bitmap, 0, 0, kBitmapUnflipped);
}

static void createEdge(PDRect rect, float x, float y) {
    LCDSprite *edge = pd->sprite->newSprite();
    pd->sprite->setBounds(edge, rect);
    pd->sprite->setCollisionsEnabled(edge, 1);
    pd->sprite->setCollideRect(edge, rect);
    pd->sprite->setTag(edge, 's');
    pd->sprite->moveTo(edge, x, y);
    pd->sprite->addSprite(edge);
}

static Arena *createArena(void) {
    Visual *visual = malloc(sizeof(Visual));
    visual->image_path = arenaPath;
    visual->bounds = PDRectMake(0, 0, 400, 240);
    visual->z_index = 0;
    visual->sprite = create_sprite(pd, visual);
    pd->sprite->setDrawFunction(visual->sprite, arenaDrawFunction);
    pd->sprite->addSprite(visual->sprite);

    //left edge
    createEdge(PDRectMake(0, 0, 17, 240), 8, 120);
    // right edge
    createEdge(PDRectMake(0, 0, 17, 240), 392, 120);
    // top edge
    createEdge(PDRectMake(0, 0, 400, 18), 200, 9);
    // bottom edge
    createEdge(PDRectMake(0, 0, 400, 18), 200, 231);


    Arena *arena = malloc(sizeof(Arena));
    arena->visual = visual;
    return arena;
}

struct arena_functions *initArenaModule(PlaydateAPI *playdate) {
    pd = playdate;
    bitmap = create_bitmap(pd, arenaPath);

    struct arena_functions *functions = malloc(sizeof(struct arena_functions));
    functions->create = createArena;
    return functions;
}