#ifndef PROJECT_TESTS_INCLUDE_TESTS_H_
#define PROJECT_TESTS_INCLUDE_TESTS_H_


typedef void (* test_func)(const char *);


// Init/release/basic operations
void test_matrix_creating(const char *);
void test_element_setting(const char *);

// Math operations
void test_scalar_mul(const char *);
void test_matrix_transponse(const char *);
void test_matrix_arithmetic(const char *);

// Extra operations
void test_matrix_determinant(const char *);
void test_matrix_adjugate(const char *);
void test_matrix_inverse(const char *);


#endif  // PROJECT_TESTS_INCLUDE_TESTS_H_
