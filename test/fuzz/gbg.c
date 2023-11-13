#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <time.h>

typedef enum {
	GARBAGE_PURE,
	GARBAGE_TEXT,

} Mode;

static void generate_pure(char *dst, size_t len) {
	size_t i;
	for(i = 0; i < len; i++)
		dst[i] = (char)(rand() % 256);
	dst[i] = '\0';
}

static void generate_text(char *dst, size_t len) {
	static const char * const chars =
	"qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890!@#$%^&*()`~-_=+[{]}\\|;:'\",<.>/?";
	size_t charslen = strlen(chars);
	size_t i;
	for(i = 0; i < len; i++) {
		if(rand() % 100 < 10)
			dst[i] = '\n';
		if(rand() % 100 < 10)
			dst[i] = '\n';
		else
			dst[i] = chars[rand() % charslen];
	}
	dst[i] = '\0';
}

int main(int argc, char **argv) {
	puts("Hello World!");
	unsigned int s = time(NULL);
	s *= (unsigned int)((uint64_t)&s);
	srand(s);
	
	Mode  mode = GARBAGE_PURE;
	char *out = "gbg.bson";
	if(argc > 0) {	
		out  = argv[1];
		if(argc >= 3 && strcmp("text", argv[2]) == 0)
			mode = GARBAGE_TEXT;
	}

	size_t filesize = rand() % 1024 + 1024;
	char  *file     = malloc(filesize + 1);
	if(mode == GARBAGE_PURE)
		generate_pure(file, filesize);
	else
		generate_text(file, filesize);
	
	FILE  *os = fopen(out, "w");
	if(!os) {
		free(file);
		printf("Could not write '%s'\n", out);
		return -1;
	}
	
	fwrite(file, 1, filesize, os);

	fclose(os);
	free(file);
	puts("Goodbye World!");
	return 0;
}
