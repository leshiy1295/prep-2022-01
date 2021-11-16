#pragma once

#include <memory>
#include <string>

#include <net/common/utils.hpp>

static constexpr unsigned int kDefaultServerPort = 8080;
static constexpr std::string_view kDefaultWorkingDirectory = "/tmp";
static constexpr size_t kDefaultMaxWorkersCount = 1;

struct Server {
    Server(unsigned int port = kDefaultServerPort,
           std::string_view working_directory = kDefaultWorkingDirectory,
           size_t max_workers_count = kDefaultMaxWorkersCount);
    ~Server();
    ErrorStatus Start();

private:
    struct ServerImpl;
    std::unique_ptr<ServerImpl> impl_;
};
