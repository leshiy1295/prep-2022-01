#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "tests.h"
#include "utils.h"


#define ARRAY_SIZE(_a) (sizeof(_a) / sizeof((_a)[0]))
#define TESTS_DEFAULT_DIR "./project/tests/data"
#define WITH_EXTRA_ARG "--with-extra"


static test_func tests[] = {
    // Init/release/basic operations
    test_matrix_creating,
    test_element_setting,

    // Math operations
    test_matrix_arithmetic,
    test_scalar_mul,
    test_matrix_transponse,
};

static test_func extra_tests[] = {
    // Extra operations
    test_matrix_determinant,
    test_matrix_adjugate,
    test_matrix_inverse,
};

/**
 * Usage only:
 *  $ ./tests.out [./path/to/tests] [--with-extra]
 */
int main(int argc, const char** argv) {
    int with_extra = 0;
    const char *base_dir = NULL;

    if (argc == 3) {
        base_dir = argv[1];
        with_extra = !strcmp(argv[2], WITH_EXTRA_ARG);
    } else if (argc == 2) {
        with_extra = !strcmp(argv[1], WITH_EXTRA_ARG);
        if (!with_extra) {
            base_dir = argv[1];
        } else {
            base_dir = TESTS_DEFAULT_DIR;
        }
    } else {
        base_dir = TESTS_DEFAULT_DIR;
    }

    for (size_t i = 0; i < ARRAY_SIZE(tests); i++) {
        tests[i](base_dir);
    }

    if (with_extra) {
        puts("Extra:");
        for (size_t i = 0; i < ARRAY_SIZE(extra_tests); i++) {
            extra_tests[i](base_dir);
        }
    }

    puts("SUCCESS\n");

    return 0;
}
