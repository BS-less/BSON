#ifndef _BSON_ALLOCATOR_H_
#define _BSON_ALLOCATOR_H_

#include "enums.h"
#include <stddef.h>

typedef struct BsonAllocator    BsonAllocator;

typedef void *   (*pfn_bson_malloc)(size_t bytes, void *userdata);
typedef void *   (*pfn_bson_realloc)(void *src, size_t bytes, void *userdata);
typedef void     (*pfn_bson_free)(void *src, void *userdata);

struct BsonAllocator {
    void               *userdata;
    pfn_bson_malloc     pfn_malloc;
    pfn_bson_realloc    pfn_realloc;
    pfn_bson_free       pfn_free;
};

#endif
