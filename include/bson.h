#ifndef BSON_H
#define BSON_H

#include "enums.h"
#include "memory.h"
#include "tokens.h"

#include <stdint.h>
#include <stddef.h>

/* Opaque */
typedef struct BsonLib          BsonLib;

/* Defined */
typedef struct BsonNode         BsonNode;
typedef struct BsonBuiltin      BsonBuiltin;

typedef BsonResult (*pfn_bson_builtin)(BsonNode *dst, const BsonToken *token, void *userdata);

struct BsonBuiltin {
    void               *userdata;
    pfn_bson_builtin    func;
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
    BsonType type;
};

BsonLib    *bson_lib_default(BsonResult *result, BsonLogLevel log);
BsonLib    *bson_lib_create(BsonResult *result, BsonLogLevel log, const BsonAllocator *allocator, const BsonBuiltin *builtins, size_t nbuiltins);
void        bson_lib_free(BsonLib **lib);
BsonResult  bson_lib_attach_allocator(BsonLib *lib, const BsonAllocator *allocator);
BsonResult  bson_lib_attach_builtin(BsonLib *lib, const BsonBuiltin *builtin);
BsonResult  bson_lib_attach_builtins(BsonLib *lib, const BsonBuiltin *builtins, size_t nbuiltins);

BsonNode   *bson_file(const char * const file, const BsonLib *lib, BsonResult *result);
BsonNode   *bson_parse(const char * const text, const BsonLib *lib, BsonResult *result);
void        bson_free(BsonNode **bson, const BsonLib *lib);
BsonNode   *bson_get(BsonNode *start, const char * const key, BsonResult *result);

#endif



