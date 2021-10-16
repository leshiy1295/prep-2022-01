#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"


/**
 * Usage:
 *      char *path = path_join("./data", "test", "in.txt", (const char *) NULL);   // == "./data/test/in.txt"
 *      free(path);
 */
char *path_join(const char *dir, ...) {
    const char *tmp_dir;

    va_list dirs, dirs_copy;

    va_start(dirs, dir);
    va_copy(dirs_copy, dirs);

    // Count result size
    size_t len = strlen(dir) + 1;
    size_t slash_counts = 0;
    while ((tmp_dir = va_arg(dirs_copy, const char *))) {
        len += strlen(tmp_dir) + 1;
        slash_counts += 1;
    }
    va_end(dirs_copy);

    // Concat dirs with "/"
    char *result = (char *) calloc(len + slash_counts, sizeof(char));
    strncpy(result, dir, strlen(dir) + 1);
    while ((tmp_dir = va_arg(dirs, const char *))) {
        // FIXME(a.telishev): snprintf
        result = strcat(strcat(result, "/"), tmp_dir);
    }

    va_end(dirs);

    return result;
}

void print_matrix(const Matrix *matrix) {
    size_t rows = 0;
    get_rows(matrix, &rows);

    size_t cols = 0;
    get_cols(matrix, &cols);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            double elem = 0.;
            get_elem(matrix, i, j, &elem);
            printf("%10.4f\t", elem);
        }
        puts("");
    }
    puts("");
}

Matrix *create_identity_matrix(int size) {
    Matrix *matrix = create_matrix(size, size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            set_elem(matrix, i, j, (i == j) ? 1. : 0.);
        }
    }

    return matrix;
}

int double_equals(double left, double right, double eps) {
    return fabs(left - right) < eps;
}

void assert_matrix_equal(Matrix *expected, Matrix *received, double eps) {
    size_t l_rows = 0;
    get_rows(expected, &l_rows);
    size_t r_rows = 0;
    get_rows(received, &r_rows);

    assert((l_rows == r_rows) && "Matrices have different number of rows");

    size_t l_cols = 0;
    get_cols(expected, &l_cols);
    size_t r_cols = 0;
    get_cols(received, &r_cols);

    for (size_t i = 0; i < l_rows; i++) {
        for (size_t j = 0; j < l_cols; j++) {
            double l_elem = 0.;
            get_elem(expected, i, j, &l_elem);
            double r_elem = 0.;
            get_elem(received, i, j, &r_elem);

            if (!double_equals(l_elem, r_elem, eps)) {
                printf("\nMatrix element does not match in position [%zu][%zu]\n", i, j);

                puts("Expected matrix:");
                print_matrix(expected);

                puts("Received matrix:");
                print_matrix(received);

                assert(0 && "Matrices are not equal");
            }
        }
    }
}
