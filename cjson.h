#ifndef CJSON_H
#define CJSON_H

#include <stdbool.h>

typedef enum {
    CJSON_EVENT_OBEGIN,
    CJSON_EVENT_OEND,
    CJSON_EVENT_ABEGIN,
    CJSON_EVENT_AEND,
    CJSON_EVENT_STR,
    CJSON_EVENT_BOOL,
    CJSON_EVENT_NULL,
    CJSON_EVENT_NUM
} cjson_event_t;

typedef bool (*cjson_callback_t)(const char *begin, const char *end, cjson_event_t event);

bool cjson_parse(const char *json, cjson_callback_t callback);

#endif
