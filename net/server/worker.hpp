#pragma once

#include <net/common/utils.hpp>

#include <memory>
#include <string>

struct WorkerInfo {
    WorkerInfo(pid_t worker_pid, const Pipe &to_worker_pipe, const Pipe &from_worker_pipe) :
        worker_pid(worker_pid), to_worker_pipe(to_worker_pipe), from_worker_pipe(from_worker_pipe) {}

    pid_t worker_pid;
    AutoClosingPipe to_worker_pipe;
    AutoClosingPipe from_worker_pipe;

#ifdef DEBUG
    friend std::ostream &operator<<(std::ostream &os, const WorkerInfo &wi) {
        os << "WorkerInfo<pid=" << wi.worker_pid << ", to_worker_pipe" << wi.to_worker_pipe << ", from_worker_pipe=" << wi.from_worker_pipe << ">";
        return os;
    }
#endif
};

union TaggedId {
    enum class Tag {
        kNet,
        kTask
    };

    static constexpr int kUnknown = -1;

    TaggedId(int id = kUnknown, Tag tag_for = Tag::kTask)
#ifdef DEBUG
        : tag(tag_for)
#endif
    {
        if (tag_for == Tag::kNet) {
            for_net = id;
        } else {
            for_task = id;
        }
    }

#ifdef DEBUG
    friend std::ostream &operator<<(std::ostream &os, const TaggedId &id) {
        if (id.tag == TaggedId::Tag::kTask) {
            os << "Tag=Task, id=" << id.for_task;
        } else {
            os << "Tag=Net, id=" << id.for_net;
        }
        return os;
    }

    Tag tag;
#endif
    int for_task;
    int for_net;
};

struct WorkerTask {
    enum class Type {
        kUnknown,
        kReceive,
        kProcess
    };

    enum class Status {
        kUnknown,
        kNew,
        kWork,
        kDone,
        kError
    };

    WorkerTask(int id, Status task_status, Type type, TaggedId::Tag tag = TaggedId::Tag::kTask) :
            id(id, tag), status(task_status), type(type) {}

    TaggedId id;
    Status status;
    Type type;
};

#ifdef DEBUG
inline std::ostream &operator<<(std::ostream &os, const WorkerTask::Type &type) {
    os << "WorkerTask::Type=";
    switch (type) {
      case WorkerTask::Type::kProcess: { os << "Process"; break; }
      case WorkerTask::Type::kReceive: { os << "Receive"; break; }
      case WorkerTask::Type::kUnknown:
      default: { os << "Unknown"; break; }
    }
    return os;
}

inline std::ostream &operator<<(std::ostream &os, const WorkerTask::Status &status) {
    os << "WorkerTask::Status=";
    switch (status) {
      case WorkerTask::Status::kDone: { os << "Done"; break; }
      case WorkerTask::Status::kError: { os << "Error"; break; }
      case WorkerTask::Status::kNew: { os << "New"; break; }
      case WorkerTask::Status::kWork: { os << "Work"; break; }
      case WorkerTask::Status::kUnknown:
      default: { os << "Unknown"; break; }
    }
    return os;
}

inline std::ostream &operator<<(std::ostream &os, const WorkerTask &task) {
    os << "WorkerTask<" << task.id << ", " << task.status << ", " << task.type << ">";
    return os;
}
#endif

struct UpdateQueueRequest {
    enum class Type {
        kUnknown,
        kCreate, // -> новый id задачи
        kNew,    // id -> статус New
        kGet,    // -> id задачи и статус Work
        kCheck,  // id задачи -> статус. Если статус Done/Error - удалить из очереди
        kDone,   // перевести id задачи в статус Done
        kError,  // перевести id задачи в статус Error
        kDelete  // удалить задачу из очереди
    };

    UpdateQueueRequest(Type type = Type::kUnknown, int task_id = TaggedId::kUnknown)
        : type(type), task_id(task_id) {}

#ifdef DEBUG
    friend std::ostream &operator<<(std::ostream &os, const Type &type) {
        os << "Type=";
        switch (type) {
          case Type::kCreate: { os << "Create"; break; }
          case Type::kNew: { os << "New"; break; }
          case Type::kGet: { os << "Get"; break; }
          case Type::kCheck: { os << "Check"; break; }
          case Type::kDone: { os << "Done"; break; }
          case Type::kError: { os << "Error"; break; }
          case Type::kDelete: { os << "Delete"; break; }
          case Type::kUnknown:
          default: { os << "Unknown"; break; }
        }
        return os;
    }

    friend std::ostream &operator<<(std::ostream &os, const UpdateQueueRequest &request) {
        os << "Request<" << request.type << ", task_id=" << request.task_id << ">";
        return os;
    }
#endif

    Type type;
    int task_id;
};

struct UpdateQueueResponse {
    UpdateQueueResponse(int id = TaggedId::kUnknown,
                        WorkerTask::Status task_status = WorkerTask::Status::kUnknown,
                        WorkerTask::Type task_type = WorkerTask::Type::kUnknown,
                        TaggedId::Tag tag = TaggedId::Tag::kTask) :
            id(id, tag), task_status(task_status), task_type(task_type) {}

#ifdef DEBUG
    friend std::ostream &operator<<(std::ostream &os, const UpdateQueueResponse &response) {
        os << "Response <" << response.id << ", task_status=" << response.task_status << ", task_type" << response.task_type << ">";
        return os;
    }
#endif

    TaggedId id;
    WorkerTask::Status task_status;
    WorkerTask::Type task_type;
};

struct Worker {
    Worker(const Pipe &to_worker_pipe, const Pipe &from_worker_pipe, std::string_view working_directory);
    ~Worker();

    ErrorStatus Run();
    static ErrorStatus CleanWorkingDirectory(std::string_view working_directory);

  private:
    struct WorkerImpl;
    std::unique_ptr<WorkerImpl> impl_;
};
