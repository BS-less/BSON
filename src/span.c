#include "span.h"
#include "memory.h"
#include <string.h>
#include <stdio.h>

void bson_span_set(BsonSpan * const span, char *str) {
    span->start = str;
    span->end = str + strlen(str);
}

size_t bson_span_len(const BsonSpan * const span) {
    return (size_t)(span->end - span->start);
}

char *bson_span_dup(const BsonSpan *span, const BsonAllocator *allocator) {
	size_t i, len = bson_span_len(span);
	char *cstr = allocator->pfn_malloc(len + 1, allocator->userdata);
	for(i = 0; i < len; i++)
		cstr[i] = span->start[i];
	cstr[i] = '\0';
	return cstr;
}

void bson_span_cpy(char *dst, const BsonSpan *src) {
	size_t i, len = bson_span_len(src);
	for(i = 0; i < len; i++)
		dst[i] = src->start[i];
	dst[i] = '\0';
}

#define ws(c)					\
(								\
	c == ' '  || c == '\n' ||	\
	c == '\r' || c == '\t' ||	\
	c == '\b' || c == '\a'		\
)								
void bson_span_trim(BsonSpan * const span) {
    while(ws(*span->start))
		span->start++;
    while(ws(*(span->end - 1)))
		span->end--;
}
#undef ws

int bson_span_cmp(const BsonSpan *sp1, const BsonSpan *sp2) {
    /* I hate this function */
    char *s1 = sp1->start, *s2 = sp2->start;
    while(
        *s1 && *s2 && 
         s1 != sp1->end &&
         s2 != sp2->end
    ) {
        if     (*s1 < *s2) return -1;
        else if(*s1 > *s2) return  1;
        s1++;
        s2++;
    }
    if(s1 == sp1->end && s2 == sp2->end)
        return 0;
    if(s1 == sp1->end)
        return -1;
    return 1;
}

size_t bson_span_dpri(const BsonSpan *span) {
	char *start = span->start;
	while(start != span->end) {
		putchar(*start);
		start++;
	}
	return span->end - span->start;
}



