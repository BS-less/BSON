#ifndef BSON_TOKENIZER_H
#define BSON_TOKENIZER_H

#include "span.h"
//#include "log.h"
#include "tokens.h"

#include <stddef.h>


BsonToken *bson_tokenize(const char *text, size_t *len, const BsonAllocator *allocator);

void       bson_tokens_dpri(const BsonToken *tokens, size_t ntokens);

#endif

