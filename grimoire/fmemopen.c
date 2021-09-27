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
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#define HANDLE_ERROR(msg) do { perror(msg); return EXIT_FAILURE; } while(0);

int main(void) {
  printf("simple usage of fmemopen\n");

  uint8_t contents[] = {
    0xca, 0xfe, 0xd0, 0x0d,
    0x01, 0x02, 0x03, 0x04,
  };

  FILE* fd = fmemopen(&contents[0],
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
