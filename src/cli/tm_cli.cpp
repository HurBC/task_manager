#include "tm_cli.hpp"
#include "nlohmann/json_fwd.hpp"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

TaskManagerCli::TaskManagerCli(int argc, char *argv[]) {
  this->args = std::vector<std::string>(argv + 1, argv + argc);
  this->args_size = this->args.size();
}

int TaskManagerCli::run() {
  if (this->args.size() < 1) {
    std::cout << "TaskManager [COMMAND] || TaskManager help" << std::endl;

    return 1;
  }

  std::string command = args.at(0);

  std::transform(command.begin(), command.end(), command.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  std::optional<Command> matched_command;

  for (std::pair<std::string_view, Command> current_pair :
       TaskManagerCli::c_map) {
    if (command == current_pair.first)
      matched_command = current_pair.second;
  }

  if (!matched_command.has_value()) {
    std::cout << "The command " << command << " doesn't exists\n"
              << "TaskManager help" << std::endl;

    return 1;
  }

  this->load_tasks();

  switch (matched_command.value()) {
  case Command::Add:
    return this->new_task();
  case Command::List:
    return this->list_tasks();
  case Command::Edit:
  case Command::Delete:
    return this->delete_task();
  case Command::Help:
    return this->help();
  }

  return 0;
}

int TaskManagerCli::new_task() {
  if (this->args_size == 1) {
    std::cout
        << "TaskManager add [TITLE] || TaskManager add [TITLE] -d [DESCRIPTION]"
        << std::endl;

    return 1;
  }

  int next_id;

  std::string title = this->args.at(1);
  std::optional<std::string> descritption;

  if (this->args_size > 2) {
    if (this->args_size == 3) {
      std::cout << "Description may not be empty or null\n"
                << "If you don't want to add a description please try:\n"
                << "TaskManager add [TITLE]" << std::endl;

      return 1;
    }
    std::string flag = this->args.at(2);

    if (flag != "-d") {
      std::cout << "The " << flag << " is not a valid flag\n"
                << "You wanted to say: \"-d\"?" << std::endl;

      return 1;
    }

    descritption = this->args.at(3);
  }

  next_id = this->tasks.size() + 1;

  if (!descritption.has_value()) {
    this->tasks.emplace_back(next_id, title);
  } else {

    this->tasks.emplace_back(next_id, title, descritption.value());
  }

  return 0;
}

int TaskManagerCli::list_tasks() {
  if (this->args_size > 2) {
    std::cout << "This command takes 1 flag per usage" << std::endl;

    return 1;
  }

  int filter_type = -1;
  std::optional<std::string> flag;
  std::vector<Task> tasks;

  if (this->args_size == 2) {
    flag = this->args.at(1);

    if (flag != "-a" && flag != "-c") {
      std::cout << "The flag " << flag.value() << " isn't a valid flag\n"
                << "Please use -a or -c" << std::endl;

      return 1;
    }
  }

  if (flag.has_value()) {
    filter_type = (flag.value() == "-a") ? 0 : 1;
  }

  if (filter_type == -1)
    tasks = this->tasks;
  else
    std::copy_if(this->tasks.begin(), this->tasks.end(),
                 std::back_inserter(tasks), [filter_type](Task t) {
                   bool is_complete = t.is_complete();
                   return (filter_type == 1) ? is_complete : !is_complete;
                 });

  if (tasks.empty()) {
    std::cout << "There's no tasks ";

    if (filter_type != -1) {
      std::string status = (filter_type == 1) ? "complete" : "active";

      std::cout << "in " << status << " ";
    }

    std::cout << "created";
  }

  for (int i = 0; i < tasks.size(); i++) {
    Task c_task = tasks.at(i);

    std::cout << c_task;
  }

  return 0;
}

int TaskManagerCli::help() {
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

  return 0;
}

int TaskManagerCli::delete_task() {
  int task_id;

  if (this->args_size == 1) {
    std::cout << "TaskManager delete <TASK_ID> || TaskManager delete <TASK_ID> "
                 "<TASK_ID2> <TASK_ID3>..."
              << std::endl;

    return 1;
  }

  for (int i = 1; i < this->args_size; i++) {
    std::cout << this->args[i] << "\n";
  }

  return 0;
}

void TaskManagerCli::load_tasks() {
  nlohmann::json json;

  std::ifstream input_file(TASKS_FILE);

  if (!input_file.is_open())
    return;

  input_file >> json;
  input_file.close();

  this->tasks = json.get<std::vector<Task>>();
}

void TaskManagerCli::save_tasks() {
  // Save the info always when the class is destroyed

  nlohmann::json j = this->tasks;

  std::ofstream output_file(TASKS_FILE);

  output_file << j.dump(4);
  output_file.close();
}
