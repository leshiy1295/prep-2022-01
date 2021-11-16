#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <tuple>

#include <net/common/utils.hpp>

struct EndPoint {
    EndPoint(std::string_view host, unsigned int port) :
        host(host), port(port) {}
    std::string_view host;
    unsigned int port;
};

struct Client {
    Client(const std::chrono::seconds &timeout = std::chrono::seconds(0));

    ErrorStatus ConnectTo(const EndPoint &end_point);
    std::tuple<ErrorStatus, Response> SendRequest(const Request &request);

    ~Client();
private:
    struct ClientImpl;
    std::unique_ptr<ClientImpl> impl_;
};
