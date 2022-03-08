#include "utils.h"


TEST(MatrixMathExtraNegSuite, Det) {
  for (auto& entry : fs::directory_iterator(glob_test_dir / "matrix_extra_neg")) {
    std::cout << entry.path() << std::endl;

    std::ifstream is(entry.path());
    const prep::Matrix m{is};
    is.close();

    ASSERT_NEAR(0.0, m.det(), 1e-07);
  }
}

TEST(MatrixMathExtraNegSuite, Inv) {
  for (auto& entry : fs::directory_iterator(glob_test_dir / "matrix_extra_neg")) {
    std::cout << entry.path() << std::endl;

    std::ifstream is(entry.path());
    const prep::Matrix m{is};
    is.close();

    ASSERT_THROW(m.inv(), prep::SingularMatrix);
  }
}

TEST(MatrixMathExtraNegSuite, DetDimension) {
  auto m = genNonsquare();
  ASSERT_THROW(m.det(), prep::DimensionMismatch);
}

TEST(MatrixMathExtraNegSuite, AdjDimension) {
  auto m = genNonsquare();
  ASSERT_THROW(m.adj(), prep::DimensionMismatch);
}

TEST(MatrixMathExtraNegSuite, InvDimension) {
  auto m = genNonsquare();
  ASSERT_THROW(m.inv(), prep::DimensionMismatch);
}
