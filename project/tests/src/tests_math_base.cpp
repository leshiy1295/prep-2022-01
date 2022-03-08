#include "utils.h"


TEST(MatrixMathBaseSuite, Sum) {
  const auto m1 = genM();
  const auto m2 = genM(m1.getRows(), m1.getCols());
  ASSERT_NO_THROW(m1 + m2)
                << m1 << m2;

  const auto res = m1 + m2;
  for (size_t i = 0; i < res.getRows(); ++i) {
    for (size_t j = 0; j < res.getCols(); ++j) {
      ASSERT_DOUBLE_EQ(m1(i, j) + m2(i, j), res(i, j))
                    << "i=" << i << ", j=" << j << std::endl
                    << m1 << m2 << res;
    }
  }
}

TEST(MatrixMathBaseSuite, Sub) {
  const auto m1 = genM();
  const auto m2 = genM(m1.getRows(), m1.getCols());
  ASSERT_NO_THROW(m1 - m2)
                << m1 << m2;

  const auto res = m1 - m2;
  for (size_t i = 0; i < res.getRows(); ++i) {
    for (size_t j = 0; j < res.getCols(); ++j) {
      ASSERT_DOUBLE_EQ(m1(i, j) - m2(i, j), res(i, j))
                    << "i=" << i << ", j=" << j << std::endl
                    << m1 << m2 << res;
    }
  }
}

TEST(MatrixMathBaseSuite, ScalarMul) {
  const auto m1 = genM();
  const auto val = genV();
  ASSERT_NO_THROW(m1 * val)
                << "val=" << val << std::endl << m1;

  const auto res_1 = m1 * val;
  for (size_t i = 0; i < res_1.getRows(); ++i) {
    for (size_t j = 0; j < res_1.getCols(); ++j) {
      ASSERT_DOUBLE_EQ(m1(i, j) * val, res_1(i, j))
                    << "i=" << i << ", j=" << j << std::endl
                    << "val=" << val << std::endl
                    << m1 << res_1;
    }
  }

  ASSERT_NO_THROW(val * m1)
                << "val=" << val << std::endl << m1;
  const auto res_2 = val * m1;
  ASSERT_EQ(res_1, res_2)
                << "val=" << val << std::endl << m1;
}

TEST(MatrixMathBaseSuite, Transp) {
  const auto m1 = genM();
  ASSERT_NO_THROW(m1.transp())
                << m1;

  const auto res = m1.transp();
  ASSERT_EQ(m1.getRows(), res.getCols());
  ASSERT_EQ(m1.getCols(), res.getRows());

  for (size_t i = 0; i < res.getRows(); ++i) {
    for (size_t j = 0; j < res.getCols(); ++j) {
      ASSERT_DOUBLE_EQ(m1(j, i), res(i, j))
                    << "i=" << i << ", j=" << j << std::endl
                    << m1 << res;
    }
  }
}

TEST(MatrixMathBaseSuite, MatrixMul) {
  const auto m1 = genM();
  const auto m2 = genM(m1.getCols());
  ASSERT_NO_THROW(m1 * m2)
                << m1 << m2;

  const auto res = m1 * m2;
  ASSERT_EQ(m1.getRows(), res.getRows());
  ASSERT_EQ(m2.getCols(), res.getCols());

  for (size_t i = 0; i < res.getRows(); ++i) {
    for (size_t j = 0; j < res.getCols(); ++j) {
      double sum = 0.0;
      for (size_t k = 0; k < m1.getCols(); ++k) {
        sum += m1(i, k) * m2(k, j);
      }
      ASSERT_DOUBLE_EQ(sum, res(i, j))
                    << "i=" << i << ", j=" << j << std::endl
                    << m1 << m2 << res;
    }
  }
}