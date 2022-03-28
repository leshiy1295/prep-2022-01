#include "cjson.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/*
Этапы разбора выражений:
- лексический анализ
- синтаксический анализ
- семантический анализ

-5.6 * 2.5e-3 + 3
-1 * 2 + 5

NUM :- [-5.6; 2.5e-3; 3]
NUM * NUM + NUM


Лексемы:
{ } [ ] " : , STR NUM BOOL NULL

{
    "key1": {
        "key2": [1, false, "str"],
        "key3": true
    },
    "key4": "hello"
}

{
    STR: {
        STR: [NUM, BOOL, STR],
        STR: BOOL
    },
    STR: STR
}


}{


{STR}

Состояния конечного автомата для JSON:
- BEGIN
- OBEGIN
- OKEY
- OCOLON
- OVALUE
- OCOMMA
- ABEGIN
- AVALUE
- ACOMMA
- END
- ERROR
*/

// Лексемы: { } [ ] : , STR NUM BOOL NULL
typedef enum {
    L_OBEGIN,
    L_OEND,
    L_ABEGIN,
    L_AEND,
    L_COLON,
    L_COMMA,
    L_STR,
    L_NUM,
    L_BOOL,
    L_NULL,
    L_COUNT,
    L_ERR
} lexem_t;

typedef enum {
    S_BEGIN,
    S_OBEGIN,
    S_OKEY,
    S_OCOLON,
    S_OVALUE,
    S_OCOMMA,
    S_ABEGIN,
    S_AVALUE,
    S_ACOMMA,
    S_END,
    S_COUNT,
    S_ERR
} state_t;

typedef bool (*action_t)(const char *s, const char **end, cjson_callback_t callback);

typedef struct {
    state_t state;
    action_t action;
} rule_t;

// Таблица переходов - матрица размерности
// "число состояний" x "число лексем"

static lexem_t get_lexem(const char *s, const char **end);
static lexem_t get_string(const char *s, const char **end);
static lexem_t get_value(const char *s, const char **end);

static bool extract_json(const char *s, const char **end, cjson_callback_t callback);

static bool on_obegin(const char *s, const char **end, cjson_callback_t callback) { return callback(s, *end, CJSON_EVENT_OBEGIN); }
static bool on_oend(const char *s, const char **end, cjson_callback_t callback) { return callback(s, *end, CJSON_EVENT_OEND); }
static bool on_abegin(const char *s, const char **end, cjson_callback_t callback) { return callback(s, *end, CJSON_EVENT_ABEGIN); }
static bool on_aend(const char *s, const char **end, cjson_callback_t callback) { return callback(s, *end, CJSON_EVENT_AEND); }
static bool on_str(const char *s, const char **end, cjson_callback_t callback) { return callback(s + 1, *end - 1, CJSON_EVENT_STR); }
static bool on_bool(const char *s, const char **end, cjson_callback_t callback) { return callback(s, *end, CJSON_EVENT_BOOL); }
static bool on_null(const char *s, const char **end, cjson_callback_t callback) { return callback(s, *end, CJSON_EVENT_NULL); }
static bool on_num(const char *s, const char **end, cjson_callback_t callback) { return callback(s, *end, CJSON_EVENT_NUM); }

static rule_t transitions[S_COUNT][L_COUNT] = {
//             L_OBEGIN                         L_OEND               L_ABEGIN                        L_AEND               L_COLON              L_COMMA               L_STR                            L_NUM                             L_BOOL                            L_NULL
/* S_BEGIN */  {{ S_OBEGIN, on_obegin},         { S_ERR, NULL },     { S_ABEGIN, on_abegin },        { S_ERR, NULL },     { S_ERR, NULL },     { S_ERR, NULL },      { S_END, on_str },               { S_END, on_num },                { S_END, on_bool },               { S_END, on_null }},
/* S_OBEGIN */ {{ S_ERR, NULL },                { S_END, on_oend },  { S_ERR, NULL },                { S_ERR, NULL },     { S_ERR, NULL },     { S_ERR, NULL },      { S_OKEY, on_str },              { S_ERR, NULL },                  { S_ERR, NULL },                  { S_ERR, NULL }},
/* S_OKEY */   {{ S_ERR, NULL },                { S_ERR, NULL },     { S_ERR, NULL },                { S_ERR, NULL },     { S_OCOLON, NULL },  { S_ERR, NULL },      { S_ERR, NULL },                 { S_ERR, NULL },                  { S_ERR, NULL },                  { S_ERR, NULL }},
/* S_OCOLON */ {{ S_OVALUE, extract_json },     { S_ERR, NULL },     { S_OVALUE, extract_json },     { S_ERR, NULL },     { S_ERR, NULL },     { S_ERR, NULL },      { S_OVALUE, extract_json },      { S_OVALUE, extract_json },       { S_OVALUE, extract_json },       { S_OVALUE, extract_json }},
/* S_OVALUE */ {{ S_ERR, NULL },                { S_END, on_oend },  { S_ERR, NULL },                { S_ERR, NULL },     { S_ERR, NULL },     { S_OCOMMA, NULL },   { S_ERR, NULL },                 { S_ERR, NULL },                  { S_ERR, NULL },                  { S_ERR, NULL }},
/* S_OCOMMA */ {{ S_ERR, NULL },                { S_ERR, NULL },     { S_ERR, NULL },                { S_ERR, NULL },     { S_ERR, NULL },     { S_ERR, NULL },      { S_OKEY, on_str },              { S_ERR, NULL },                  { S_ERR, NULL },                  { S_ERR, NULL }},
/* S_ABEGIN */ {{ S_AVALUE, extract_json },     { S_ERR, NULL },     { S_AVALUE, extract_json },     { S_END, on_aend },  { S_ERR, NULL },     { S_ERR, NULL },      { S_AVALUE, extract_json },      { S_AVALUE, extract_json },       { S_AVALUE, extract_json },       { S_AVALUE, extract_json }},
/* S_AVALUE */ {{ S_ERR, NULL },                { S_ERR, NULL },     { S_ERR, NULL },                { S_END, on_aend },  { S_ERR, NULL },     { S_ACOMMA, NULL },   { S_ERR, NULL },                 { S_ERR, NULL },                  { S_ERR, NULL },                  { S_ERR, NULL }},
/* S_ACOMMA */ {{ S_AVALUE, extract_json },     { S_ERR, NULL },     { S_AVALUE, extract_json },     { S_ERR, NULL },     { S_ERR, NULL },     { S_ERR, NULL },      { S_AVALUE, extract_json },      { S_AVALUE, extract_json },       { S_AVALUE, extract_json },       { S_AVALUE, extract_json }},
/* S_END */    {{ S_ERR, NULL },                { S_ERR, NULL },     { S_ERR, NULL },                { S_ERR, NULL },     { S_ERR, NULL },     { S_ERR, NULL },      { S_ERR, NULL },                 { S_ERR, NULL },                  { S_ERR, NULL },                  { S_ERR, NULL }}
};

static bool is_hex(char sym) {
    return (sym >= '0' && sym <= '9') || (sym >= 'A' && sym <= 'F') || (sym >= 'a' && sym <= 'f');
}

static lexem_t get_string(const char *s, const char **end) {
    ++s;
    while (*s != '\0' && *s != '"') {
        if (*s == '\\') {
            char next = *(s + 1);
            if (next == '"' || next == '\\' || next == '/' || next == 'b' || next == 'f' || next == 'n' || next == 'r' || next == 't') {
                ++s;
            } else if (next == 'u' && is_hex(*(s + 2)) && is_hex(*(s + 3)) && is_hex(*(s + 4)) && is_hex(*(s + 5))) {
                 s += 4;
            } else {
                 return L_ERR;
            }
        }
        ++s;
    }
    if (*s == '\0') {
        return L_ERR;
    }
    *end = s + 1;
    return L_STR;
}

static lexem_t get_value(const char *s, const char **end) {
    if (strncmp(s, "true", sizeof("true") - 1) == 0) {
        *end = s + sizeof("true") - 1;
        return L_BOOL;
    }

    if (strncmp(s, "false", sizeof("false") - 1) == 0) {
        *end = s + sizeof("false") - 1;
        return L_BOOL;
    }

    if (strncmp(s, "null", sizeof("null") - 1) == 0) {
        *end = s + sizeof("null") - 1;
        return L_NULL;
    }

    char *tmp;
    strtod(s, &tmp);
    if (tmp == s) {
        return L_ERR;
    }
    *end = tmp;
    return L_NUM;
}

// Лексемы: { } [ ] : , STR NUM BOOL NULL
static lexem_t get_lexem(const char *s, const char **end) {
    if (!s || !end) { return L_ERR; }

    *end = s + 1;
    switch (*s) {
        case '{': return L_OBEGIN;
        case '}': return L_OEND;
        case '[': return L_ABEGIN;
        case ']': return L_AEND;
        case ':': return L_COLON;
        case ',': return L_COMMA;
        case '"': return get_string(s, end);
        default: return get_value(s, end);
    }

    return L_ERR;
}

static bool extract_json(const char *s, const char **end, cjson_callback_t callback) {
    state_t state = S_BEGIN;
    while (*s) {
        while (isspace(*s)) {
            ++s;
        }
        lexem_t lexem = get_lexem(s, end);
        if (lexem == L_ERR) {
            return false;
        }
        rule_t rule = transitions[state][lexem];
        if (rule.state == S_ERR) {
            return false;
        }
        if (rule.action != NULL) {
            if (!rule.action(s, end, callback)) {
                return false;
            }
        }
        state = rule.state;
        if (rule.state == S_END) {
            break;
        }
        s = *end;
    }
    return state == S_END;
}

/*
{
    "key1": {
        "key2": [1, false, "str"],
        "key3": true
    },
    "key4": "hello"
}
*/

bool cjson_parse(const char *s, cjson_callback_t callback) {
    if (!s) {
        return false;
    }
    const char *end;
    if (!extract_json(s, &end, callback)) {
        return false;
    }
    while (isspace(*end)) {
        ++end;
    }
    return *end == '\0';
}
