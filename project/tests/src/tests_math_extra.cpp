#include "utils.h"


TEST(MatrixMathExtraSuite, Det) {
  for (auto& entry : fs::directory_iterator(glob_test_dir / "matrix_det")) {
    std::cout << entry.path() << std::endl;

    std::ifstream is(entry.path() / "in.txt");
    const prep::Matrix m{is};
    is.close();

    is.open(entry.path() / "out.txt");
    double expected_det = 0.0;
    is >> expected_det;
    is.close();

    double m_det = 0.0;
    ASSERT_NO_THROW(m_det = m.det());
    ASSERT_NEAR(expected_det, m_det, 1e-07);
  }
}

TEST(MatrixMathExtraSuite, Adj) {
  for (auto& entry : fs::directory_iterator(glob_test_dir / "matrix_adj")) {
    std::cout << entry.path() << std::endl;

    std::ifstream is(entry.path() / "in.txt");
    const prep::Matrix m{is};
    is.close();

    is.open(entry.path() / "out.txt");
    const prep::Matrix expected_adj{is};
    is.close();

    prep::Matrix m_adj{};
    ASSERT_NO_THROW(m_adj = m.adj());
    ASSERT_EQ(expected_adj, m_adj);
  }
}

TEST(MatrixMathExtraSuite, Inv) {
  for (auto& entry : fs::directory_iterator(glob_test_dir / "matrix_inv")) {
    std::cout << entry.path() << std::endl;

    std::ifstream is(entry.path() / "in.txt");
    const prep::Matrix m{is};
    is.close();

    is.open(entry.path() / "out.txt");
    const prep::Matrix expected_inv{is};
    is.close();

    prep::Matrix m_inv{};
    ASSERT_NO_THROW(m_inv = m.inv());
    ASSERT_EQ(expected_inv, m_inv);
  }
}
