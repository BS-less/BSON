#include "enums.h"

const char *bson_result(BsonResult r) {
    switch(r) {
		case BSON_FAILURE:       return "Result[FAILURE/FALSE]";  break;
		case BSON_SUCCESS:       return "Result[SUCCESS/TRUE]";   break;
		case BSON_FILE_PATH:     return "Result[FILE NOT FOUND]"; break;
		case BSON_MEMORY:        return "Result[MEMORY ERROR]";   break;
		case BSON_NULL_POINTER:  return "Result[NULL POINTER]";   break;
		case BSON_NOT_FOUND:	 return "Result[NOT FOUND]";      break;	
		case BSON_INVALID:		 return "Result[INVALID]";        break;
		case BSON_SYNTAX:        return "Result[SYNTAX ERROR]";   break;
    }
    return "Result[UNKNOWN]";
}

const char *bson_type(BsonType t) {
	switch(t) {	
		case BSON_TYPE_FIRST:       return "Type[FIRST]";       break;
		case BSON_TYPE_LONG:        return "Type[LONG]";        break;
		case BSON_TYPE_DOUBLE:      return "Type[DOUBLE]";		break;
		case BSON_TYPE_STRING:      return "Type[STRING]";		break;
		case BSON_TYPE_ARRAY:       return "Type[ARRAY]";		break;
		case BSON_TYPE_OBJECT:      return "Type[OBJECT]";		break;
		case BSON_TYPE_BOOLEAN:     return "Type[BOOLEAN]";     break;
        default:
        case BSON_TYPE_UNKNOWN:     return "Type[UNKNOWN]";     break;
        case BSON_TYPE_MAX:         return "Type[MAX]";         break;
    }
}

