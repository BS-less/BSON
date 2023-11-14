#include "log.h"
#include "memory.h"

#include <stdio.h>
#include <string.h>

void bson_log_init(BsonLog *log) {
    if(log == NULL)
        return;
    log->cursor = 0;
    log->nbuffer = 256;
    log->buffer = bsonmalloc(log->nbuffer);
    memset(log->buffer, 0, log->nbuffer);
}

void bson_log_free(BsonLog *log) {
    if(log == NULL || log->buffer == NULL)
        return;
    bsonfree(log->buffer);
    log->buffer = NULL;
}

void bson_logf(BsonLog *log, const char * const fmt, ...) {
    if(log == NULL || log->buffer == NULL)
        return;
    va_list args;
    va_start(args, fmt);
    size_t len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    if(len == 0)
        return;
    if(log->cursor + len >= log->nbuffer) {
        log->nbuffer = log->nbuffer * 3 + len;
        log->buffer = bsonrealloc(log->buffer, log->nbuffer);
    }
    va_start(args, fmt);
    vsprintf(log->buffer + log->cursor, fmt, args);
    va_end(args);
    log->cursor += len;
}

void bson_log_clear(BsonLog *log) {
    log->cursor = 0;
    memset(log->buffer, 0, log->nbuffer);
}
