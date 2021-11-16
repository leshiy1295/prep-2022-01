#include <calc/matrix.hpp>
#include <net/client/client.hpp>
#include <tests/tests_generator.hpp>

#include <cassert>
#include <future>
#include <iostream>
#include <thread>
#include <tuple>
#include <vector>
#include <queue>

#define LOG_CLIENT_FAILURE { std::cerr << "Client failed on " << __FILE__ << ":" << __LINE__ << "\n"; }

static constexpr std::string_view kDefaultServerHostName = "127.0.0.1";
static constexpr unsigned int kDefaultServerPort = 8080;

#ifdef EXTRA
static constexpr unsigned int kCorrectClientsCount = 5;
static constexpr unsigned int kIncorrectClientsCount = 5;
static constexpr unsigned int kRequestsCount = 5;
#else
static constexpr unsigned int kCorrectClientsCount = 1;
static constexpr unsigned int kIncorrectClientsCount = 1;
static constexpr unsigned int kRequestsCount = 1;
#endif

static constexpr unsigned int kTestRandomSeed = 1234567890;

struct Task {
    Task(int id, Request request) : id(id), request(request) {}
    int id;
    Request request;

    friend std::ostream &operator<<(std::ostream &os, const Task &task) {
        os << "Task<id=" << task.id << ", request=" << task.request.content << ">";
        return os;
    }
};

enum class TestStatus {
    kOk,
    kError
};

TestStatus RunTests(std::string_view hostname, unsigned int port);

int main(int argc, char *argv[]) {
    const std::string_view hostname = argc > 1 ? argv[1] : kDefaultServerHostName;
    const unsigned int port = argc > 2 ? atoi(argv[2]) : kDefaultServerPort;

    auto tests_status = RunTests(hostname, port);
    if (tests_status == TestStatus::kOk) {
        std::cout << "**** TESTS SUCCEED ****\n";
    }

    return tests_status == TestStatus::kOk ? EXIT_SUCCESS : EXIT_FAILURE;
}

TestStatus RunTests(std::string_view hostname, unsigned int port) {
    const EndPoint server_endpoint{hostname, port};

    std::vector<std::future<TestStatus>> correct_clients;
    correct_clients.reserve(kCorrectClientsCount);

    std::vector<std::future<TestStatus>> incorrect_clients;
    incorrect_clients.reserve(kIncorrectClientsCount);

    static auto generate_tasks = [&](RandomMatrixTestGenerator &generator,
                                     Request::Type request_type = Request::Type::kPost) {
        std::queue<Task> tasks;

        for (unsigned int request_idx = 0; request_idx < kRequestsCount; ++request_idx) {
            auto test_data = generator.Generate();

            Client client;
            auto connection_status = client.ConnectTo(server_endpoint);
            assert(connection_status == ErrorStatus::kNoError);

            auto request = Request{
                request_type,
                test_data
            };
            auto [error_status, response] = client.SendRequest(request);
            assert(error_status == ErrorStatus::kNoError);

            if (request_type == Request::Type::kPost) {
                assert(response.type == Response::Type::kNew);
                tasks.push(Task{std::stoi(response.content), request});
            } else {
                assert(response.type == Response::Type::kError);
            }
        }

        return std::make_tuple(TestStatus::kOk, tasks);
    };

    static constexpr unsigned int correctClientRandomSeedOffset = 100500;

    for (unsigned int client_idx = 0; client_idx < kCorrectClientsCount; ++client_idx) {
        correct_clients.emplace_back(std::async(std::launch::async, [&, client_idx]{
            auto generator = RandomCorrectMatrixTestGenerator{kTestRandomSeed + client_idx + correctClientRandomSeedOffset};
            auto [status, tasks] = generate_tasks(generator);
            assert(status == TestStatus::kOk);

            while (!tasks.empty()) {
                auto task = tasks.front();
                tasks.pop();

                Client client;
                auto connection_status = client.ConnectTo(server_endpoint);
                assert(connection_status == ErrorStatus::kNoError);

                auto content_data = std::to_string(task.id);

                auto [error_status, response] = client.SendRequest(Request{
                    Request::Type::kGet,
                    content_data
                });

                assert(error_status == ErrorStatus::kNoError);

                assert(response.type == Response::Type::kWork || response.type == Response::Type::kDone);
                if (response.type == Response::Type::kWork) {
                    tasks.push(task);
                } else if (response.type == Response::Type::kDone) {
                    auto [error_status, expected] = Matrix::Process(task.request.content, true);
                    assert(error_status == Matrix::ErrorStatus::kNoError);

                    if (expected != response.content) {
                        std::cerr << "EXPECTED: '" << expected << "', RECEIVED: '" << response.content << "'\n";
                    }

                    assert(expected == response.content);
                }
            }

            return TestStatus::kOk;
        }));
    }

    for (unsigned int client_idx = 0; client_idx < kIncorrectClientsCount; ++client_idx) {
        incorrect_clients.emplace_back(std::async(std::launch::async, [&]{
            auto generator = RandomIncorrectMatrixTestGenerator{kTestRandomSeed + client_idx};
            auto [status, tasks] = generate_tasks(generator);
            assert(status == TestStatus::kOk);

            std::tie(status, std::ignore) = generate_tasks(generator, Request::Type::kGet);
            assert(status == TestStatus::kOk);
            std::tie(status, std::ignore) = generate_tasks(generator, Request::Type::kUnknown);
            assert(status == TestStatus::kOk);

            while (!tasks.empty()) {
                auto task = tasks.front();
                tasks.pop();

                Client client;
                auto connection_status = client.ConnectTo(server_endpoint);
                assert(connection_status == ErrorStatus::kNoError);

                auto content_data = std::to_string(task.id);

                auto [error_status, response] = client.SendRequest(Request{
                    Request::Type::kGet,
                    content_data
                });

                assert(error_status == ErrorStatus::kNoError);

                assert(response.type != Response::Type::kDone);

                if (response.type != Response::Type::kError) {
                    tasks.push(task);
                }
            }

            return TestStatus::kOk;
        }));
    }

    TestStatus final_status = TestStatus::kOk;

    for (size_t client_idx = 0; client_idx < kCorrectClientsCount; ++client_idx) {
        try {
            auto status = correct_clients[client_idx].get();
            if (status == TestStatus::kError) {
                std::cerr << "Client " << client_idx << " failed\n";
                final_status = TestStatus::kError;
            }
        }
        catch (const std::future_error &e) {
            std::cerr << "Client " << client_idx << " failed - " << e.what() << "\n";
            final_status = TestStatus::kError;
        }
    }

    for (size_t client_idx = 0; client_idx < kIncorrectClientsCount; ++client_idx) {
        try {
            auto status = incorrect_clients[client_idx].get();
            if (status != TestStatus::kOk) {
                std::cerr << "Incorrect client " << client_idx << " failed\n";
                final_status = TestStatus::kError;
            }
        }
        catch (const std::future_error &e) {
            std::cerr << "Incorrect client " << client_idx << " failed - " << e.what() << "\n";
            final_status = TestStatus::kError;
        }
    }

    return final_status;
}
