#include "utils.h"


TEST(MatrixMathBaseNegSuite, Sum) {
  const auto m1 = genM();
  const auto m2 = genM(m1.getRows(), m1.getCols() + 1);
  ASSERT_THROW(m1 + m2, prep::DimensionMismatch)
                << m1 << m2;
}

TEST(MatrixMathBaseNegSuite, Sub) {
  const auto m1 = genM();
  const auto m2 = genM(m1.getRows(), m1.getCols() + 1);
  ASSERT_THROW(m1 - m2, prep::DimensionMismatch)
                << m1 << m2;
}

TEST(MatrixMathBaseNegSuite, MatrixMul) {
  const auto m1 = genM();
  const auto m2 = genM(m1.getCols() + 1);
  ASSERT_THROW(m1 * m2, prep::DimensionMismatch)
                << m1 << m2;
}