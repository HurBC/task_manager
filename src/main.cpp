#include "cli/tm_cli.hpp"

int main(int argc, char *argv[]) {
  TaskManagerCli cli(argc, argv);

  return cli.run();
}
