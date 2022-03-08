#include "utils.h"

fs::path glob_test_dir;

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << "[gtest params] <path_to_test_dir>" << std::endl;
    return -1;
  }

  glob_test_dir = argv[1];
  return RUN_ALL_TESTS();
}
