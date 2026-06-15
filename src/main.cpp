#include "nlohmann/json_fwd.hpp"
#include "tasks/task.hpp"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <optional>
#include <ostream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

int add_new_task(std::vector<Task> &vec, std::vector<std::string> &args);
void help();
void list_tasks(const std::vector<Task> &vec,
                const std::vector<std::string> &args);

enum class Command { Add, List, Edit, Delete, Help };

static const std::unordered_map<std::string, Command> commands_map = {
    {"add", Command::Add},
    {"list", Command::List},
    {"edit", Command::Edit},
    {"delete", Command::Delete},
    {"help", Command::Help}};

int main(int argc, char *argv[]) {
  std::vector<Task> tasks;
  nlohmann::json input_json;

  std::ifstream ifile("tasks.json");

  if (ifile.is_open()) {

    ifile >> input_json;
    ifile.close();

    tasks = input_json.get<std::vector<Task>>();
  }

  if (argc < 2) {
    std::cout << "TaskManager [COMMAND] || TaskManager help" << std::endl;

    return 1;
  }

  std::vector<std::string> args(argv + 1, argv + argc);

  std::string command(args.at(0));

  std::transform(command.begin(), command.end(), command.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  auto it = commands_map.find(command);

  if (it == commands_map.end()) {
    std::cout << "The command " << command << " doesn't exists\n"
              << "TaskManager help" << std::endl;

    return 1;
  }

  Command matched_command = it->second;

  int result;

  switch (matched_command) {
  case Command::Add:
    result = add_new_task(tasks, args);

    if (result == 1) {
      return result;
    }

    break;
  case Command::List:
    list_tasks(tasks, args);
    break;
  case Command::Delete:
    std::cout << "Delete a tasks";
    break;
  case Command::Edit:
    std::cout << "Edit a task";
    break;
  case Command::Help:
    help();
    break;
  }

  nlohmann::json j = tasks;

  // Save the info
  std::ofstream file("tasks.json");

  file << j.dump(4);
  file.close();

  return 0;
}

int add_new_task(std::vector<Task> &vec, std::vector<std::string> &args) {
  if (args.size() == 1) {
    std::cout
        << "TaskManager add [TITLE] || TaskManager add [TITLE] -d [DESCRIPTION]"
        << std::endl;

    return 1;
  }

  int args_size = args.size();

  std::string title = args.at(1);
  std::optional<std::string> descritption;

  if (args_size > 2) {
    if (args_size == 3) {
      std::cout << "Description may not be empty or null\n"
                << "If you don't want to add a description please try:\n"
                << "TaskManager add [TITLE]" << std::endl;

      return 1;
    }

    std::string flag = args.at(2);

    if (flag != "-d") {
      std::cout << "The " << flag << " is not a valid flag\n"
                << "You wanted to say: \"-d\"?" << std::endl;

      return 1;
    }

    descritption = args.at(3);
  }

  int next_id = vec.size() + 1;

  if (!descritption.has_value()) {
    vec.emplace_back(next_id, title);
  } else {
    vec.emplace_back(next_id, title, descritption.value());
  }

  return 0;
}

void list_tasks(const std::vector<Task> &vec,
                const std::vector<std::string> &args) {
  if (args.size() > 2) {
    std::cout << "This command takes 1 flag per usage" << std::endl;

    return;
  }

  std::string flag = args.at(1);

  if (flag != "-a" || flag != "-c") {
    std::cout << "The flag " << flag << "isn't a valid flag\n"
              << "Please use -a or -c" << std::endl;

    return;
  }

  if (vec.empty()) {
    std::cout << "There's no tasks created" << std::endl;

    return;
  }

  if (flag == "-a") {
  }

  for (int i = 0; i < vec.size(); i++) {
    Task c_task = vec.at(i);

    std::cout << c_task;
  }
}

void help() {
  std::cout << "Hur's TaskManager:\n"
            << "\thelp\n"
            << "\tadd:\n"
            << "\t\tARGS: [TITLE]\n"
            << "\t\tFLAGS: -d [DESCRIPTION]\n"
            << "\tlist:\n"
            << "\t\tFLAGS: -a (active) -c (complete)\n"
            << "\tedit:\n"
            << "\t\tARGS: [TASK_ID]\n"
            << "\t\tFLAGS: -t (title) -d (description)\n"
            << "\tdelete:\n"
            << "\t\tARGS: [TASK_ID]" << std::endl;
}
