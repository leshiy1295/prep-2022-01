#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "matrix.h"
#include "tests.h"
#include "utils.h"


#define ARRAY_SIZE(_a) (sizeof(_a) / sizeof((_a)[0]))

#define TEST_DIR "test_scalar_mul"
#define CASE_DIR_PREFIX "case_"
#define IN_FILE "in.txt"
#define OUT_FILE "out.txt"


static void _test_scalar_mul(const char *, double , const char *);


// TODO(a.telishev): Move to file factor.txt
static double factors[] = {
    0., 1., -232.23, 3.,
};

void test_scalar_mul(const char *base_dir) {
    printf("Testing of matrix scalar multiplication");
    fflush(stdout);

    char case_dir[10] = {0};

    for (size_t i = 0; i < ARRAY_SIZE(factors); i++) {
        snprintf(case_dir, sizeof(case_dir), "%s%zu", CASE_DIR_PREFIX, i);

        char *in = path_join(base_dir, TEST_DIR, case_dir, IN_FILE, (const char *) NULL);
        char *out = path_join(base_dir, TEST_DIR, case_dir, OUT_FILE, (const char *) NULL);

        _test_scalar_mul(in, factors[i], out);
        printf(".");

        free(in);
        free(out);
    }

    puts("OK");
}

void _test_scalar_mul(const char *in, double factor, const char *out) {
    Matrix *in_matrix = create_matrix_from_file(in);
    Matrix *expected_matrix = create_matrix_from_file(out);

    Matrix *result_matrix = mul_scalar(in_matrix, factor);
    assert_matrix_equal(expected_matrix, result_matrix, 0.3);

    free_matrix(in_matrix);
    free_matrix(expected_matrix);
    free_matrix(result_matrix);
}

