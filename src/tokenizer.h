#ifndef BSON_TOKENIZER_H
#define BSON_TOKENIZER_H

#include "span.h"
#include "log.h"

#include <stddef.h>

#include "tokens.h"

BsonToken *bson_tokenize(const char *text, size_t *len, BsonLog *log);
void       bson_tokens_free(BsonToken **tokens);

void       bson_tokens_dpri(const BsonToken *tokens, size_t ntokens);

#endif

