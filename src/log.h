#ifndef BSON_LOG_H
#define BSON_LOG_H

#include "memory.h"
#include "enums.h"

#include <stdarg.h>
#include <stddef.h>

typedef struct BsonLog {
    size_t         nbuffer;
    char           *buffer;
    size_t          cursor;
    BsonAllocator   allocator;
    BsonLogLevel    priority;
} BsonLog;

void bson_log_init(BsonLog *log, BsonLogLevel priority, const BsonAllocator *allocator);
void bson_log_free(BsonLog *log);

void bson_logc(BsonLog *log, BsonLogLevel level, char c);
void bson_log(BsonLog *log, BsonLogLevel level, const char * const str);
void bson_logf(BsonLog *log, BsonLogLevel level, const char * const fmt, ...);
void bson_log_clear(BsonLog *log);

#endif
