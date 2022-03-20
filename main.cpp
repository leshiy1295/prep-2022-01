#include <iostream>


#include "utils.h"
#include "four.h"
#include "five.h"

#define ERR_ARGS_COUNT (-1)
#define ERR_WRONG_FLG (-2)

#define TST_FOO_FIX     1
#define TST_FOO_IMPL    2
#define TST_MOD_IMPL    3
#define TST_REC_IMPL    4

/* NOTE(stitaevskiy):
 * We use `atoi` function just for simplification and code reducing.
 * This function doesn't report conversation errors.
 * For safety program we recommend using `strtol` and its analogs.
 * (See `man atoi` and `man strtol` for more info).
 *
 * const char str_num[] = "1234";
 * char* end = NULL;
 * int val = (int) strtol(str_num, &end, 0);
 * if (end != '\0') {
 *     //ERROR
 * }
 *
 * */

int main(int argc, const char** argv) {
    if (argc < 3) {
        return ERR_ARGS_COUNT;
    }

    int Test_case = atoi(argv[1]);
    const char* data;
    data = argv[2];

    switch (Test_case) {
        case TST_FOO_FIX: {
            int to = atoi(data);
            size_t ticks_count = timer_from(to);
            printf("%ld\n", ticks_count);
            break;
        }
        case TST_FOO_IMPL: {
            if (argc = 4) {
                 int base = atoi(data);
                 int pow =  atoi(argv[3]);
                 int res = custom_pow(base, pow);
                 printf("%i\n", res);
            } else {
                return ERR_ARGS_COUNT;
            }
        }
        case TST_MOD_IMPL: {
            int num = atoi(data);
            int res = prime_num(num);
           printf("%i\n", res);
            // TODO: Print to stdout `1` if `num` is prime number and `0` otherwise
            // This function MUST be implemented in
            // a separate C-module (not in `main` or `utils` module)
        }
        case TST_REC_IMPL: {
          int n = atoi(data);
          rec(n);
        }
        default: {
            return ERR_WRONG_FLG;
        }
    }
}
