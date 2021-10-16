#include <assert.h>
#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "tests.h"
#include "utils.h"


#define ARRAY_SIZE(_a) (sizeof(_a) / sizeof((_a)[0]))

#define LEFT_FILE "left.txt"
#define RIGHT_FILE "right.txt"
#define RESULT_FILE "rsult.txt"


typedef Matrix * (* matrix_arith_func)(const Matrix*, const Matrix*);

typedef struct TestCaseArith {
    const char* dir_name;
    matrix_arith_func fn;
} TestCaseArith;

static TestCaseArith tests[] = {
    {.dir_name = "test_matrices_sum", .fn = sum},
    {.dir_name = "test_matrices_sub", .fn = sub},
    {.dir_name = "test_matrices_mul", .fn = mul},
};

static void _test_arith(const char *, const char *, const char *, matrix_arith_func);


void test_matrix_arithmetic(const char *base_dir) {
    printf("Testing of matrix arithmetic: sum, sub, mul");
    fflush(stdout);

    DIR *dir = NULL;
    struct dirent *dp = NULL;
    char *test_dir = NULL;

    for (size_t i = 0; i < ARRAY_SIZE(tests); i++) {
        test_dir = path_join(base_dir, tests[i].dir_name, (const char *) NULL);
        dir = opendir(test_dir);

        while ((dp = readdir(dir))) {
            if (!(strcmp(dp->d_name, ".") && strcmp(dp->d_name, ".."))) {
                continue;
            }

            char *left = path_join(test_dir, dp->d_name, LEFT_FILE, (const char *) NULL);
            char *right = path_join(test_dir, dp->d_name, RIGHT_FILE, (const char *) NULL);
            char *result = path_join(test_dir, dp->d_name, RESULT_FILE, (const char *) NULL);

            _test_arith(left, right, result, tests[i].fn);
            printf(".");

            free(left);
            free(right);
            free(result);
        }

        closedir(dir);
        free(test_dir);
    }

    puts("OK");
}

static void _test_arith(const char *left, const char *right, const char *result,
                        matrix_arith_func test_func) {
    Matrix *left_matrix = create_matrix_from_file(left);
    Matrix *right_matrix = create_matrix_from_file(right);
    Matrix *expected_matrix = create_matrix_from_file(result);

    Matrix *result_matrix = test_func(left_matrix, right_matrix);
    assert_matrix_equal(expected_matrix, result_matrix, eps_base);

    free_matrix(left_matrix);
    free_matrix(right_matrix);
    free_matrix(expected_matrix);
    free_matrix(result_matrix);
}
