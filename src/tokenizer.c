#include "tokenizer.h"
#include "memory.h"
#include "span.h"
#include "vector.h"

#include <assert.h>
#include <stdio.h>

#define is_builtinchar(c) is_keychar(c)
static int is_keychar(char c) {
    return (
        (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        (c >= '0' && c <= '9') ||
        c == '-'               ||
        c == '_'
    );
}

static int is_digit(char c) {
	return (
		(c >= '0' && c <= '9')
	);
}

static int is_escapechar(char c) {
    return (
        c == 'n' || c == 'r' ||
        c == 'b' || c == '\\' ||
        c == '"' || c == 'a'
    );
}

static void token_set(BsonToken *dst, char *start, char *end, BsonTokenType type, size_t line) {
	dst->text = (BsonSpan){ .start = start, .end = end };
	dst->type = type;
	dst->line = line;
}

static int token_singlech(BsonToken *dst, BsonSpan *all, BsonTokenType type, size_t *lines) {
	dst->text.start = all->start;
	all->start += 1;
	dst->text.end   = all->start;
	dst->line       = *lines;
	dst->type       = type;

	/* token_set(dst, ..., ..., type, *lines); */

    return 1;
}

static int token_string(BsonToken *dst, BsonSpan *all, size_t *lines) {
	dst->text.start = all->start;
	/* TODO, DO NOT COUNT QUOTES AFTER A '\' */
	do all->start++;
	while(*all->start && (*all->start != '"' && *(all->start - 1) != '\\'));
	dst->text.end   = all->start;
	dst->line       = *lines;
	dst->type       = TOKEN_STRING;
    dst->text.start++;
    /* all->start will still be pointing to a quote and attempt to read another string in the loop... ++ */
    all->start++;
    if(dst->text.start == dst->text.end)
        return 0;
    return 1;
}

static int token_key(BsonToken *dst, BsonSpan *all, size_t *lines) {
	dst->text.start = all->start;
	do all->start++;
    while(is_keychar(*all->start) || is_digit(*all->start));
    dst->text.end   = all->start;
	dst->line       = *lines;
	dst->type       = TOKEN_KEY;
	return 1;
}

static int token_num(BsonToken *dst, BsonSpan *all, size_t *lines) {
	dst->text.start = all->start;
	int decimals = 0, digits = 0, nonnum = 0;
	while(*all->start && (is_digit(*all->start) || *all->start == '.')) {
		if(is_digit(*all->start))
			digits++;
		else if(*all->start)
			decimals++;
		else {
			nonnum++;
			break;
		}
		all->start++;
	}
	dst->text.end   = all->start;
	dst->line       = *lines;
	dst->type       = (decimals) ? TOKEN_DECIMAL : TOKEN_INTEGER;
	if(nonnum != 0 || digits == 0 || decimals > 1)
		return 0;
	return 1;
}

static int token_multi(BsonToken *dst, BsonSpan *all, size_t *lines) {
    dst->text.start = ++all->start;
    do all->start++;
    while(
        *all->start                 && 
        *all->start != '\n'         && 
        !(*all->start == '\\' && !is_escapechar(*(all->start + 1)))
    );
    dst->text.end = all->start;
    dst->line = (*lines)++;
    dst->type = *all->start == '\\' ? TOKEN_STRING_MULTI_END : TOKEN_STRING_MULTI;
    all->start++;
    return 1;
}

static int token_builtin(BsonToken *dst, BsonSpan *all, size_t *lines) {
    dst->text.start = all->start;
    do all->start++;
    while(*all->start && is_builtinchar(*all->start));
    dst->text.end = all->start;
    dst->line = *lines;
    dst->type = TOKEN_BUILTIN;
    return 1;
}

BsonToken *bson_tokenize(const char *text, size_t *len, BsonLog *log, const BsonAllocator *allocator) {
	assert(text != NULL && "No text provided for tokenization");
	assert(len  != NULL && "Len destination must be provided");

    BsonToken *tokens = bson_vector_new_cap(BsonToken, 96, allocator);
    
	size_t i, lines = 1;
	BsonToken tok;
	BsonSpan  all;
	bson_span_set(&all, (char *) text);
	assert(all.start != all.end && "Invalid text");
	int success;
	
	while(all.start != all.end) {
        switch(*all.start) {
            /* Syntax error */
            default:
                bson_span_dpri(&all);
                bson_logf(log, BSON_LOG_NORMAL, "[BSON-SYNTAX]: Unknown character '%c' @ line %lu.\n", *all.start, lines);
                all.start++;
                continue;
                break;
            /* Key */
            case 'a': case 'b': case 'c': case 'd':
            case 'e': case 'f': case 'g': case 'h':
            case 'i': case 'j': case 'k': case 'l':
            case 'm': case 'n': case 'o': case 'p':
            case 'q': case 'r': case 's': case 't':
            case 'u': case 'v': case 'w': case 'x':
            case 'y': case 'z':
            case 'A': case 'B': case 'C': case 'D':
            case 'E': case 'F': case 'G': case 'H':
            case 'I': case 'J': case 'K': case 'L':
            case 'M': case 'N': case 'O': case 'P':
            case 'Q': case 'R': case 'S': case 'T':
            case 'U': case 'V': case 'W': case 'X':
            case 'Y': case 'Z':
            case '_':
                success = token_key(&tok, &all, &lines);
                break;
            /* Long/double */
            case '0': case '1': case '2': case '3':
            case '4': case '5': case '6': case '7':
            case '8': case '9': case '.':
                success = token_num(&tok, &all, &lines);
                break;
            /* Whitespace, skip */
            case '\n': lines++;
            case  ' ': case '\r': case '\t': case ':':
                all.start++;
                continue;
                break;
            /* Check for comment, if not comment then skip */
            case '/':
                all.start++;
                assert(all.start != all.end && "TODO: FIX THIS EDGE CASE");
                if(*all.start != '/') {
                    bson_vector_free(tokens);
                    return NULL;
                }
                while(*all.start != '\n')
                    all.start++;
                continue;
                break;
            /* Single char tokens */
            case  '[': success = token_singlech(&tok, &all, TOKEN_ARRAY_OPEN, &lines);    break;
            case  ']': success = token_singlech(&tok, &all, TOKEN_ARRAY_CLOSE, &lines);   break;
            case  '{': success = token_singlech(&tok, &all, TOKEN_OBJECT_OPEN, &lines);   break;
            case  '}': success = token_singlech(&tok, &all, TOKEN_OBJECT_CLOSE, &lines);  break;
            case  '"': success = token_string(&tok, &all, &lines);                        break;
            case '\\': success = token_multi(&tok, &all, &lines);                         break;
            case  '$': success = token_builtin(&tok, &all, &lines);                       break;
        }
        if(success)
            bson_vector_push(tokens, tok);	
        else {
            bson_logf(log, BSON_LOG_NORMAL, "[BSON-SYNTAX]: Could not parse '");
            char *s = tok.text.start;
            while(s != tok.text.end)
                bson_logc(log, BSON_LOG_NORMAL, *s);
            bson_logf(log, BSON_LOG_NORMAL, "' @ line %lu.\n", lines);
        }
        all.start++;
    }
	*len = bson_vector_length(tokens);
    return bson_vector_carr(tokens);
}

#define TABBING 48
static void token_print(const BsonToken *tok) {
	printf("{ <");
	int i, chars = bson_span_dpri(&tok->text);
	printf(">");

    int spaces = TABBING - chars;
	for(i = 0; i < spaces; i++)
	    putchar(' ');
	printf(" : ");

	switch(tok->type) {
		case TOKEN_KEY:				  printf("KEY             "); break;
		case TOKEN_INTEGER:			  printf("INTEGER         "); break;
		case TOKEN_DECIMAL:			  printf("DECIMAL         "); break;
		case TOKEN_STRING:			  printf("STRING          "); break;
        case TOKEN_STRING_MULTI:      printf("STRING_MULTI    "); break;
        case TOKEN_STRING_MULTI_END:  printf("STRING_MULTI_END"); break;
		case TOKEN_ARRAY_OPEN:		  printf("ARRAY_OPEN      "); break;	
		case TOKEN_ARRAY_CLOSE: 	  printf("ARRAY_CLOSE     "); break;
		case TOKEN_OBJECT_OPEN:		  printf("OBJECT_OPEN     "); break;
		case TOKEN_OBJECT_CLOSE:	  printf("OBJECT_CLOSE    "); break;
		case TOKEN_BUILTIN:     	  printf("BUILT-IN        "); break;
		default:					  printf(" <invalid>      "); break;
	}
	printf("  : line %3lu }", tok->line);
}

void bson_tokens_dpri(const BsonToken *tokens, size_t ntokens) {
	assert(tokens != NULL && "Tokens to print are NULL.");
	size_t i;
	for(i = 0; i < ntokens; i++) {
		printf("#%3ld: ", i);
		token_print(&tokens[i]);
		putchar('\n');
	}
}













