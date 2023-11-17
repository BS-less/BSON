#ifndef _BSON_ENUM_H_
#define _BSON_ENUM_H_

typedef enum {
    BSON_FAILURE = 0,
    BSON_FALSE   = 0,
    BSON_SUCCESS = 1,
    BSON_TRUE    = 1,
    BSON_FILE_PATH,
    BSON_MEMORY,
    BSON_NULL_POINTER,
    BSON_NOT_FOUND,
    BSON_INVALID,
    BSON_SYNTAX
} BsonResult;

typedef enum {
    BSON_TYPE_LNG = 0,
    BSON_TYPE_DBL,
    BSON_TYPE_STR,
    BSON_TYPE_ARR,
    BSON_TYPE_OBJ,
    BSON_TYPE_BOOL,
	BSON_TYPE_UNKNOWN,
    BSON_TYPE_MAX
} BsonType;

typedef enum {
    BSON_LOG_NONE = 0,
    BSON_LOG_NORMAL,
    BSON_LOG_VERBOSE,
    BSON_LOG_DEBUG,
    BSON_LOG_MAX
} BsonLogLevel;

const char *bson_res(BsonResult r);
const char *bson_type(BsonType t);

#endif
