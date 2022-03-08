#pragma once

#include "matrix.h"
#include "exceptions.h"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>


namespace fs = std::filesystem;

extern fs::path glob_test_dir;

using MatrixData = std::vector<std::vector<double>>;
using MatrixRow = std::vector<double>;

MatrixData createMatrixData(std::istream& is);
std::ostream& operator<<(std::ostream& os, const MatrixData& matrix_data);

const size_t MAX_SIZE = 10;
const double MIN_VAL = -100.0;
const double MAX_VAL = 100.0;

double genV(double from = MIN_VAL, double to = MAX_VAL);
size_t genI(size_t from = 0, size_t to = MAX_SIZE);
prep::Matrix genM(size_t rows = 0, size_t cols = 0);
prep::Matrix genNonsquare();

namespace prep {
  bool operator==(const prep::Matrix& matrix, const MatrixData& matrix_data);
  bool operator==(const MatrixData& matrix_data, const prep::Matrix& matrix);
}