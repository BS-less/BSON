#ifndef BSON_ENUM_H
#define BSON_ENUM_H

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
	BSON_TYPE_FIRST = -8,
    BSON_TYPE_LONG,
    BSON_TYPE_DOUBLE,
    BSON_TYPE_STRING,
    BSON_TYPE_ARRAY,
    BSON_TYPE_OBJECT,
    BSON_TYPE_BOOLEAN,
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

const char *bson_result(BsonResult r);
const char *bson_type(BsonType t);

#endif
