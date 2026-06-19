#pragma once

#include "../tasks/task.hpp"
#include <array>
#include <string>
#include <utility>
#include <vector>

enum class Command { Add, List, Edit, Delete, Help };

const std::string TASKS_FILE = "tasks.json";

class TaskManagerCli {
private:
  int args_size;

  std::vector<Task> tasks;
  std::vector<std::string> args;

  static constexpr std::array<std::pair<std::string_view, Command>, 5> c_map = {
      {{"add", Command::Add},
       {"list", Command::List},
       {"edit", Command::Edit},
       {"delete", Command::Delete},
       {"help", Command::Help}}};

  int new_task();
  int list_tasks();
  int help();
  int delete_task();
  int edit_task() {}
  int mark_task_as_complete() {}

  void load_tasks();
  void save_tasks();

public:
  TaskManagerCli(int argc, char *argv[]);

  int run();
};
