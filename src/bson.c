#include "bson.h"
#include "span.h"
#include "tokenizer.h"
#include "vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static bsonenum builtin(const BsonParseContext *ctx, BsonNode *node, void *userdata) {
    BsonSpan btrue, bfalse, bvoid;
    bson_span_set(&btrue,  "$true");
    bson_span_set(&bfalse, "$false");
    bson_span_set(&bvoid,  "$void");
    node->type = BSON_BOOL;
    node->numchildren = 0;
    if(bson_span_cmp(&ctx->tokens[ctx->index].text, &btrue) == 0) {
        node->lng  = 1;
        return 1;
    }
    if(bson_span_cmp(&ctx->tokens[ctx->index].text, &bfalse) == 0) {
        node->lng  = 0;
        return 1;
    }
    if(bson_span_cmp(&ctx->tokens[ctx->index].text, &bvoid) == 0) {
        return BSON_UNIMPLEMENTED;
    }
    return 0;
}

static int fix_string(char **src) {
	char *str = *src;
    size_t i, len = strlen(str);
    for(i = len; i --> 0;) {
        if(str[i] == '\\') {
            if(i != 0 && str[i - 1] == '\\') {
                memmove(str + i, str + i + 1, len - i);
                i--;
            }
            else switch(str[i + 1]) {
                case 'n':
                    str[i] = '\n';
                    memmove(str + i + 1, str + i + 2, len - i - 1);
                    break;
                case 'r':
                    str[i] = '\r';
                    memmove(str + i + 1, str + i + 2, len - i - 1);
                    break;
                case 't':
                    str[i] = '\t';
                    memmove(str + i + 1, str + i + 2, len - i - 1);
                    break;
                case 'a':
                    str[i] = '\a';
                    memmove(str + i + 1, str + i + 2, len - i - 1);
                    break;
                case '"':
                    str[i] = '"';
                    memmove(str + i + 1, str + i + 2, len - i - 1);
                    break;
            }
        }
    }
	*src = bsonrealloc(*src, strlen(*src) + 1);
    return 1;
}

static int parse_lng(BsonNode *dst, BsonToken *src) {
    dst->lng = strtol(src->text.str, NULL, 10);
    dst->numchildren = 0;
    dst->type = BSON_LNG;
    return 1;
}

static int parse_dbl(BsonNode *dst, BsonToken *src) {
    dst->dbl = strtod(src->text.str, NULL);
    dst->numchildren = 0;
    dst->type = BSON_DBL;
    return 1;
}

static int parse_str(BsonNode *dst, BsonToken *src) {
    dst->str = bson_span_dup(&src->text);
    fix_string(&dst->str);
    dst->numchildren = 0;
    dst->type = BSON_STR;
    return 1;
}

static int parse_multistr(BsonNode *dst, BsonParseContext *ctx) {
    char   *str = bson_span_dup(&ctx->tokens[ctx->index].text);
    char   *tmp;
    size_t  slen = strlen(str), tlen;
    ctx->index++;
    while(
        ctx->index < ctx->ntokens &&
        (ctx->tokens[ctx->index].type == TOKEN_STRING_MULTI ||
         ctx->tokens[ctx->index].type == TOKEN_STRING_MULTI_END)
    ) {
        tmp = bson_span_dup(&ctx->tokens[ctx->index].text);
        tlen = strlen(tmp);
        slen += tlen;
        str = bsonrealloc(str, slen + 1);
        strcat(str, tmp);
        bsonfree(tmp);
        if(ctx->tokens[ctx->index].type == TOKEN_STRING_MULTI_END) {
            ctx->index++;
            break;
        }
        ctx->index++;
    }
    ctx->index--; /* Hack */
    fix_string(&str);
    dst->str         = str;
    dst->numchildren = 0;
    dst->type        = BSON_STR;
    return 1;
}

