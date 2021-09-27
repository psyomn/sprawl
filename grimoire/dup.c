#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {
  const char* str = "hello how are you today";
  char *dupstr = strndup(str, sizeof(*str));
  FILE* strfd = fmemopen(dupstr, sizeof(*str), "r");

  char c = 0;
  size_t read_bytes = 0;

  do {
    read_bytes = fread(&c, sizeof(c), 1, strfd);
    printf("%c.", c);
  } while (read_bytes != 0);

  printf("\n");

  fclose(strfd);
  free(dupstr);

  return 0;
}
