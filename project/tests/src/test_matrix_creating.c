#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "matrix.h"
#include "tests.h"
#include "utils.h"


#define ARRAY_SIZE(_a) (sizeof(_a) / sizeof((_a)[0]))

#define TEST_DIR "test_matrix_creating"
#define CASE_DIR_PREFIX "case_"
#define IN_FILE "in.txt"

#define N 100


typedef struct TestCase {
    size_t expected_rows_count;
    size_t expected_cols_count;
    double expected_matrix[N][N];
} TestCase;

static void _test_create_from_file(const char *, TestCase);


static TestCase test_cases[] = {
    {
        .expected_rows_count = 1,
        .expected_cols_count = 1,
        .expected_matrix = {
            {-10.1},
        }
    },
    {
        .expected_rows_count = 3,
        .expected_cols_count = 6,
        .expected_matrix = {
            {485.7029, -911.5387, -799.193, 85.8300, -572.56, 841.7244},
            {-273.4, -251.8131, 552.171, 175.81138, -810.2, 309.235},
            {65.79005, -656.0, -830.5480, 907.957, 400.522, -241.1129},
        }
    },
    {
        .expected_rows_count = 9,
        .expected_cols_count = 4,
        .expected_matrix = {
            {-46.13465, -282.8, 692.1370, -993.425},
            {-484.30, 632.1, 773.37, -638.59303},
            {-303.4, -917.8957, 342.03900, 223.42296},
            {181.179, -129.71231, 66.481, 789.0572},
            {405.27, -426.1, 656.093, -850.68058},
            {736.47, -531.27, 363.23012, -983.11},
            {363.79074, -81.6517, -927.0703, -465.4201},
            {-411.382, -978.3965, -628.6961, -671.658},
            {211.9, -313.71465, 917.456, 624.910},
        }
    },
    {
        .expected_rows_count = 7,
        .expected_cols_count = 7,
        .expected_matrix = {
            {578.1822, -375.2, 881.4873, 677.599, 100.7, 366.0101, 705.41548},
            {605.846, 561.56103, 880.4, -224.18451, -681.620, 128.75176, -443.93},
            {55.2, 484.1759, -971.91, 448.6518, 18.75644, -529.4, -961.65},
            {-842.1007, 419.6, -357.84765, -781.75242, -285.77, -887.1176, -212.9},
            {202.35896, -124.693, -303.5088, -604.835, -381.6, 233.23757, -635.066},
            {372.01, 420.5092, -985.694, 762.42897, 909.92, -83.412, -715.40},
            {-44.32186, -280.73075, -946.30, 602.1756, -761.3094, -115.22, -439.6},
        }
    },
};

// Dirty, but you must strictly relate the `test_cases` and dirs
void test_matrix_creating(const char *base_dir) {
    printf("Testing of creating matrix from file");
    fflush(stdout);

    char case_dir[10] = {0};

    for (size_t i = 0; i < ARRAY_SIZE(test_cases); i++) {
        snprintf(case_dir, sizeof(case_dir), "%s%zu", CASE_DIR_PREFIX, i);
        char *in = path_join(base_dir, TEST_DIR, case_dir, IN_FILE, (const char *) NULL);

        _test_create_from_file(in, test_cases[i]);
        printf(".");

        free(in);
    }

    puts("OK");
}

/*
 * - Create matrix from file.
 * - Check rows count, check cols count.
 * - Check every element of matrix.
 * - Free matrix.
 */
static void _test_create_from_file(const char *in, TestCase test_case) {
    Matrix *matrix = create_matrix_from_file(in);
    assert((matrix != NULL) && "Creating from file was failed");

    size_t rows = 0;
    int res = get_rows(matrix, &rows);
    assert((res == 0) && "Getting rows count was failed");
    if (rows != test_case.expected_rows_count) {
        printf("\nExpected rows: %zu\n", test_case.expected_rows_count);
        printf("Received rows: %zu\n", rows);
        assert(0 && "Invalid rows count");
    }

    size_t cols = 0;
    res = get_cols(matrix, &cols);
    assert((res == 0) && "Getting cols count was failed");
    if (cols != test_case.expected_cols_count) {
        printf("\nExpected cols: %zu\n", test_case.expected_cols_count);
        printf("Received cols: %zu\n", cols);
        assert(0 && "Invalid cols count");
    }

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            double elem = 0.;
            get_elem(matrix, i, j, &elem);

            if (!double_equals(elem, test_case.expected_matrix[i][j], 0.1)) {
                printf("\nExpected element: %.4lf\n", elem);
                printf("Received element: %.4lf\n", test_case.expected_matrix[i][j]);
                assert(0 && "The value of the element does not match the expected");
            }
        }
    }

    free_matrix(matrix);
}
