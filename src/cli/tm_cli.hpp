#pragma once

#include "../tasks/task.hpp"
#include <array>
#include <string>
#include <utility>
#include <vector>

enum class Command { Add, List, Edit, Delete, Help };

class TaskManagerCli {
private:
  std::vector<Task> tasks;
  std::vector<std::string> args, flags;

  static constexpr std::array<std::pair<std::string_view, Command>, 5> c_map = {
      {{"add", Command::Add},
       {"list", Command::List},
       {"edit", Command::Edit},
       {"delete", Command::Delete},
       {"help", Command::Help}}};

  void new_task() {}
  void list_tasks() {}
  void help() {}
  void delete_task() {}
  void edit_task() {}
  void mark_task_as_complete() {}

public:
  TaskManagerCli(int argc, char *argv[]);
  ~TaskManagerCli() {}

  void run() {}
};
