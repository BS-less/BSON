#ifndef BSON_SPAN_H
#define BSON_SPAN_H

#include "memory.h"
#include <stddef.h>

typedef struct BsonSpan {
    union {
		char *str;
		char *start;
    };
    char *end;
} BsonSpan;

void   bson_span_set(BsonSpan *span, char *str);
size_t bson_span_len(const BsonSpan * const span);
char  *bson_span_dup(const BsonSpan * span, const BsonAllocator *allocator);
void   bson_span_cpy(char *dst, const BsonSpan *src);
void   bson_span_trim(BsonSpan *span);
int    bson_span_cmp(const BsonSpan *s1, const BsonSpan *s2);
size_t bson_span_dpri(const BsonSpan *span);
#endif
