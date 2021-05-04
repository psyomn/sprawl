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
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <string.h>

extern "C" {
#include "pshy/shell.h"
#include "pshy/dynbuf.h"
#include "pshy/tokenizer.h"

#include <fcntl.h>
#include <string.h>
}

TEST(pshy, exit_command) {
  char *buff = strdup("exit");
  FILE* instream = fmemopen(buff, strnlen(buff, 10), "r");
  FILE* outstream = fopen("/dev/null", "w");

  pshy_shell(instream, outstream);

  fclose(instream);
  fclose(outstream);
  free(buff);
}

TEST(pshy, dynbuf) {
  struct pshy_buff *buff = pshy_buff_create();

  for (size_t i = 0; i < 10; ++i)
    pshy_buff_add(buff, 'a');

  const char* data = pshy_buff_data(buff);
  EXPECT_THAT(pshy_buff_len(buff), 10);
  EXPECT_TRUE(strncmp(data, "aaaaaaaaaa", 10) == 0);

  pshy_buff_free(buff);
}

TEST(pshy, tokenizer) {
  struct pshy_buff *buff = pshy_buff_create();

  for (size_t y = 0; y < 10; ++y) {
    for (size_t x = 0; x < 5; ++x)
      pshy_buff_add(buff, 'a');
    pshy_buff_add(buff, ' ');
  }

  struct pshy_tokens *toks = pshy_tokens_from_buff(buff);
  const size_t tok_len = pshy_tokens_len(toks);
  EXPECT_EQ(tok_len, 11);

  const char** raw_tokens = pshy_tokens_get(toks);
  EXPECT_TRUE(raw_tokens[10] == NULL);
  EXPECT_FALSE(raw_tokens[0] == NULL);
  EXPECT_TRUE(strncmp(raw_tokens[0], "aaaaa", 5) == 0);

  pshy_tokens_free(toks);
  pshy_buff_free(buff);
}
