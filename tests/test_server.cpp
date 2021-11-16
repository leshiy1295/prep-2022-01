#include <cassert>
#include <string>

#include <net/server/server.hpp>

int main(int argc, char *argv[]) {
    const unsigned int port = argc > 1 ? std::stoi(argv[1]) : kDefaultServerPort;
    const std::string_view working_directory = argc > 2 ? argv[2] : kDefaultWorkingDirectory;
    const size_t max_workers_count = argc > 3 ? std::stoi(argv[3]) : kDefaultMaxWorkersCount;

    auto server = Server{port, working_directory, max_workers_count};
    assert(server.Start() == ErrorStatus::kNoError);
}
