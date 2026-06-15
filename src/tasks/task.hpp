#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
enum class TaskStatus { ACTIVE, COMPLETE };

NLOHMANN_JSON_SERIALIZE_ENUM(TaskStatus, {{TaskStatus::ACTIVE, "ACTIVE"},
                                          {TaskStatus::COMPLETE, "COMPLETE"}})

class Task {
private:
  int id;
  std::string title, description;
  TaskStatus status = TaskStatus::ACTIVE;

public:
  Task(int id, std::string title, std::string description);
  Task(int id, std::string title);
  Task() = default;

  bool mark_as_complete();

  friend std::ostream &operator<<(std::ostream &os, const Task &t) {
    std::string status_string =
        (t.status == TaskStatus::COMPLETE) ? "COMPLETE" : "ACTIVE";

    std::cout << "Task " << t.id << ":\n"
              << "\ttitle:\t" << t.title << "\n\tdescription:\t"
              << t.description << "\n\tstatus:\t" << status_string << "\n";

    return os;
  }

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Task, id, title, description, status)
};
