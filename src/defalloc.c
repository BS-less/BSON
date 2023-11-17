#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

/* For debug purposes, I want any memory failures to quit immediately */

void *defmalloc(size_t bytes, void *userdata) { 
    void *ptr = malloc(bytes); 
    if(ptr == NULL) {
        printf("Malloc fail. %lu bytes requested, ud = %p\n", bytes, userdata);
        assert(0 && "defmalloc() fail");
    }
    return ptr;
}

void *defrealloc(void *src, size_t bytes, void *userdata) { 
    void *ptr = realloc(src, bytes);
    if(ptr == NULL) {
        printf("Realloc fail. %lu bytes requested, src = %p, ud = %p\n", bytes, src, userdata);
        assert(0 && "defrealloc() fail");
    }
    return ptr;
}

void deffree(void *src, void *userdata) { 
    free(src); 
}

