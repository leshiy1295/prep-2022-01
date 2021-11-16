#include <calc/matrix.hpp>

#include <net/server/worker.hpp>

#include <csignal>
#include <filesystem>
#include <fstream>

struct Worker::WorkerImpl {
    WorkerImpl(const Pipe &to_worker_pipe, const Pipe &from_worker_pipe, std::string_view working_directory) :
        to_worker_pipe_(to_worker_pipe),
        from_worker_pipe_(from_worker_pipe),
        working_directory_(working_directory) {}

    ErrorStatus Run();
    static ErrorStatus CleanWorkingDirectory(std::string_view working_directory);

  private:
    static void HandlerWorkerSignal(int sig_number) {
        // TODO: а что же должен делать таск-воркер по SIGINT и иным сигналам?
    }

    static constexpr std::string_view kProcessTaskPrefix = "task-";

    ErrorStatus RunReceiveTask(int client_pid);
    ErrorStatus RunGetQueueUpdate(int client_pid, int task_id);
    ErrorStatus RunPostQueueUpdate(int client_pid, std::string_view task_content);

    ErrorStatus RunProcessTask(int task_id);

    ErrorStatus SendResponse(int client_pid, const Response &response);

    std::tuple<ErrorStatus, std::string> LoadTask(int task_id);
    ErrorStatus SaveTask(int task_id, std::string_view task_content);
    void RemoveTask(int task_id);

    std::filesystem::path TaskPath(int task_id);

    inline static volatile bool worker_running_ = false;

    AutoClosingPipe to_worker_pipe_;
    AutoClosingPipe from_worker_pipe_;
    std::string_view working_directory_;
};

ErrorStatus Worker::WorkerImpl::CleanWorkingDirectory(std::string_view working_directory) {
    for (const auto &entry : std::filesystem::directory_iterator(working_directory)) {
        // TODO: Удаляем все созданные файлы с задачами при окончании работы
    }

    return ErrorStatus::kNoError;
}

Worker::Worker(const Pipe &to_worker_pipe, const Pipe &from_worker_pipe, std::string_view working_directory) :
    impl_(std::make_unique<WorkerImpl>(to_worker_pipe, from_worker_pipe, working_directory)) {}

ErrorStatus Worker::Run() { return impl_->Run(); }

Worker::~Worker() = default;

ErrorStatus Worker::CleanWorkingDirectory(std::string_view working_directory) {
    return WorkerImpl::CleanWorkingDirectory(working_directory);
}

std::filesystem::path Worker::WorkerImpl::TaskPath(int task_id) {
    auto result = std::filesystem::path{working_directory_} /
        (std::filesystem::path{kProcessTaskPrefix}.concat(std::to_string(task_id)));
#ifdef DEBUG
    LOG_MESSAGE << "[WORKER pid=" << getpid() << "]" << "TaskPath(" << task_id << ") = " << result << "\n";
#endif
    return result;
}

ErrorStatus Worker::WorkerImpl::SendResponse(int client_pid, const Response &response) {
    // TODO: Сериализуем результат (response) в массив байт и отправляем в NamedPipe to_net
}

void Worker::WorkerImpl::RemoveTask(int task_id) {
#ifdef DEBUG
    LOG_MESSAGE << "[WORKER pid=" << getpid() << "]" << "RemoveTask(" << task_id << ")\n";
#endif
    // TODO: удаляем файл с задачей
}

std::tuple<ErrorStatus, std::string> Worker::WorkerImpl::LoadTask(int task_id) {
#ifdef DEBUG
    LOG_MESSAGE << "[WORKER pid=" << getpid() << "]" << "LoadTask(" << task_id << ")\n";
#endif
    // TODO: Читаем содержимое файла с задачей и бросаем ошибку, если случилась
}

ErrorStatus Worker::WorkerImpl::SaveTask(int task_id, std::string_view task_content) {
    // TODO: Сохраняем задачу в файл TaskPath()
}

// обработка POST-запроса от клиента
ErrorStatus Worker::WorkerImpl::RunPostQueueUpdate(int client_pid, std::string_view task_content) {
    // Запрашиваем уникальный id
    if (from_worker_pipe_.Write(UpdateQueueRequest{UpdateQueueRequest::Type::kCreate}) == ErrorStatus::kError) {
        // TODO: что делать при подобных ошибках?
    }

    // TODO: получаем ответ от планировщика с уникальным id
    UpdateQueueResponse response;
    auto read_status = to_worker_pipe_.Read(response);

    // TODO: Сохраняем задачу в файл TaskPath()
    // (не забываем обрабатывать всевозможные ошибки)

    // TODO: Обновляем статус задачи на New, если сохранение в файл прошло успешно
    // Удаляем задачу - если ошибка
    // Отправляем net-воркеру результат
    SendResponse(client_pid, Response{/*...*/});

    // TODO: что возвращаем?
}

