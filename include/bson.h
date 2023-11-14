#ifndef BSON_H
#define BSON_H

#include "enums.h"
#include "memory.h"
//#include "log.h"
#include "tokens.h"

#include <stdint.h>
#include <stddef.h>

/* Opaque */
typedef struct BsonLib          BsonLib;

/* Defined */
typedef struct BsonNode         BsonNode;
typedef struct BsonBuiltin      BsonBuiltin;

typedef bsonenum (*pfn_bson_builtin)(BsonNode *dst, const BsonToken *token, void *userdata);

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
    bsonenum type;
};

BsonLib  *bson_lib_default(bsonenum *result, int logging);
BsonLib  *bson_lib_create(bsonenum *result, int logging, const BsonAllocator *allocator, const BsonBuiltin *builtins, size_t nbuiltins);
void      bson_lib_free(BsonLib **lib);
bsonenum  bson_lib_attach_allocator(BsonLib *lib, const BsonAllocator *allocator);
bsonenum  bson_lib_attach_builtin(BsonLib *lib, const BsonBuiltin *builtin);
bsonenum  bson_lib_attach_builtins(BsonLib *lib, const BsonBuiltin *builtins, size_t nbuiltins);

BsonNode *bson_file(const char * const file, const BsonLib *lib, bsonenum *result);
BsonNode *bson_parse(const char * const text, const BsonLib *lib, bsonenum *result);
void      bson_free(BsonNode **bson, const BsonLib *lib);
BsonNode *bson_get(BsonNode *start, const char * const key, bsonenum *result);

void      bson_dpri(const BsonNode *root);

#endif



