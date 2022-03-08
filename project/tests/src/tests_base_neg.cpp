#include "utils.h"


TEST(MatrixBaseNegSuite, CreationFromFile) {
  for (auto& entry : fs::directory_iterator(glob_test_dir / "matrix_creation_neg")) {
    std::cout << entry.path() << std::endl;

    std::ifstream matrix_stream(entry.path());
    ASSERT_THROW(prep::Matrix{matrix_stream}, prep::InvalidMatrixStream);
  }
}

TEST(MatrixBaseNegSuite, OutOfRangeAccess) {
  const auto m = genM();
  ASSERT_THROW(m(m.getRows(), m.getCols()), prep::OutOfRange);
}
