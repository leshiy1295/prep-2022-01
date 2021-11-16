#include <net/server/planner.hpp>
#include <net/server/worker.hpp>

#include <algorithm>
#include <csignal>
#include <deque>
#include <sys/wait.h>

struct Planner::PlannerImpl {
    PlannerImpl(int incoming_fd, size_t max_workers_count, std::string_view working_directory) :
        incoming_fd_(incoming_fd), max_workers_count_(max_workers_count), working_directory_(working_directory) {}

    static void HandlePlannerSignal(int sig_number) {
        // TODO: Что нужно делать по тому или иному сигналу?
    }

    ErrorStatus Start();
    ErrorStatus Stop();

  private:
    inline static volatile bool planner_running_ = 0;

    AutoClosingFileDescriptor incoming_fd_;
    size_t max_workers_count_;
    std::string_view working_directory_;

    std::vector<WorkerInfo> workers_;
};

Planner::Planner(int incoming_fd, size_t max_workers_count, std::string_view working_directory) :
    impl_(std::make_unique<PlannerImpl>(incoming_fd, max_workers_count, working_directory)) {}

Planner::~Planner() { Stop(); }

ErrorStatus Planner::Start() { return impl_->Start(); }

ErrorStatus Planner::Stop() { return impl_->Stop(); }

ErrorStatus Planner::PlannerImpl::Stop() {
    // TODO - при остановке нужно корректно завершить работу дочерних таск-воркеров
}

ErrorStatus Planner::PlannerImpl::Start() {
#ifdef DEBUG
    LOG_MESSAGE << "[PLANNER] My pid is " << getpid() << "...\n";
#endif
    workers_.reserve(max_workers_count_);

    std::deque<WorkerTask> planned_tasks;

    int last_task_id = 0;

    // TODO: регистрация обработчиков сигналов (SIGINT/...)
    planner_running_ = true;

    auto planner_error_status = ErrorStatus::kNoError;

    while (planner_running_) {
        bool queue_updated = false;

        // TODO: чтение incoming_fd_ (pipe от основного серверного процесса) в поисках net_pid

        // TODO: не забываем обрабатывать ошибки
        // Read(incoming_fd) -> net_pid, read_status

        if (read_status == ReadStatus::kSucceed) {
#ifdef DEBUG
            LOG_MESSAGE << "Planned added new receive task - " << net_pid << "\n";
#endif

            // TODO: Добавляем задачу Receive
            queue_updated = true;
        }

        for (auto &worker : workers_) {
            // TODO: если ничего не запросил worker - переходим к следующему
            // worker.from_worker_pipe.Read() -> request, read_status
            // если запросил - обрабатываем далее

#ifdef DEBUG
            LOG_MESSAGE << "Received request from worker " << worker.worker_pid << ": " << request << "\n";
#endif

            UpdateQueueResponse response;

#ifdef DEBUG
            LOG_MESSAGE << "Current planned tasks\n";
            for (const auto &task : planned_tasks) {
                LOG_MESSAGE << "TASK -> " << task << "\n";
            }
#endif

            switch (request.type) {
              case UpdateQueueRequest::Type::kNew: {
                // TODO: добавление Receive-задачи
              }
              case UpdateQueueRequest::Type::kCreate: {
                // TODO: генерация нового id (возможно, сразу с задачей типа Unknown)
              }
              case UpdateQueueRequest::Type::kDelete: {
                // TODO: удаление задачи по id
              }
              case UpdateQueueRequest::Type::kCheck: {
                // TODO: проверка статуса задачи по id
              }
              case UpdateQueueRequest::Type::kGet: {
                // TODO: получение новой (New) задачи из очереди и перевод её статуса в Work
              }
              case UpdateQueueRequest::Type::kDone: {
                // TODO: перевод задачи в статус Done
              }
              case UpdateQueueRequest::Type::kError: {
                // TODO: перевод задачи в статус Error
              }
              case UpdateQueueRequest::Type::kUnknown:
              default: {
#ifdef DEBUG
                LOG_ERROR << "[PLANNER] Unknown queue request type...\n";
#endif
                break;
              }
            }

#ifdef DEBUG
            LOG_MESSAGE << "Sending response to worker " << worker << ": " << response << "\n";
#endif
            if (worker.to_worker_pipe.Write(response) == ErrorStatus::kError) {
#ifdef DEBUG
                LOG_ERROR << "[PLANNER] Broken planner write...\n";
#endif
                // TODO: что делать при ошибке?
            }
        }

        // TODO: если всё сделали, нераспределённых задач нет, очередь не менялась - можно и поспать немного (например, 100 мс)
        // Например, это можно сделать примерно так...
        /*if (!queue_updated) {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(100ms);
            continue;
        }*/

        // TODO: если есть нераспределённые задачи - создаём нового воркера (если есть ещё места в пуле)

        if (workers_.size() == max_workers_count_) {
            continue;
        }

        // TODO: при создании новых task-воркеров нужно создать pipe_from_worker и pipe_to_worker для связи с ними
        // pipe_from_worker = Pipe::Create(NonBlocking);

        auto new_worker_pid = fork();
        if (new_worker_pid < 0) {
            // TODO - что делать при ошибках?
        }

        if (new_worker_pid == 0) {
            // TODO: запускаем task-воркеров
            auto worker_exit_status = Worker{pipe_to_worker, pipe_from_worker, working_directory_}.Run();
            return worker_exit_status;
        }

        // TODO: сохраняем информацию о новом воркере, чтобы потом не забыть удалить!

#ifdef DEBUG
        LOG_MESSAGE << "Adding new worker " << new_worker_pid << " pipe_to_worker = " << pipe_to_worker.ReaderFd() << ":" << pipe_to_worker.WriterFd() << ", pipe_from_worker = " << pipe_from_worker.ReaderFd() << ":" << pipe_from_worker.WriterFd() << "\n";
#endif
    }

#ifdef DEBUG
    LOG_MESSAGE << "[PLANNER] After while stopping planner, planner_error_status = Error " << (planner_error_status == ErrorStatus::kError) << "...\n";
#endif

    if (Stop() == ErrorStatus::kError) {
        planner_error_status = ErrorStatus::kError;
    }

#ifdef DEBUG
    if (planner_error_status == ErrorStatus::kError) {
        LOG_MESSAGE << "[PLANNER] Finishing with error...\n";
    } else {
        LOG_MESSAGE << "[PLANNER] Finishing OK...\n";
    }

#endif

    return planner_error_status;
}
