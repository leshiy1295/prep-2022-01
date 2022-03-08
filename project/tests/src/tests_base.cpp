#include "utils.h"


TEST(MatrixBaseSuite, CreationFromFile) {
  for (auto& entry : fs::directory_iterator(glob_test_dir / "matrix_creation")) {
    std::cout << entry.path() << std::endl;

    std::ifstream matrix_stream(entry.path());
    ASSERT_NO_THROW(prep::Matrix{matrix_stream});

    matrix_stream.seekg(0);
    const prep::Matrix matrix(matrix_stream);
    matrix_stream.seekg(0);
    const MatrixData expected_matrix = createMatrixData(matrix_stream);
    ASSERT_EQ(expected_matrix, matrix);
  }
}

TEST(MatrixBaseSuite, CreationFromScratch) {
  ASSERT_NO_THROW(prep::Matrix());
  ASSERT_NO_THROW(prep::Matrix(genI(1), genI(1)));

  size_t rows = genI(1);
  size_t cols = genI(1);
  const prep::Matrix matrix(rows, cols);
  ASSERT_EQ(rows, matrix.getRows());
  ASSERT_EQ(cols, matrix.getCols());
}

TEST(MatrixBaseSuite, ElementAccess) {
  prep::Matrix matrix(genI(1), genI(1));

  auto i = genI(0, matrix.getRows() - 1);
  auto j = genI(0, matrix.getCols() - 1);
  auto val = genV();

  ASSERT_NO_THROW(matrix(i, j))
                << "i=" << i << ", j=" << j << std::endl
                << "val=" << val << std::endl << matrix;
  ASSERT_NO_THROW(matrix(i, j) = val)
                << "i=" << i << ", j=" << j << std::endl
                << "val=" << val << std::endl << matrix;
  ASSERT_DOUBLE_EQ(val, matrix(i, j))
                << "i=" << i << ", j=" << j << std::endl
                << "val=" << val << std::endl << matrix;
}

TEST(MatrixBaseSuite, CopyAssign) {
  const auto m1 = genM();
  ASSERT_NO_THROW(prep::Matrix{m1});

  const prep::Matrix m2{m1};
  ASSERT_EQ(m1, m2);

  prep::Matrix m3;
  ASSERT_NO_THROW(m3 = m1);
  ASSERT_EQ(m1, m3);
}

TEST(MatrixBaseSuite, Store) {
  const prep::Matrix m1 = genM();

  auto file_name = "store_matrix";
  std::ofstream os(file_name);
  ASSERT_NO_THROW(os << m1);
  os.close();

  std::ifstream is(file_name);
  const prep::Matrix m2(is);
  is.close();

  ASSERT_EQ(m1, m2);

  fs::remove(file_name);
}

TEST(MatrixBaseSuite, CompareNeg) {
  const prep::Matrix m1 = genM();
  const prep::Matrix m2 = genM(m1.getRows() + 1);
  ASSERT_NE(m1, m2);

  prep::Matrix m3{m1};
  m3(genI(0, m3.getRows() - 1), genI(0, m3.getCols() - 1)) = genV();
  ASSERT_NE(m1, m3);
}
