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
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>
#include <linux/limits.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define MAX_BUFFER 256

struct session {
  char path[PATH_MAX];
};

struct resource {
  sem_t sem;
  uint8_t data[MAX_BUFFER];
};

void usage(const char* name) {
  printf("usage: \n"
         "  %s [-p shmpath] [-i interactive] [-h help]\n", name);
}

void print_resource(const struct resource * const res) {
  printf("res:\n"
         "  data: %s", res->data);
}

int main(int argc, char* argv[]) {
  struct session sess = {0};
  int opt = 0;

  while ((opt = getopt(argc, argv, "p:h")) != -1) {
    switch (opt) {
    case 'p':
      strncat(sess.path, optarg, PATH_MAX - 1);
      break;
    case 'h':
      usage(argv[0]);
      return EXIT_SUCCESS;
    default:
      usage(argv[0]);
      return EXIT_FAILURE;
    }
  }

  // actual shm stuff
  printf("shm path: %s\n", sess.path);

  int fd = shm_open(sess.path,
                    O_CREAT | /* O_EXCL | */ O_RDWR,
                    S_IRUSR | S_IWUSR);

  if (fd == -1) {
    perror("shm_open");
    return EXIT_FAILURE;
  }

  if (ftruncate(fd, sizeof(struct resource)) == -1) {
    perror("ftruncate");
    return EXIT_FAILURE;
  }

  struct resource *res = mmap(NULL,
                              sizeof(struct resource),
                              PROT_READ | PROT_WRITE,
                              MAP_SHARED, fd, 0);

  if (sem_init(&res->sem, 1 /* shm special config*/, 1) == -1) {
    perror("semaphore");
    return EXIT_FAILURE;
  }

  if (res == MAP_FAILED) {
    perror("mmap");
    return EXIT_FAILURE;
  }

  while (strncmp("end", (char *)res->data, sizeof(res->data)) != 0) {
    if (sem_wait(&res->sem) == -1) {
      perror("sem_wait");
      continue;
    }

    fgets((char *)res->data, sizeof(res->data) - 1, stdin);
    const size_t num_read = strnlen((char*)res->data, sizeof(res->data));
    bzero(res->data + num_read, sizeof(res->data) - num_read);

    if (sem_post(&res->sem) == -1) {
      perror("sem_post");
      continue;
    }

    print_resource(res);
  }

  shm_unlink(sess.path);
  return EXIT_SUCCESS;
}
