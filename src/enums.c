#include "enums.h"

const char *bson_res(bsonenum e) {
    switch(e) {
		case BSON_SUCCESS:       return "Result[SUCCESS]";        break;
		case BSON_FILE_PATH:     return "Result[FILE NOT FOUND]"; break;
		case BSON_MEMORY:        return "Result[MEMORY ERROR]";   break;
		case BSON_NULL_POINTER:  return "Result[NULL POINTER]";   break;
		case BSON_NOT_FOUND:	 return "Result[NOT FOUND]";      break;	
		case BSON_INVALID:		 return "Result[INVALID]";        break;
		case BSON_SYNTAX:        return "Result[SYNTAX ERROR]";   break;
        

		case BSON_NUM:           return "Value[NUMBER]";          break;
		case BSON_LNG:           return "Value[LONG]";            break;
		case BSON_DBL:			 return "Value[DOUBLE]";		  break;
		case BSON_STR:			 return "Value[STRING]";		  break;
		case BSON_ARR:			 return "Value[ARRAY]";			  break;
		case BSON_OBJ:			 return "Value[OBJECT]";		  break;
        case BSON_BOOL:          return "Value[BOOLEAN]";         break;
        
		case BSON_UNIMPLEMENTED: return "Result[UNIMPLEMENTED]";  break;
		case BSON_MAX:			 return "Value[ENUM MAX/LAST]";   break;
    }
    return "Enum[INVALID]";
}
