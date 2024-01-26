//
// Created by Maxim Maximchuk on 1/18/24.
//

#ifndef BUMP_CARS_LOGUTILS_H
#define BUMP_CARS_LOGUTILS_H

#endif //BUMP_CARS_LOGUTILS_H

#include <pd_api.h>

typedef struct {
    void (*log)(char *format, int param);
} Logging;

Logging* initLogging(PlaydateAPI *pdPtr);