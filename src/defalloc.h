#ifndef BSON_DEF_ALLOC_H
#define BSON_DEF_ALLOC_H

#include <stddef.h>

void *defmalloc(size_t bytes, void *userdata);
void *defrealloc(void *src, size_t bytes, void *userdata); 
void  deffree(void *src, void *userdata); 

#define amalloc(s, a)      (a)->pfn_malloc(s, (a)->userdata)
#define arealloc(p, s, a)  (a)->pfn_realloc(p, s, (a)->userdata)
#define afree(p, a)        (a)->pfn_free(p, (a)->userdata)

#endif

