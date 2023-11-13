#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include <log.h>
#include <bson.h>

#define RUN_ARGV 0



#if RUN_ARGV == 0
#define TAB(n)									\
	for(i = 0; i < (tabs + n) * tabsize; i++)	\
		putchar(' ');
void print_bson(BsonNode *node, size_t tabs, size_t tabsize, int printkey, size_t *bytes) {
	assert(node != NULL && "-Critical-");
    *bytes += sizeof(BsonNode);
    if(node->key != NULL)
		*bytes += strlen(node->key) + 1;
	if(node->type == BSON_STR)
		*bytes += strlen(node->str) + 1;
	size_t i, j;
	if(printkey) {
		TAB(0);
		printf("KEY(%s): ", node->key);
	}
	switch(node->type) {
		case BSON_LNG:
			printf("LONG(%ld)\n", node->lng);
			break;
		case BSON_DBL:
			printf("DOUBLE(%lf)\n", node->dbl);
			break;
		case BSON_STR:
			printf("STRING(%s)\n", node->str);
			break;
        case BSON_BOOL:
            printf("BOOL(%s)\n", node->lng ? "TRUE" : "FALSE");
            break;
		case BSON_ARR:
			printf("ARRAY [\n");
			for(j = 0; j < node->numchildren; j++) {
				TAB(1);
				print_bson(&node->arr[j], tabs + 1, tabsize, 0, bytes);
			}
			TAB(0);
			printf("]\n");
			break;
		case BSON_OBJ:
			printf("OBJECT {\n");
			for(j = 0; j < node->numchildren; j++)
				print_bson(&node->obj[j], tabs + 1, tabsize, 1, bytes);
			TAB(0);
			printf("}\n");
			break;
		default:
            printf("UNKNOWN(%ld; %d)\n", node->lng, node->type);
			break;
	}

}
void functionality_test(BsonNode *node) {
	size_t bytes = 0;
	puts("Printing tree...");
	print_bson(node, 0, 2, 0, &bytes);
	printf("Number of bytes: %lu\n", bytes);
}
#endif



int main(int argc, char **argv) {
	puts("Hello World!");

    BsonSpan sp1, sp2;
    bson_span_set(&sp1, "Shmoovin");
    bson_span_set(&sp2, "What");
    printf("%d\n", bson_span_cmp(&sp1, &sp2));

#if RUN_ARGV == 0
	
	if(argc <= 1) {
		puts("No files");
		return 0;
	}
	argv = argv + 1;
	size_t   nfiles = argc - 1;
	FILE     *file  = NULL;
	size_t   ntext  = 0;
	char     *text  = NULL;
	BsonNode *root  = NULL;
	
	bsonenum  result;
	BsonLog   log;
	size_t i;
	
	bson_log_init(&log);
	for(i = 0; i < nfiles; i++) {
		printf("- - - -  '%s'  - - - -\n", argv[i]);
		file = fopen(argv[i], "r");
		if(file == NULL) {
			printf("Couldn't open '%s'\n", argv[i]);
			continue;
		}
		
		fseek(file, 0, SEEK_END);
		ntext = ftell(file);
		fseek(file, 0, SEEK_SET);

		text = malloc(ntext + 1);
		fread(text, 1, ntext, file);
		text[ntext] = '\0';
		fclose(file);
		
        BsonHints hints = {
            .builtins = NULL,
            .nbuiltins = 0,
            .log = &log
        };

		root = bson_parse(text, &result, &hints);
		if(root != NULL)
			functionality_test(root);	
		else {
			printf("Error: %s (%d)\n", bson_res(result), result);
		}
		
		printf("bson_free\n");
		bson_free(&root);
		printf("free text\n");
		free(text);
	}
	printf("-=-=-= log =-=-=-=-\n%s\n", log.buffer);
	bson_log_free(&log);

#endif
    puts("Goodbye World!");
    return 0;
}
