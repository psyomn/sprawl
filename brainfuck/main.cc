#include <iostream>

#include "brainfuck.h"

void usage();

void usage() {
  std::cout << "bf <program.bf>" << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    usage();
    return 0;
  }

  auto path = std::filesystem::path(argv[1]);
  bfi::Brainfuck bf(std::cout);

  bf.FromFile(path);
  bf.Run();

  return bf.GetExitCode();
}
