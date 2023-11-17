#include "log.h"
#include "memory.h"

#include <stdio.h>
#include <string.h>

void bson_log_init(BsonLog *log, BsonLogLevel priority, const BsonAllocator *allocator) {
    if(log->buffer == NULL)
        return;
    log->allocator = *allocator;
    log->cursor = 0;
    log->nbuffer = 256;
    log->buffer = allocator->pfn_malloc(log->nbuffer, allocator->userdata);
    log->priority = priority;
    memset(log->buffer, 0, log->nbuffer);
}

void bson_log_free(BsonLog *log) {
    if(log->buffer == NULL)
        return;
    log->allocator.pfn_free(log->buffer, log->allocator.userdata);
    log->buffer = NULL;
}

static void log_grow(BsonLog *log, size_t extra) {
    log->nbuffer = log->nbuffer * 3 + extra;
    log->buffer = log->allocator.pfn_realloc(log->buffer, log->nbuffer, log->allocator.userdata);
}

void bson_logc(BsonLog *log, char c) {
    if(log->buffer == NULL)
        return;
    if(log->cursor + 1 >= log->nbuffer)
        log_grow(log, 0);
    log->buffer[log->cursor++] = c;
    log->buffer[log->cursor] = '\0';
}

void bson_log_span(BsonLog *log, const BsonSpan *span) {
    if(log->buffer == NULL)
        return;
    size_t len = bson_span_len(span);
    if(log->cursor + len >= log->nbuffer)
        log_grow(log, len);
    char *s = span->start;
    while(s != span->end) {
        log->buffer[log->cursor] = *s;
        log->cursor++;
        *s++;
    }
}

void bson_logf(BsonLog *log, const char * const fmt, ...) {
    if(log->buffer == NULL)
        return;
    va_list args;
    va_start(args, fmt);
    size_t len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    if(len == 0)
        return;
    if(log->cursor + len >= log->nbuffer)
        log_grow(log, len);
    va_start(args, fmt);
    vsprintf(log->buffer + log->cursor, fmt, args);
    va_end(args);
    log->cursor += len;
}

void bson_log_clear(BsonLog *log) {
    log->cursor = 0;
    memset(log->buffer, 0, log->nbuffer);
}

