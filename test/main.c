#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "bson.h"

#define TAB                                             \
do {                                                    \
    size_t _iii_;                                       \
    for(_iii_ = 0; _iii_ < tabs * tabsize; _iii_++)     \
        putchar(' ');                                   \
} while(0)

void print_node(BsonNode *node, size_t tabs, size_t tabsize, int printkey, size_t *bytes) {
    assert(node != NULL && "-- Critical error --");
    *bytes += sizeof(BsonNode);
    if(node->key != NULL)
        *bytes += strlen(node->key) + 1;
    if(node->type == BSON_TYPE_STR)
        *bytes += strlen(node->str) + 1;
    size_t i, j;
    TAB;
    if(printkey)
        printf("KEY(%s): ", node->key);
    switch(node->type) {
        case BSON_TYPE_LNG:
            printf("LONG(%ld)\n", node->lng);
            break;
        case BSON_TYPE_DBL:
            printf("DOUBLE(%lf)\n", node->dbl);
            break;
        case BSON_TYPE_STR:
            printf("STRING(%s)\n", node->str);
            break;
        case BSON_TYPE_BOOL:
            printf("BOOL(%s)\n", node->lng ? "True" : "False");
            break;
        case BSON_TYPE_ARR:
            printf("ARRAY [\n");
            for(j = 0; j < node->numchildren; j++) {
                print_node(&node->arr[j], tabs + 1, tabsize, 0, bytes);
            }
            TAB;
            printf("]\n");
            break;
        case BSON_TYPE_OBJ:
            printf("OBJECT {\n");
            for(j = 0; j < node->numchildren; j++) {
                print_node(&node->obj[j], tabs + 1, tabsize, 1, bytes);
            }
            TAB;
            printf("}\n");
            break;
        default:
            printf("UNKNOWN(%016lX)\n", node->lng);
            break;
    }
}

size_t test_print(BsonNode *node) {
    size_t bytes = 0;
    puts("Printing tree...");
    print_node(node, 0, 2, 0, &bytes);
    printf("Finished. # of bytes: %lu\n", bytes);
    return bytes;
}

int main(int argc, char **argv) {
    puts("Hello World!");
    if(argc <= 1) {
        puts("No input files.");
        return 0;
    }
    
    FILE  *file = NULL;
    size_t ntext = 0;
    char  *text = NULL;
    argv = argv + 1;
    argc--;

    BsonResult result;
    BsonLib *lib = bson_lib_default(&result, BSON_LOG_DEBUG);
    assert(lib != NULL && "Library could not be created.");
   
    BsonNode *root;

    size_t i, bytes = 0;
    for(i = 0; i < argc; i++) {
        printf("-------- '%s' --------\n", argv[i]);
        file = fopen(argv[i], "r");
        if(file == NULL) {
            puts(" > Could not load file");
            continue;
        }
        fseek(file, 0, SEEK_END);
        ntext = ftell(file);
        printf("THIS IS THE SIZE OF THE FILE IN CHARACTERS: %lu\n", ntext);
        fseek(file, 0, SEEK_SET);
        text = malloc(ntext + 1);
        fread(text, ntext, 1, file);
        text[ntext] = '\0';
        fclose(file);

        root = bson_parse(text, lib, &result);
        printf(" > main.c: bson_parse() = '%s'/(%d)\n", bson_result(result), result);
        if(result == BSON_SUCCESS)
            bytes += test_print(root);
        bson_free(&root, lib);
        free(text);
        printf(":: log ::\n%s\n", bson_lib_log_get(lib));
        bson_lib_log_clear(lib);
    }

    bson_lib_free(&lib);

    puts("Goodbye World!");
    printf("The finished result of each file summed to %lu bytes!\n", bytes);
    return 0;
}

