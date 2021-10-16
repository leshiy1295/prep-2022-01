#include <assert.h>
#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "tests.h"
#include "utils.h"


#define TEST_DIR "test_matrix_adjugate"
#define IN_FILE "in.txt"
#define OUT_FILE "out.txt"


static void _test_adjugate(const char *, const char *);


void test_matrix_adjugate(const char *base_dir) {
    printf("Test of getting agjugate matrix");

    char *test_dir = path_join(base_dir, TEST_DIR, (const char *) NULL);

    DIR* dir = opendir(test_dir);
    struct dirent* dp = NULL;
    while ((dp = readdir(dir))) {
        if (!(strcmp(dp->d_name, ".") && strcmp(dp->d_name, ".."))) {
            continue;
        }
        char *in = path_join(test_dir, dp->d_name, IN_FILE, (const char *) NULL);
        char *out = path_join(test_dir, dp->d_name, OUT_FILE, (const char *) NULL);

        _test_adjugate(in, out);
        printf(".");

        free(in);
        free(out);
    }

    closedir(dir);
    free(test_dir);

    puts("OK");
}

static void _test_adjugate(const char *in, const char *out) {
    Matrix *in_matrix = create_matrix_from_file(in);
    Matrix *expected_matrix = create_matrix_from_file(out);

    Matrix *adj_matrix = adj(in_matrix);
   
    assert_matrix_equal(expected_matrix, adj_matrix, 1e-2);

    free_matrix(in_matrix);
    free_matrix(expected_matrix);
    free_matrix(adj_matrix);
}

