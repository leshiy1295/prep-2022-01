#include <assert.h>
#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "tests.h"
#include "utils.h"


#define TEST_DIR "test_element_setting"
#define IN_FILE "in.txt"


static void _test_set_elem(const char *);


void test_element_setting(const char *base_dir) {
    printf("Testing of element setting");
    fflush(stdout);

    char *test_dir = path_join(base_dir, TEST_DIR, (const char *) NULL);

    DIR* dir = opendir(test_dir);
    struct dirent* dp = NULL;
    while ((dp = readdir(dir))) {
        if (!(strcmp(dp->d_name, ".") && strcmp(dp->d_name, ".."))) {
            continue;
        }
        char *in = path_join(test_dir, dp->d_name, IN_FILE, (const char *) NULL);

        _test_set_elem(in);
        printf(".");

        free(in);
    }

    closedir(dir);
    free(test_dir);

    puts("OK");
}

static void _test_set_elem(const char *in) {
    Matrix *src_matrix = create_matrix_from_file(in);

    size_t rows = 0;
    size_t cols = 0;
    get_rows(src_matrix, &rows);
    get_cols(src_matrix, &cols);

    Matrix *dst_matrix = create_matrix(rows, cols);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            double elem = 0.;
            get_elem(src_matrix, i, j, &elem);
            int res = set_elem(dst_matrix, i, j, elem);
            assert((res == 0) && "Setting element was failed");
        }
    }

    assert_matrix_equal(src_matrix, dst_matrix, eps_base);

    free_matrix(src_matrix);
    free_matrix(dst_matrix);
}
