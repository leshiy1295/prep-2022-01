/*

  {
      "key1": {
          "key2": [1, false, "str"],
          "key3": true
      },
      "key4": "hello"
  }

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cjson.h"

bool callback(const char *begin, const char *end, cjson_event_t event) {
    switch (event) {
    case CJSON_EVENT_OBEGIN: { puts("{"); break; }
    case CJSON_EVENT_OEND: { puts("}"); break; }
    case CJSON_EVENT_ABEGIN: { puts("["); break; }
    case CJSON_EVENT_AEND: { puts("]"); break; }
    case CJSON_EVENT_STR: {
        char *copy = strndup(begin, end - begin);
        puts(copy);
        free(copy);
        break;
    }
    case CJSON_EVENT_BOOL: {
        if (strncmp(begin, "true", sizeof("true") - 1) == 0) {
            puts("true");
        } else {
            puts("false");
        }
        break;
    }
    case CJSON_EVENT_NULL: { puts("null"); break; }
    case CJSON_EVENT_NUM: {
        double value = strtod(begin, NULL);
        printf("%lf\n", value);
        break;
    }
    }
    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <json>\n", argv[0]);
        return -1;
    }

    char *json_string = argv[1];
    if (cjson_parse(json_string, callback)) {
        printf("Json is correct\n");
    } else {
        printf("Json is incorrect\n");
    }

    return 0;
}

