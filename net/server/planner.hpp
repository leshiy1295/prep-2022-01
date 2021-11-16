#pragma once

#include <net/common/utils.hpp>

#include <memory>
#include <string>

struct Planner {
    Planner(int incoming_fd, size_t max_workers_count, std::string_view working_directory);

    ErrorStatus Start();
    ErrorStatus Stop();

    ~Planner();

  private:
    struct PlannerImpl;
    std::unique_ptr<PlannerImpl> impl_;
};
