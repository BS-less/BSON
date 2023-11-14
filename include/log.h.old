#ifndef BSON_LOG_H
#define BSON_LOG_H

#include <stdarg.h>
#include <stddef.h>

typedef struct BsonLog {
    size_t nbuffer;
    char   *buffer;
    size_t  cursor;
} BsonLog;

void bson_log_init(BsonLog *log);
void bson_log_free(BsonLog *log);
void bson_logf(BsonLog *log, const char * const fmt, ...);
void bson_log_clear(BsonLog *log);

#endif
