/*
   Copyright 2021 Simon (psyomn) Symeonidis

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
// TODO madvise usage might be interesting to take a look at

#include <iostream>
#include <cstdint>

// C
#include <unistd.h>

// system
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  int opt = 0;

  struct session {
    std::string filename_;
  } sess;

  while ((opt = getopt(argc, argv, "f:")) != -1) {
    switch (opt) {
    case 'f':
      sess.filename_ = optarg;
      break;
    default:
      std::cout << "usage: " << std::endl;
      std::cout << "  " << argv[0] << " -f <filename> " << std::endl;
      return 0;
    }
  }

  if (sess.filename_.empty()) {
    std::cerr << "must provide -f <filename>" << std::endl;
    return -1;
  }

  struct stat st = {0};
  int fd = open(sess.filename_.c_str(), O_RDWR, S_IRUSR | S_IWUSR);

  if (fd == -1) {
    perror("file");
    return -1;
  }

  if (fstat(fd, &st) == -1) {
    perror("fstat");
    return -1;
  }

  const size_t file_size = static_cast<size_t>(st.st_size);
  uint8_t *curr = NULL;

  curr = reinterpret_cast<uint8_t*>
    (mmap(NULL,
          file_size,
          PROT_READ | PROT_WRITE,
          MAP_PRIVATE,
          fd,
          0));


  std::cout << "file size: " << file_size << std::endl;
  for (size_t i = 0; i < file_size; ++i)
    std::cout << std::hex << int(curr[i]) << std::dec << ", ";

  std::cout << std::endl;

  if (munmap(curr, file_size) == -1) perror("munmap");

  close(fd);
  return 0;
}
