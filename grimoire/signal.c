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