static int parse_builtin(BsonNode *dst, BsonParseContext *ctx) {
    size_t i;
    bsonenum ret;
    for(i = 0; i < ctx->nbuiltins; i++) {
        ret = ctx->builtins[i](ctx, dst, NULL);
        switch(ret) {
            default:
            case BSON_UNIMPLEMENTED:
                return 0;
                break;
            case 0:
                break;
            case 1:
                return 1;
                break;
        }
    }
    return 0;
}

static BsonNode *parse_obj(BsonParseContext *ctx, size_t *children); 
static BsonNode *parse_arr(BsonParseContext *ctx, size_t *children);

static int interpret_node(BsonParseContext *ctx, BsonNode *node) {
    size_t children;
    switch(ctx->tokens[ctx->index].type) {
        case TOKEN_INTEGER: parse_lng(node, &ctx->tokens[ctx->index]); break;
        case TOKEN_DECIMAL: parse_dbl(node, &ctx->tokens[ctx->index]); break;
        case TOKEN_STRING:  parse_str(node, &ctx->tokens[ctx->index]); break;
        case TOKEN_STRING_MULTI:
        case TOKEN_STRING_MULTI_END:
            parse_multistr(node, ctx);
            break;
        case TOKEN_BUILTIN:
            if(!parse_builtin(node, ctx)) {
                return 0;
            }
            break;
        case TOKEN_ARRAY_OPEN:
            ctx->index++;
            node->arr = parse_arr(ctx, &children);
            node->type = BSON_ARR;
            node->numchildren = children;
            break;
        case TOKEN_OBJECT_OPEN:
            ctx->index++;
            node->obj = parse_obj(ctx, &children);
            node->type = BSON_OBJ;
            node->numchildren = children;
            break;
        default: {
			bson_logf(ctx->log, "[BSON-SYNTAX]: Unexpected token \"");
			BsonSpan *badtext = &ctx->tokens[ctx->index].text;
			char *c = badtext->start;
			while(*c && c != badtext->end) {
				bson_logf(ctx->log, "%c", *c);
				c++;
			}
			bson_logf(ctx->log, "\"\n");
			ctx->index++;
			return 0;
		} break;
    }
    return 1;
}

static BsonNode *parse_obj(BsonParseContext *ctx, size_t *children) {
    BsonNode *nodes = bson_vector_new(BsonNode);
    BsonNode add;
    while(ctx->index < ctx->ntokens && ctx->tokens[ctx->index].type != TOKEN_OBJECT_CLOSE) {
        if(ctx->tokens[ctx->index].type != TOKEN_KEY) {
            bson_logf(
                ctx->log, 
                "[BSON-SYNTAX]: Expected key on line %lu\n", 
                ctx->tokens[ctx->index].line
            );
            ctx->index++;
            continue;
        }
        add.key = bson_span_dup(&ctx->tokens[ctx->index].text);
        ctx->index++;
        if(interpret_node(ctx, &add))
            bson_vector_push(nodes, add);
        else if(add.key != NULL) bsonfree(add.key);
        ctx->index++;
    }
    *children = bson_vector_length(nodes);
    return bson_vector_carr(nodes);
}

static BsonNode *parse_arr(BsonParseContext *ctx, size_t *children) {
    BsonNode *nodes = bson_vector_new(BsonNode);
    BsonNode add;
    add.key = NULL;
    while(ctx->index < ctx->ntokens && ctx->tokens[ctx->index].type != TOKEN_ARRAY_CLOSE) {
        if(interpret_node(ctx, &add))
            bson_vector_push(nodes, add);
        else if(add.key != NULL) bsonfree(add.key);
        ctx->index++;
    }
    *children = bson_vector_length(nodes);
    return bson_vector_carr(nodes);
}



static BsonNode *create_tree(BsonParseContext *ctx, bsonenum *result) {
	BsonNode *root = bsonmalloc(sizeof(BsonNode));
    size_t children;
    root->key = NULL;
    root->obj = parse_obj(ctx, &children);
    root->type = BSON_OBJ; 
    root->numchildren = children;

	if(result != NULL)
		*result = BSON_SUCCESS;
	return root;
}




