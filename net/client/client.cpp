#include <net/client/client.hpp>

#include <arpa/inet.h>
#include <cstring>
#include <cstddef>
#include <memory>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <tuple>
#include <unistd.h>
#include <vector>

struct Client::ClientImpl {
    ClientImpl(const std::chrono::seconds &timeout) : timeout(timeout) {}

    std::tuple<ErrorStatus, std::vector<std::byte>> SendNetworkRequest(const std::vector<std::byte> &request);
    void CloseConnectionIfNeeded();
    int socket_fd = kBadFileDescriptor;
    std::chrono::seconds timeout = std::chrono::seconds(0);
    bool connected = false;
};

Client::Client(const std::chrono::seconds &timeout) : impl_(std::make_unique<ClientImpl>(timeout)) {}

void Client::ClientImpl::CloseConnectionIfNeeded() {
    if (socket_fd < 0) {
        return;
    }
    connected = false;
    close(socket_fd);
    socket_fd = kBadFileDescriptor;
}

Client::~Client() {
    impl_->CloseConnectionIfNeeded();
}

ErrorStatus Client::ConnectTo(const EndPoint &end_point) {
    // TODO - создать сокет и подключиться к серверу, вернуть ошибку, если не получилось
    impl_->connected = true;
    return ErrorStatus::kNoError;
}

#include <iostream>

 std::tuple<ErrorStatus, std::vector<std::byte>> Client::ClientImpl::SendNetworkRequest(const std::vector<std::byte> &request) {
    if (!connected) {
#ifdef DEBUG
        LOG_ERROR << "No active connection\n";
#endif
        return std::make_tuple(ErrorStatus::kError, std::vector<std::byte>{});
    }

#ifdef DEBUG
    LOG_MESSAGE << "Trying send to network\n";
#endif

    if (SendToNetwork(socket_fd, request) == ErrorStatus::kError) {
#ifdef DEBUG
        LOG_ERROR << "Failed to send network request\n";
#endif
        CloseConnectionIfNeeded();
        return std::make_tuple(ErrorStatus::kError, std::vector<std::byte>{});
    }

#ifdef DEBUG
    LOG_MESSAGE << "Trying read from network\n";
#endif

    std::vector<std::byte> response_bytes;
    if (ReadFromNetwork(socket_fd, response_bytes, true, timeout) != ReadStatus::kSucceed) {
#ifdef DEBUG
        LOG_ERROR << "Read from network failed\n";
#endif
        CloseConnectionIfNeeded();
        return std::make_tuple(ErrorStatus::kError, std::vector<std::byte>{});
    }

    return std::make_tuple(ErrorStatus::kNoError, response_bytes);
}

std::tuple<ErrorStatus, Response> Client::SendRequest(const Request &request) {
#ifdef DEBUG
    LOG_MESSAGE << "Trying to send " << request << "\n";
#endif
    auto request_as_bytes = NetworkRequest::Serialize(request);
    auto [send_error_status, response_bytes] = impl_->SendNetworkRequest(request_as_bytes);
    if (send_error_status == ErrorStatus::kError) {
#ifdef DEBUG
        LOG_ERROR << "Network error\n";
        return std::make_tuple(ErrorStatus::kError, Response{});
#endif
    }
    auto [deserialize_error_status, response] = NetworkResponse::Deserialize(response_bytes);
#ifdef DEBUG
    if (deserialize_error_status == ErrorStatus::kError) {
        LOG_ERROR << "Failed to deserialize network response\n";
        return std::make_tuple(ErrorStatus::kError, Response{});
    }

    LOG_MESSAGE << "Received " << response << "\n";
#endif
    impl_->CloseConnectionIfNeeded();
    return std::make_tuple(ErrorStatus::kNoError, response);
}

#ifndef TESTS_ENABLED
#ifdef DEBUG
#include <cassert>
#include <tests/tests_generator.hpp>

static const unsigned int kTestRandomSeed = 123456789;

int main(int argc, char *argv[]) {
    const char *hostname = argc > 1 ? argv[1] : "127.0.0.1";
    unsigned int port = argc > 2 ? atoi(argv[2]) : 8080;
    Client c;
    assert(c.ConnectTo(EndPoint{hostname, port}) == ErrorStatus::kNoError);
    auto correct_generator = RandomCorrectMatrixTestGenerator{kTestRandomSeed};
    auto test_data = correct_generator.Generate();
    auto [error_status, initial_response] = c.SendRequest(Request{Request::Type::kPost, test_data});
    assert(error_status == ErrorStatus::kNoError);
    std::cout << "Received response: " << initial_response << "\n";
    assert(initial_response.type == Response::Type::kNew);

    do {
        assert(c.ConnectTo(EndPoint{hostname, port}) == ErrorStatus::kNoError);
        auto [error_status, response] = c.SendRequest(Request{Request::Type::kGet, initial_response.content});
        assert(error_status == ErrorStatus::kNoError);
        assert(response.type != Response::Type::kError);
        std::cout << "Received response: " << response << "\n";

        if (response.type == Response::Type::kDone) {
            assert(response.content == test_data);
            break;
        }

        assert(response.content == initial_response.content);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } while (true);
}
#endif
#endif
