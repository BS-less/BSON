#ifndef _BSON_ALLOCATOR_H_
#define _BSON_ALLOCATOR_H_

#include "enums.h"
#include <stddef.h>

typedef void *(*pfn_bson_malloc)(size_t, void *);
typedef void *(*pfn_bson_calloc)(size_t, size_t, void *);
typedef void *(*pfn_bson_realloc)(void *, size_t, void *);
typedef void  (*pfn_bson_free)(void *, void *);

typedef struct {
    pfn_bson_malloc 	malloc;
    pfn_bson_calloc 	calloc;
    pfn_bson_realloc 	realloc;
    pfn_bson_free 	free;
    void               *userdata;
} bsonallocator;

bsonenum bson_set_allocator(const bsonallocator *all);

void *bsonmalloc(size_t size);
void *bsoncalloc(size_t nummemb, size_t size);
void *bsonrealloc(void *ptr, size_t size);
void  bsonfree(void *ptr);

#endif
