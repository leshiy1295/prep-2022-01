#pragma once

#include <random>
#include <string>
#include <vector>

struct RandomMatrixTestGenerator {
    RandomMatrixTestGenerator(unsigned int random_seed = std::random_device{}()) : generator_{random_seed} {}
    virtual ~RandomMatrixTestGenerator() = default;

    std::string Generate();
protected:
    struct MatrixSize {
        size_t rows;
        size_t columns;
    };

    enum class TestType {
        kConstantMul,
        kMatricesSum,
        kMatricesSub,
        kMatricesMul,
        kTransMatrix,
        kDeterMatrix,
        kInverMatrix,
        kUnsupported
    };

    struct TestInputInfo {
        TestType type;
        std::string_view operation_code;
        std::vector<MatrixSize> matrix_sizes = {};
    };

    static constexpr size_t kDefaultMinSize = 1;
    static constexpr size_t kDefaultMaxSize = 5;

    static constexpr double kDefaultMinDouble = -10;
    static constexpr double kDefaultMaxDouble = 10;

    inline size_t GenerateSizeValue(size_t min_size = kDefaultMinSize, size_t max_size = kDefaultMaxSize);
    inline MatrixSize GenerateMatrixSize(size_t min_size = kDefaultMinSize, size_t max_size = kDefaultMaxSize);
    inline double GenerateDoubleValue(double min_value = kDefaultMinDouble, double max_value = kDefaultMaxDouble);

    virtual TestInputInfo GenerateTestInputInfo() = 0;
    virtual std::vector<MatrixSize> GenerateMatrixSizesForTestType(TestType type) = 0;

    std::string GenerateOperationCodeData(std::string_view operation_code);
    std::string GenerateOperationData(const TestInputInfo &test_info);
    std::string GenerateDoubleData();
    std::string GenerateRandomMatrixData(const MatrixSize &size);

    std::mt19937 generator_;
};

struct RandomCorrectMatrixTestGenerator final : RandomMatrixTestGenerator {
    RandomCorrectMatrixTestGenerator(unsigned int random_seed = std::random_device{}()) : RandomMatrixTestGenerator(random_seed) {}
private:
    TestInputInfo GenerateTestInputInfo() override;
    std::vector<MatrixSize> GenerateMatrixSizesForTestType(TestType type) override;
};

struct RandomIncorrectMatrixTestGenerator final : RandomMatrixTestGenerator {
    RandomIncorrectMatrixTestGenerator(unsigned int random_seed = std::random_device{}()) : RandomMatrixTestGenerator(random_seed) {}
private:
    TestInputInfo GenerateTestInputInfo() override;
    std::vector<MatrixSize> GenerateMatrixSizesForTestType(TestType type) override;
};
