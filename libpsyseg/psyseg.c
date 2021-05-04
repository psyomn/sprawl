/*
   Copyright 2021 Simon (pshyomn) Symeonidis

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
#include "psyseg.h"
#include "common.h"

struct psyseg_file {
  // the file size of the file we're going to encapsulate
  size_t file_size;

  // the calculated final size with the appended header
  size_t final_size;

  // the size of the signature we'll append to the headers
  size_t sig_size;
  const char* sig;

  // the size of the tags
  size_t tag_size;
  const char* tag;

  // file descriptor for file
  int fd;

  // mmap cursor
  uint8_t *cursor;
};

void set_file_size(const char *filename, struct psyseg_file *pf) {
  struct stat st = {0};
  if (stat(filename, &st) == -1) {
    perror("stat");
    abort();
  }

  pf->file_size = st.st_size;

  pf->final_size =
    pf->file_size +
    sizeof(pf->sig_size) + pf->sig_size +
    sizeof(pf->tag_size) + pf->tag_size;
}

void set_mmap(const char *filename, struct psyseg_file *pf) {
  if((pf->fd = open(filename, O_RDONLY)) == -1) {
    perror("open");
    abort();
  }

  void *mret = mmap(NULL, pf->final_size, PROT_READ, MAP_PRIVATE, pf->fd, 0);

  if (mret == MAP_FAILED) {
    perror("mmap");
    abort();
  }

  pf->cursor = (uint8_t *)mret;
}

void create_and_stamp(struct psyseg_file *pf) {
  int fd = 0;
  fd = creat("out.bin", 0744);
  ftruncate(fd, pf->final_size);

  write(fd, &pf->sig_size, sizeof(pf->sig_size));
  write(fd, pf->sig, pf->sig_size);

  write(fd, &pf->tag_size, sizeof(pf->tag_size));
  write(fd, pf->tag, pf->tag_size);

  write(fd, pf->cursor, pf->file_size);

  close(fd);


void cleanup(struct psyseg_file *pf) {
  munmap(pf->cursor, pf->final_size);
}

int psyseg_mark(const char *filename, const char* sigfile, const char *tags) {
  struct psyseg_file sfile = {0};
  sfile.sig_size = strlen(sigfile);
  sfile.sig = sigfile;
  sfile.tag_size = strlen(tags);
  sfile.tag = tags;

  set_file_size(filename, &sfile);
  set_mmap(filename, &sfile);
  create_and_stamp(&sfile);

  printf("process file : %s\n", filename);
  printf("process sig  : %s\n", sigfile);
  printf("process tags : %s\n", tags);
  printf("size in bytes: %zu\n", sfile.file_size);

  cleanup(&sfile);
  return 0;
}
