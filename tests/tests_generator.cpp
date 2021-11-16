#include <tests/tests_generator.hpp>

#include <array>
#include <iostream>
#include <functional>
#include <sstream>

std::string RandomMatrixTestGenerator::Generate() {
    const auto test_input_info = GenerateTestInputInfo();
    std::stringstream test_stream;

    test_stream << GenerateOperationCodeData(test_input_info.operation_code);
    test_stream << GenerateOperationData(test_input_info);

    return test_stream.str();
}

size_t RandomMatrixTestGenerator::GenerateSizeValue(size_t min_size, size_t max_size) {
    return std::uniform_int_distribution<size_t>{min_size, max_size}(generator_);
}

RandomMatrixTestGenerator::MatrixSize RandomMatrixTestGenerator::GenerateMatrixSize(size_t min_size, size_t max_size) {
    return { GenerateSizeValue(min_size, max_size), GenerateSizeValue(min_size, max_size) };
}

double RandomMatrixTestGenerator::GenerateDoubleValue(double min_value, double max_value) {
    return std::uniform_real_distribution<double>{min_value, max_value}(generator_);
}

std::string RandomMatrixTestGenerator::GenerateOperationCodeData(std::string_view operation_code) {
    std::stringstream operation_stream;
    operation_stream << operation_code << "\n";
    return operation_stream.str();
}

std::string RandomMatrixTestGenerator::GenerateOperationData(const RandomMatrixTestGenerator::TestInputInfo &test_input_info) {
    std::stringstream operation_stream;

    if (test_input_info.type == TestType::kConstantMul) {
        operation_stream << GenerateDoubleData();
    }

    for (const auto &matrix_size : test_input_info.matrix_sizes) {
        operation_stream << GenerateRandomMatrixData(matrix_size);
    }

    return operation_stream.str();
}

std::string RandomMatrixTestGenerator::GenerateDoubleData() {
    std::stringstream double_stream;
    double_stream << GenerateDoubleValue() << "\n";
    return double_stream.str();
}

std::string RandomMatrixTestGenerator::GenerateRandomMatrixData(const MatrixSize &matrix_size) {
    std::stringstream matrix_stream;
    matrix_stream << matrix_size.rows << " " << matrix_size.columns << "\n";

    for (size_t row_index = 0; row_index < matrix_size.rows; ++row_index) {
        if (matrix_size.columns == 0) {
            continue;
        }
        for (size_t column_index = 0; column_index < matrix_size.columns - 1; ++column_index) {
            matrix_stream << GenerateDoubleValue() << " ";
        }
        matrix_stream << GenerateDoubleValue() << "\n";
    }

    return matrix_stream.str();
}

RandomMatrixTestGenerator::TestInputInfo RandomCorrectMatrixTestGenerator::GenerateTestInputInfo() {
    static const std::array<TestInputInfo, 7> kTestsPrototypes = {{
        { TestType::kConstantMul, "cA" },
        { TestType::kMatricesSum, "A+B" },
        { TestType::kMatricesSub, "A-B" },
        { TestType::kMatricesMul, "AxB" },
        { TestType::kTransMatrix, "AT" },
        { TestType::kDeterMatrix, "detA" },
        { TestType::kInverMatrix, "invA" }
    }};

    size_t chosen_index = std::uniform_int_distribution<size_t>{0, std::size(kTestsPrototypes) - 1}(generator_);
    auto test_info = kTestsPrototypes[chosen_index];
    test_info.matrix_sizes = GenerateMatrixSizesForTestType(test_info.type);
    return test_info;
}

std::vector<RandomMatrixTestGenerator::MatrixSize> RandomCorrectMatrixTestGenerator::GenerateMatrixSizesForTestType(RandomMatrixTestGenerator::TestType type) {
    switch (type) {
      case TestType::kConstantMul: {
        return std::vector<MatrixSize>{ GenerateMatrixSize() };
      }
      case TestType::kMatricesSum:
      case TestType::kMatricesSub: {
        const auto matrix_size = GenerateMatrixSize();
        return std::vector<MatrixSize>{ matrix_size, matrix_size };
      }
      case TestType::kMatricesMul: {
        const auto first_matrix_size = GenerateMatrixSize();
        const auto second_matrix_size = MatrixSize{ first_matrix_size.columns, GenerateSizeValue() };
        return std::vector<MatrixSize>{ first_matrix_size, second_matrix_size };
      }
      case TestType::kTransMatrix: {
        return std::vector<MatrixSize>{ GenerateMatrixSize() };
      }
      case TestType::kDeterMatrix:
      case TestType::kInverMatrix: {
        const auto size = GenerateSizeValue();
        return std::vector<MatrixSize>{ {size, size} };
      }
      default: {
        throw std::invalid_argument{"Unknown test type"};
      }
    }

    return {};
}

