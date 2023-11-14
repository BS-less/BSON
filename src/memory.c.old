#include "memory.h"
#include <stdlib.h>

void *defmalloc(size_t s, void *ud)           { return malloc(s);     }
void *defcalloc(size_t n, size_t s, void *ud) { return calloc(s, n);  }
void *defrealloc(void *p, size_t s, void *ud) { return realloc(p, s); }
void  deffree(void *p, void *ud)              {        free(p);       }

static bsonallocator mem = {
    .malloc 	= defmalloc,
    .calloc 	= defcalloc,
    .realloc 	= defrealloc,
    .free 	= deffree,
    .userdata   = NULL
};

bsonenum bson_set_allocator(const bsonallocator *all) {
    if(
	all          == NULL ||
	all->malloc  == NULL ||
	all->calloc  == NULL ||
	all->realloc == NULL ||
	all->free    == NULL
    ) return BSON_NULL_POINTER;
    mem = *all;
    return BSON_SUCCESS;
}

void *bsonmalloc(size_t size)                 { return mem.malloc(size, mem.userdata);          }
void *bsoncalloc(size_t nummemb, size_t size) { return mem.calloc(nummemb, size, mem.userdata); }
void *bsonrealloc(void *ptr, size_t size)     { return mem.realloc(ptr, size, mem.userdata);    }
void  bsonfree(void *ptr) 		      {        mem.free(ptr, mem.userdata);             }
