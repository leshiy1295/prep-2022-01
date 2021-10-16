#include <assert.h>
#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "tests.h"
#include "utils.h"


#define TEST_DIR "test_matrix_inverse"
#define IN_FILE "in.txt"


static void _test_inverse(const char *);


void test_matrix_inverse(const char *base_dir) {
    printf("Testing of matrix inversing");
    fflush(stdout);

    char *test_dir = path_join(base_dir, TEST_DIR, (const char *) NULL);

    DIR* dir = opendir(test_dir);
    struct dirent* dp = NULL;
    while ((dp = readdir(dir))) {
        if (!(strcmp(dp->d_name, ".") && strcmp(dp->d_name, ".."))) {
            continue;
        }
        char *in = path_join(test_dir, dp->d_name, IN_FILE, (const char *) NULL);

        _test_inverse(in);
        printf(".");

        free(in);
    }

    closedir(dir);
    free(test_dir);

    puts("OK");
}

static void _test_inverse(const char *in) {
    Matrix *matrix = create_matrix_from_file(in);
    size_t size = 0;
    get_rows(matrix, &size);
    Matrix *expected_identity_matrix = create_identity_matrix(size);

    Matrix *inversed_matrix = inv(matrix);
    Matrix *identity_matrix = mul(matrix, inversed_matrix);

    assert_matrix_equal(expected_identity_matrix, identity_matrix, eps_base);

    free_matrix(matrix);
    free_matrix(expected_identity_matrix);
    free_matrix(inversed_matrix);
    free_matrix(identity_matrix);
}