// TODO: обработка GET-запроса пользователя
ErrorStatus Worker::WorkerImpl::RunGetQueueUpdate(int client_pid, int task_id) {
    // TODO: Запрашиваем статус задачи по id у планировщика
    // Отправляем ответ в зависимости от полученного статуса

    switch (task_status) {
      case WorkerTask::Status::kNew:
      case WorkerTask::Status::kWork: {
        return SendResponse(client_pid, Response{
            // TODO: Implement me
        });
      }
      case WorkerTask::Status::kDone: {
        // TODO: в Done и Error не забываем удалять файлы с диска и очереди!
        if (error_status == ErrorStatus::kError) {
            SendResponse(client_pid, Response{
                // TODO: Implement me
            });
            return ErrorStatus::kError;
        }

        return SendResponse(client_pid, Response{
            // TODO: Implement me
        });
      }
      case WorkerTask::Status::kError: {
        RemoveTask(task_id);
        return SendResponse(client_pid, Response{
            // TODO: Implement me
        });
      }
      case WorkerTask::Status::kUnknown:
      default: {
        return SendResponse(client_pid, Response{
            // TODO: Implement me
        });
      }
    }

    return ErrorStatus::kNoError;
}

// TODO: Обработка Receive-задачи
ErrorStatus Worker::WorkerImpl::RunReceiveTask(int client_pid) {
    // TODO: Считываем из named-pipe from-net данные

    std::vector<std::byte> network_bytes;
    auto read_status = named_pipe.Read(network_bytes, kFileDescriptor_NonBlocking);

#ifdef DEBUG
    LOG_MESSAGE << "[WORKER pid=" << getpid() << "]" << "Read from named pipe from pid = " << client_pid << " result...\n";
    std::transform(std::begin(network_bytes), std::end(network_bytes), std::ostream_iterator<int>(LOGGER, " "),
                    [](const auto &byte) {
                        return static_cast<int>(byte);
                    });
    LOGGER << "\n";
#endif

    if (read_status == ReadStatus::kFailed) {
#ifdef DEBUG
        LOG_ERROR << "[WORKER pid=" << getpid() << "]" << "Failed read...\n";
#endif
        return ErrorStatus::kError;
    }

    // TODO: Десереализуем ответ (с обработкой ошибок)
    auto [error_status, request] = NetworkRequest::Deserialize(network_bytes);

#ifdef DEBUG
    LOG_MESSAGE << "[WORKER pid=" << getpid() << "]" << "Worker recevied request from named pipe - " << request << "\n";
#endif

    // TODO: if error_status -> plan Send failure (write message to file and send)
    // if no error_status ->
    // if get -> ask planner for status and plan send
    // if post -> ask planner for create, move data from "from-net" to task- and write to client- "<task_id> OK"

    if (read_status == ReadStatus::kSucceed) {
        switch (request.type) {
          case Request::Type::kGet: {
            // TODO: Implement me
          }
          case Request::Type::kPost: {
            // TODO: Implement me
          }
          case Request::Type::kUnknown:
          default: {
            // TODO: Implement me
          }
        }
    }

    // TODO: что возвращаем?
}

// TODO: Обработка задачи типа Process
ErrorStatus Worker::WorkerImpl::RunProcessTask(int task_id) {
    // TODO: Загружаем данные из файла TaskPath

    // TODO: Запускаем обработку
    Matrix::Process(task)

    // TODO: Сохраняем результат
    SaveTask(task_id, task_result)

    // TODO: Обновляем статус планировщика
    from_worker_pipe_.Write(UpdateQueueRequest{/*...*/});

    // TODO: Что возвращаем?
}

// TODO: Основная функция task-воркера
ErrorStatus Worker::WorkerImpl::Run() {
    // TODO: ask planner for task
    // receive task
    // execute it
    // send status to planner

#ifdef DEBUG
    LOG_MESSAGE << "[WORKER] My pid is " << getpid() << "...\n";
#endif

    auto worker_status = ErrorStatus::kNoError;

    // TODO: регистрируем сигналы

    worker_running_ = true;

    while (worker_running_) {
        // TODO: Просим очередную задачу у планировщика
        from_worker_pipe_.Write(UpdateQueueRequest{UpdateQueueRequest::Type::kGet}

        // TODO: Если нет задач - спим
        // Если пришла - обрабатываем

        switch (received_task.task_type) {
            case WorkerTask::Type::kReceive: {
                // TODO: Implement me
                task_result = RunReceiveTask(/*...*/);

                break;
            }
            case WorkerTask::Type::kProcess: {
                // TODO: Implement me
                task_result = RunProcessTask(/*...*/);
                if (task_result == ErrorStatus::kError) {
#ifdef DEBUG
                    LOG_ERROR << "[WORKER pid=" << getpid() << "]" << "Failed RunProcessTask(" << received_task.id.for_task << ")...\n";
#endif
                }
                break;
            }
            default: {
#ifdef DEBUG
              LOG_ERROR << "[WORKER pid=" << getpid() << "]" << "Received unknown worker task type...\n";
#endif
              task_result = ErrorStatus::kError;
            break;
            }
        }

        if (task_result == ErrorStatus::kError) {
#ifdef DEBUG
            LOG_ERROR << "[WORKER pid=" << getpid() << "]" << "Failed task...\n";
#endif
            worker_status = ErrorStatus::kError;
            worker_running_ = false;
        }
    }

#ifdef DEBUG
    LOG_MESSAGE << "[WORKER " << getpid() << "] Finishing...\n";
#endif

    return worker_status;
}
