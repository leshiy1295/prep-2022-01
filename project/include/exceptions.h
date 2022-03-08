#pragma once // NOLINT

#include <exception>
#include <string>

#include "matrix.h"

namespace prep {
class MatrixException: public std::exception {
 public:
  explicit MatrixException(const std::string& msg)
      : m_msg(msg) {}

  const char* what() const noexcept override {
    return m_msg.c_str();
  }

 private:
  std::string m_msg;
};

class InvalidMatrixStream: public MatrixException {
 public:
  InvalidMatrixStream()
      : MatrixException("Error occurred while reading from stream") {}
};

class OutOfRange: public MatrixException {
 public:
  OutOfRange(size_t i, size_t j, const Matrix& matrix)
      : MatrixException(
      "Indexes (" + std::to_string(i) + ", " + std::to_string(j) + ") are out of range. Matrix size is [" +
      std::to_string(matrix.getRows()) + ", " + std::to_string(matrix.getCols()) + "]"
  ) {}
};

class DimensionMismatch: public MatrixException {
 public:
  DimensionMismatch(const Matrix& m1, const Matrix& m2)
      : MatrixException(
      "M1[" + std::to_string(m1.getRows()) + ", " + std::to_string(m1.getCols()) + "] and M2[" +
      std::to_string(m2.getRows()) + ", " + std::to_string(m2.getCols()) + "] are not compatible"
  ) {}

  explicit DimensionMismatch(const Matrix& m)
      : MatrixException(
      "M[" + std::to_string(m.getRows()) + ", " + std::to_string(m.getCols()) + "] isn't square matrix"
  ) {}
};

class SingularMatrix: public MatrixException {
 public:
  SingularMatrix()
      : MatrixException("Singular matrix") {}
};
}  // namespace prep
