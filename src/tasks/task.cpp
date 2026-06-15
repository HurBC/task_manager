#include "task.hpp"

Task::Task(int id, std::string title, std::string description) {
  this->id = id;
  this->title = title;
  this->description = description;
}

Task::Task(int id, std::string title) {
  this->title = title;
  this->id = id;
}

bool Task::mark_as_complete() {
  TaskStatus status = this->status;

  if (status == TaskStatus::COMPLETE)
    return false;

  this->status = TaskStatus::COMPLETE;

  return true;
}
