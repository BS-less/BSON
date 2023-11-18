#include "vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "defalloc.h"

#define GET_METADATA(p)                                    \
    ((BsonVecMD *)( (uint8_t *)(p) - sizeof(BsonVecMD) ))

void *bson_vector_allocate(size_t stride, size_t capacity, BsonAllocator *allocator) {
    assert(stride != 0 && "Stride cannot be zero.");
    BsonVecMD metadata = {
        .length    =  0,
        .capacity  =  capacity == 0 ? 16 : capacity,
        .stride    =  stride,
        .allocator =  allocator
    };
    uint8_t *bytes = allocator->pfn_malloc(
        sizeof(BsonVecMD) + 
        metadata.stride * metadata.capacity,
        allocator->userdata
    );
    BsonVecMD *md = (BsonVecMD *)(bytes);
    *md = metadata;
    return md + 1;
}

void  bson_vector_deallocate(void **vec) {
    if(vec == NULL || *vec == NULL)
        return;
    BsonVecMD *md = GET_METADATA(*vec);
    afree(md, md->allocator);
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
    md = arealloc(
        md, 
        sizeof(BsonVecMD) + newcapacity * md->stride, 
        md->allocator
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
	assert(constrain != 0 && "Constrain is zero");
    pfn_bson_realloc  pfn_realloc = md->allocator->pfn_realloc;
    void             *userdata    = md->allocator->userdata; 
    memmove(md, md + 1, constrain);
    md = pfn_realloc(md, constrain, userdata);
    assert(md != NULL && "Memory constrain failure (carr)");
	return md;
}

























