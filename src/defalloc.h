#ifndef BSON_DEF_ALLOC_H
#define BSON_DEF_ALLOC_H

#include <stddef.h>

void *defmalloc(size_t bytes, void *userdata);
void *defrealloc(void *src, size_t bytes, void *userdata); 
void  deffree(void *src, void *userdata); 

#endif

