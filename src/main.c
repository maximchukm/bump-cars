#include "game.h"


int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
    if ( event == kEventInit )
    {
        setPDPtr(playdate);
        playdate->display->setRefreshRate(30);
        playdate->system->setUpdateCallback(update, NULL);
        setupGame();
    }

    return 0;
}