/***   BSON FRONT    ***/

BsonNode *bson_parse(const char * const text, bsonenum *result, const BsonHints *extra) {
	bson_logf(extra->log, "[BSON-NEW]: txt %p ext %p\n", (void *) text, (void *) extra);

    /* Get tokens */
	size_t i;
    size_t     tokenslen;
	BsonToken *tokens = bson_tokenize(text, &tokenslen, extra->log);
	if(tokens == NULL) {
		if(result != NULL)
			*result = BSON_SYNTAX;
		return NULL;
	}
	bson_tokens_dpri(tokens, tokenslen);
    bson_logf(
        extra->log, 
        "[BSON-STATUS]: Tokenization finished. %lu tokens from %lu lines.\n",
        tokenslen, tokens[tokenslen - 1].line
    );

    /* Prepare context */
    BsonParseContext ctx = {
        .tokens = tokens,
        .ntokens = tokenslen,
        .builtins = { builtin } ,
        .nbuiltins = 1,
        .index = 0,
    };

    if(extra != NULL) {
        ctx.log = extra->log;
        for(i = 0; i < extra->nbuiltins; i++) {
            ctx.builtins[i + 1] = extra->builtins[i];
            ctx.nbuiltins++;
        }
    }

    /* Parse */
	BsonNode *root = create_tree(&ctx, result);

	/* Tokens no longer needed */
	bson_tokens_free(&tokens);
	
    bson_logf(extra->log, "[BSON-STATUS]: `bson_parse()` returning ");
	if(root == NULL) bson_logf(extra->log, "NULL.\n");
    else             bson_logf(extra->log, "%p.\n", (void *) root);

    /* create_tree has already set result */
	return root;
}

static void bson_free_rec(BsonNode *node) {
    if(node->key != NULL)
        bsonfree(node->key);
    size_t i;
    switch(node->type) {
        default:
            break;
        case BSON_STR:
            bsonfree(node->str);
            break;
        case BSON_OBJ:
        case BSON_ARR:
            for(i = 0; i < node->numchildren; i++)
                bson_free_rec(&node->obj[i]);
            bson_carr_free(node->obj);
            break;
    }
}

void bson_free(BsonNode **bson) {
	if(bson == NULL || *bson == NULL)
		return;
    BsonNode *root = *bson;
    assert(root->type == BSON_OBJ && "Root is not object");
    size_t i;
    for(i = 0; i < root->numchildren; i++)
        bson_free_rec(&root->obj[i]);
    bsonfree(root->obj);
    bsonfree(root);
    *bson = NULL;
}

BsonNode *bson_get(BsonNode *start, const char * const key) {
    if(start->type != BSON_ARR && start->type != BSON_OBJ)
        return NULL;
    size_t i;
    for(i = 0; i < start->numchildren; i++) {
        if(strcmp(key, start->obj[i].key) == 0)
            return &start->obj[i];
    }
    return NULL;
}












/* DEBUG, IGNORE */

#define TAB(n) for(i = 0; i < tabs + n; i++) putchar('\t')
static void dpri_rec(const BsonNode *node, int tabs) {
    int i;
    TAB(0);
    printf("'%s'", node->key);
    switch(node->type) {
        case BSON_LNG: printf(" lng %ld\n", node->lng); break;
        case BSON_DBL: printf(" dbk %lf\n", node->dbl); break;
        case BSON_STR: printf(" str \"%s\"\n", node->str); break;
        case BSON_ARR:
        case BSON_OBJ:
            printf(" [\n");
            for(i = 0; i < node->numchildren; i++) {
                dpri_rec(&node->arr[i], tabs + 1);
            }
            TAB(0);
            printf("]\n");
            break;
        default: printf("<invalid>\n"); break;
    }
}

void bson_dpri(const BsonNode *root) {
    assert(root != NULL && root->type == BSON_OBJ && "Invalid node to print");
    
    int i;
    for(i = 0; i < root->numchildren; i++) {
        dpri_rec(&root->obj[i], 0); 
    }
}