RandomMatrixTestGenerator::TestInputInfo RandomIncorrectMatrixTestGenerator::GenerateTestInputInfo() {
    static const std::array<TestInputInfo, 8> kTestsPrototypes = {{
#ifdef EXTRA
        { TestType::kConstantMul, "cA" },
        { TestType::kMatricesSum, "A+B" },
        { TestType::kMatricesSub, "A-B" },
        { TestType::kMatricesMul, "AxB" },
        { TestType::kTransMatrix, "AT" },
        { TestType::kDeterMatrix, "detA" },
        { TestType::kInverMatrix, "invA" },
        { TestType::kUnsupported, "???" }
#else
        { TestType::kConstantMul, "ErrorcA" },
        { TestType::kMatricesSum, "ErrorA+B" },
        { TestType::kMatricesSub, "ErrorA-B" },
        { TestType::kMatricesMul, "ErrorAxB" },
        { TestType::kTransMatrix, "ErrorAT" },
        { TestType::kDeterMatrix, "ErrordetA" },
        { TestType::kInverMatrix, "ErrorinvA" },
        { TestType::kUnsupported, "Error???" }
#endif
    }};

    size_t chosen_index = std::uniform_int_distribution<size_t>{0, std::size(kTestsPrototypes) - 1}(generator_);
    auto test_info = kTestsPrototypes[chosen_index];
    test_info.matrix_sizes = GenerateMatrixSizesForTestType(test_info.type);
    return test_info;
}

std::vector<RandomMatrixTestGenerator::MatrixSize> RandomIncorrectMatrixTestGenerator::GenerateMatrixSizesForTestType(RandomMatrixTestGenerator::TestType type) {
    static auto offset_by_one = [&](auto &value){
        auto sign = int(GenerateSizeValue(0, 1)) * 2 - 1;
        value += sign;
    };

    switch (type) {
    case TestType::kConstantMul: {
        return std::vector<MatrixSize>{ GenerateMatrixSize(0, 0) };
    }
    case TestType::kMatricesSum:
    case TestType::kMatricesSub: {
        const auto first_matrix_size = GenerateMatrixSize();
        const auto second_matrix_size = [&]{
            auto size = GenerateMatrixSize();

            if (first_matrix_size.rows == size.rows) {
                offset_by_one(size.rows);
            }

            if (first_matrix_size.columns == size.columns) {
                offset_by_one(size.columns);
            }

            return size;
        }();
        return std::vector<MatrixSize>{ first_matrix_size, second_matrix_size };
    }
    case TestType::kMatricesMul: {
        const auto first_matrix_size = GenerateMatrixSize();
        const auto second_matrix_size = [&]{
            auto size = GenerateMatrixSize();

            if (first_matrix_size.columns == size.rows) {
                offset_by_one(size.rows);
            }

            return size;
        }();
        return std::vector<MatrixSize>{ first_matrix_size, second_matrix_size };
    }
    case TestType::kTransMatrix: {
        return std::vector<MatrixSize>{ GenerateMatrixSize(0, 0) };
    }
    case TestType::kDeterMatrix:
    case TestType::kInverMatrix: {
        const auto rows = GenerateSizeValue();
        const auto cols = [&]{
            auto size = GenerateSizeValue();

            if (size == rows) {
                offset_by_one(size);
            }

            return size;
        }();
        return std::vector<MatrixSize>{ {rows, cols} };
    }
    case TestType::kUnsupported: {
        const auto matrices_count = GenerateSizeValue();
        std::vector<MatrixSize> matrix_sizes;
        matrix_sizes.reserve(matrices_count);
        for (size_t matrix_idx = 0; matrix_idx < matrices_count; ++matrix_idx) {
            matrix_sizes.emplace_back( GenerateMatrixSize(0, kDefaultMaxSize) );
        }
        return matrix_sizes;
    }
    default:
        throw std::invalid_argument{"Unknown test type"};
    }

    return {};
}

/*
int main() {
    {
        std::cout << "Correct examples\n";
        auto generator = RandomCorrectMatrixTestGenerator{};
        for (size_t i = 1; i <= 25; ++i) {
            std::cout << "Example " << i << "\n";
            std::cout << generator.Generate();
        }
    }
    {
        std::cout << "Incorrect examples\n";
        auto generator = RandomIncorrectMatrixTestGenerator{};
        for (size_t i = 1; i <= 25; ++i) {
            std::cout << "Example " << i << "\n";
            std::cout << generator.Generate();
        }
    }
    return 0;
}
*/