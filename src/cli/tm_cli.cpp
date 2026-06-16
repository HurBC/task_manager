#include "tm_cli.hpp"
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

TaskManagerCli::TaskManagerCli(int argc, char *argv[]) {
  this->args = std::vector<std::string>(argv + 1, argv + argc);

  for (std::string &arg : this->args) {
    std::transform(arg.begin(), arg.end(), arg.begin(),
                   [](unsigned char c) { return std::tolower(c); });
  }
}
