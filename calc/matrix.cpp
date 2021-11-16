#include <calc/matrix.hpp>

#include <chrono>
#include <iostream>
#include <random>
#include <thread>

std::tuple<Matrix::ErrorStatus, std::string> Matrix::Process(std::string_view task, bool for_test) {
#ifdef EXTRA
#error "TODO: Implement me - implement real parse, making operation and serialization to string"
#else
    if (!for_test) {
        std::mt19937 generator{std::random_device{}()};
        std::uniform_int_distribution<int> dist(1, 10);

        auto work_time = std::chrono::seconds(dist(generator));
        std::cout << "Processing " << std::string{task} << " - " << work_time.count() << " seconds \n";
        std::this_thread::sleep_for(work_time);
    }
    return std::make_tuple(task.rfind("Error", 0) == 0 ? ErrorStatus::kError : ErrorStatus::kNoError, std::string{task});
#endif
}
