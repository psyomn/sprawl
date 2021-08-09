#include <iostream>
#include <cstdint>
#include <locale>
#include <fstream>

#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "tvm.h"

struct Session {
  std::string filename_;

  void CompileFile() {
    std::ifstream ifile;
    ifile.open(filename_,  std::ifstream::in);
    if (!ifile.is_open()) {
      std::cerr << "error opening file: " << filename_ << std::endl;
      exit(1);
    }

    auto tokens = psy::tvm::Tokenize(ifile);
    ifile.close();

    auto inset = psy::tvm::Parse(tokens);
    auto image = psy::tvm::Encode(inset);

    const auto output_name = filename_ + ".bin";
    std::ofstream ofile;
    ofile.open(output_name, std::ofstream::binary | std::ofstream::out);
    ofile.imbue(std::locale::classic());
    if (!ofile.is_open()) {
      std::cerr << "error opening file for output: " << output_name << std::endl;
      exit(1);
    }

    for (size_t i = 0; i < image.len_; ++i) {
      const std::uint16_t le = htons(image.data_[i]);
      ofile.write(reinterpret_cast<const char*>(&le),
                  sizeof(image.data_[0]));
    }

    ofile.close();
  }
};

void usage();
void usage() { std::cout << "-c <filename>" << std::endl; }

int main(int argc, char *argv[]) {
  Session sess;

  int opt = 0;
  while ((opt = getopt(argc, argv, "c:")) != -1) {
    switch (opt) {
    case 'c': sess.filename_ = optarg; break;
    case 'h':
    default:
      usage();
      return EXIT_FAILURE;
    }
  }

  if (sess.filename_ == "") {
    usage();
    exit(1);
  }

  sess.CompileFile();

  return 0;
}
