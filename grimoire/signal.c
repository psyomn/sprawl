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
#include <signal.h>
#include <unistd.h>

void handle_term(int param) {
  fprintf(stdout, "received term: %d\n", param);
}

void handle_sigusr1(int param) {
  fprintf(stdout, "special beefcake exit: %d\n", param);
  exit(EXIT_FAILURE);
}

int main(void) {
  signal(SIGTERM, &handle_term);
  signal(SIGUSR1, &handle_sigusr1);

  printf("sleeping... try sending this process SIGKILL, SIGTERM\n");
  printf("send SIGUSR1 to terminate\n");

  while (1) {
    sleep(3);
    printf("... ");
    fflush(stdout);
  }

  return EXIT_SUCCESS;
}
