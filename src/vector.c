#include "vector.h"
#include "memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define GET_METADATA(p)                                    \
    ((BsonVecMD *)( (uint8_t *)(p) - sizeof(BsonVecMD) ))

void *bson_vector_allocate(size_t capacity, size_t stride) {
    assert(stride != 0 && "Stride cannot be zero.");
    BsonVecMD metadata = {
        .length    = 0,
        .capacity  = capacity == 0 ? 16 : capacity,
        .stride    = stride
    };
    uint8_t *bytes = bsonmalloc(
        sizeof(BsonVecMD) + 
        metadata.stride * metadata.capacity
    );
    BsonVecMD *md = (BsonVecMD *)(bytes);
    *md = metadata;
    return md + 1;
}

void  bson_vector_deallocate(void **vec) {
    if(vec == NULL || *vec == NULL)
        return;
    BsonVecMD *md = GET_METADATA(*vec);
    bsonfree(md);
    *vec = NULL;
}

const BsonVecMD *bson_vector_metadata(const void *vec) {
    return GET_METADATA(vec);
}

size_t bson_vector_length(void *vec) {
    BsonVecMD *md = GET_METADATA(vec);    
    return md->length;
}

size_t bson_vector_capacity(void *vec) {
    BsonVecMD *md = GET_METADATA(vec);    
    return md->capacity;
}

size_t bson_vector_stride(void *vec) {
    BsonVecMD *md = GET_METADATA(vec);    
    return md->stride;
}

void *bson_vector_grow(void *vec) {
    BsonVecMD *md = GET_METADATA(vec);
    size_t newcapacity = md->capacity * 3 / 2;
    md = bsonrealloc(
        md,
        sizeof(BsonVecMD) +
        newcapacity * md->stride
    );
    assert(md != NULL && "Memory failure");
    md->capacity = newcapacity;
    return md + 1;
}

void bson_vector_inc(void *vec) {
    BsonVecMD *md = GET_METADATA(vec);
    md->length++;
}

void *bson_vector_carr(void *vec) {
    BsonVecMD *md = GET_METADATA(vec);
    size_t constrain = md->length * md->stride;
    if(constrain == 0) {
        bsonfree(md);
        return NULL;
    }
	memmove(md, md + 1, constrain);
    md = bsonrealloc(md, constrain);
    assert(md != NULL && "Memory constrain failure (carr)");
	return md;
}

void bson_carr_deallocate(void **arr) {
    if(arr == NULL || *arr == NULL)
        return;
    bsonfree(*arr);
    *arr = NULL;
}

























