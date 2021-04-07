#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#define HANDLE_ERROR(msg) do { perror(msg); return EXIT_FAILURE; } while(0);

int main(void) {
  printf("simple usage of fmemopen\n");

  const uint8_t contents[] = {
    0xca, 0xfe, 0xd0, 0x0d,
    0x01, 0x02, 0x03, 0x04,
  };

  FILE* fd = fmemopen((void*)contents,
                      ARRAY_SIZE(contents),
                      "rb");

  if (fd == NULL) HANDLE_ERROR("problem opening buffer as stream\n");

  uint8_t buff[ARRAY_SIZE(contents)] = {0};

  if (!fread(buff, sizeof(buff), ARRAY_SIZE(contents), fd))
    HANDLE_ERROR("problem reading into buffer");

  fclose(fd);

  for (size_t i = 0; i < ARRAY_SIZE(contents); ++i)
    printf("%02x.", buff[i]);

  printf("\n");

  return 0;
}
