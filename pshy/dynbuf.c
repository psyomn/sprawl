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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct pshy_buff {
  size_t len;
  size_t cap;
  char *data;
};

struct pshy_buff* pshy_buff_create(void) {
  struct pshy_buff* buff = calloc(1, sizeof(struct pshy_buff));
  buff->cap = 64;
  buff->data = calloc(buff->cap, sizeof(buff->data[0]));
  return buff;
}

void pshy_buff_free(struct pshy_buff *buff) {
  if (buff->len > 0) free(buff->data);
  free(buff);
}

void pshy_buff_add(struct pshy_buff *buff, char c) {
  if (buff->len + 1 >= buff->cap) {
    buff->cap <<= 1;
    buff->data = realloc(buff->data, buff->cap);

    if (buff->data == NULL) {
      perror("realloc");
      abort();
    }

    memset(buff->data + buff->len + 1,
           0,
           buff->cap - buff->len - 1);
  }

  buff->data[buff->len] = c;
  buff->len += 1;
}

void pshy_buff_clear(struct pshy_buff *buff) {
  memset(buff->data, 0, buff->cap);
  buff->len = 0;
}

const char* pshy_buff_data(const struct pshy_buff *const buff) {
  return buff->data;
}

size_t pshy_buff_len(const struct pshy_buff *const buff) {
  return buff->len;
}
