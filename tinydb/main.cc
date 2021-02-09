#include <iostream>

#include "repl.h"

int main(int argc, char *argv[]) {
  (void) argc;

  psy::tinydb::REPL(argv[0]).Run();

  return 0;
}
