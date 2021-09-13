/*
   Copyright 2019-2021 Simon (psyomn) Symeonidis

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */
#include "lexer.h"

#include <getopt.h>

#include <fstream>
#include <optional>

/* declarations */
void usage(const char *name);

/* rest */
void usage(const char *name) {
  std::cout
    << "usage: " << std::endl
    << std::string(name) << " [-f filename] [-h]" << std::endl;
}

struct session {
  std::optional<std::string> filename_;
};

int main(int argc, char *argv[]) {
  session sess;

  int opt = 0;
  while ((opt = getopt(argc, argv, "hf:d:")) != -1) {
    switch (opt) {
    case 'f':
      sess.filename_ = optarg;
      break;
    case 'h':
    default:
      usage(argv[0]);
    }
  }

  if (sess.filename_) {
    std::ifstream ifs(sess.filename_.value());
    psy::bk::Lexer lex(ifs);
    lex.Process();
    return 0;
  }

  return 0;
}
