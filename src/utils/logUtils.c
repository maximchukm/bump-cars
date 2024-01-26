//
// Created by Maxim Maximchuk on 1/18/24.
//

#include "logUtils.h"
#include <stdlib.h>

const PlaydateAPI *pd = NULL;

static void logger_log(char *format, int param) {
    char *msg;
    pd->system->formatString(&msg, format, param);
    pd->system->logToConsole(msg);
    free(msg);
}

Logging* initLogging(PlaydateAPI *pdPtr) {
    pd = pdPtr;
    Logging *logging = malloc(sizeof(Logging));
    logging->log = logger_log;
    return logging;
}