#ifndef BSON_H
#define BSON_H

#include "enums.h"
#include "memory.h"
#include "log.h"
#include "tokens.h"

#include <stdint.h>
#include <stddef.h>

typedef struct BsonNode         BsonNode;
typedef struct BsonParseContext BsonParseContext;
typedef bsonenum (*pfn_bson_builtin)(const BsonParseContext *ctx, BsonNode *node, void *userdata);
typedef struct BsonLib BsonLib;

typedef struct BsonHints {
    BsonLog           *log;
    pfn_bson_builtin  *builtins;
    size_t            nbuiltins;
} BsonHints; /* IDK a better name for this */

struct BsonParseContext {
    BsonToken         *tokens;
    size_t            ntokens;
    size_t             index;
    pfn_bson_builtin   builtins[8];
    size_t            nbuiltins;
    BsonLog           *log;
};

struct BsonNode {
    char *key;
	union {
		int64_t			 lng;
		double			 dbl;
		char			*str;
		struct BsonNode *arr;
		struct BsonNode *obj;
    };
    size_t numchildren;
    bsonenum type;
};


BsonNode *bson_parse(const char * const text, bsonenum *result, const BsonHints *extra);
void      bson_free(BsonNode **bson);
BsonNode *bson_get(BsonNode *start, const char * const key);

void      bson_dpri(const BsonNode *root);

#endif
