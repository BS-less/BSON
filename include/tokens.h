#ifndef BSON_TOKENS_H
#define BSON_TOKENS_H

#include "span.h"
#include <stddef.h>

typedef enum BsonTokenType {
	TOKEN_KEY = 0,
	TOKEN_INTEGER,
	TOKEN_DECIMAL,
	TOKEN_STRING,
	TOKEN_STRING_MULTI,
	TOKEN_STRING_MULTI_END,
    TOKEN_ARRAY_OPEN,
	TOKEN_ARRAY_CLOSE,
	TOKEN_OBJECT_OPEN,
	TOKEN_OBJECT_CLOSE,
	TOKEN_BUILTIN,
	TOKEN_MAX
} BsonTokenType;

typedef struct BsonToken {
	BsonSpan          text;
	BsonTokenType     type;
	size_t            line;
} BsonToken;

#endif
