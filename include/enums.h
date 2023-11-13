#ifndef _BSON_ENUM_H_
#define _BSON_ENUM_H_

typedef enum {
    BSON_SUCCESS = 0,
    BSON_FILE_PATH,
    BSON_MEMORY,
    BSON_NULL_POINTER,
    BSON_NOT_FOUND,
    BSON_INVALID,
    BSON_SYNTAX,

    BSON_NUM,
    BSON_LNG,
    BSON_DBL,
    BSON_STR,
    BSON_ARR,
    BSON_OBJ,
    BSON_BOOL,

	BSON_UNIMPLEMENTED,
    BSON_MAX
} bsonenum;

const char *bson_res(bsonenum e);

#endif
