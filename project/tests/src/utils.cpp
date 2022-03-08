#include "utils.h"

#include <random>


MatrixData createMatrixData(std::istream& is) {
  size_t rows = 0;
  size_t cols = 0;

  is >> rows >> cols;
  if (!is) {
    throw std::runtime_error("MatrixData reading failed");
  }

  MatrixData matrix_data(rows, MatrixRow(cols, 0.0));
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      is >> matrix_data[i][j];
      if (!is) {
        throw std::runtime_error("MatrixData reading failed");
      }
    }
  }

  return matrix_data;
}

bool prep::operator==(const prep::Matrix& matrix, const MatrixData& matrix_data) {
  auto rows = matrix_data.size();
  auto cols = matrix_data[0].size();

  if (matrix.getRows() != rows || matrix.getCols() != cols) {
    return false;
  }

  for (size_t i = 0; i < matrix_data.size(); ++i) {
    for (size_t j = 0; j < matrix_data[i].size(); ++j) {
      if (matrix(i, j) != matrix_data[i][j]) {
        return false;
      }
    }
  }
  return true;
}

bool prep::operator==(const MatrixData& matrix_data, const prep::Matrix& matrix) {
  return matrix == matrix_data;
}


std::ostream& operator<<(std::ostream& os, const MatrixData& matrix_data) {
  auto rows = matrix_data.size();
  auto cols = matrix_data[0].size();

  os << rows << ' ' << cols << std::endl;

  for (auto& row: matrix_data) {
    for (auto& data: row) {
      os << data << ' ';
    }
    os << std::endl;
  }

  return os;
}

static std::random_device engine;

double genV(double from, double to) {
  return std::uniform_real_distribution<double>(from, to)(engine);
}

size_t genI(size_t from, size_t to) {
  return std::uniform_int_distribution<size_t>(from, to)(engine);
}

prep::Matrix genM(size_t rows, size_t cols) {
  size_t rows_ = rows ? rows : genI(1);
  size_t cols_ = cols ? cols : genI(1);

  prep::Matrix matrix(rows_, cols_);
  for (size_t i = 0; i < rows_; ++i) {
    for (size_t j = 0; j < cols_; ++j) {
      matrix(i, j) = genV();
    }
  }

  return matrix;
}

prep::Matrix genNonsquare() {
  size_t rows = genI(1);
  size_t cols = genI(1);
  cols = (cols == rows) ? (cols + 1) : cols;

  return genM(rows, cols);
}
