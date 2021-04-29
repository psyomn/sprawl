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
#include "tokenizer.h"
#include "string.h"

#include <stdlib.h>

struct pshy_tokens {
  size_t len;
  char **tokens;
};

void add_item(struct pshy_tokens *toks, char *word) {
  toks->len += 1;
  toks->tokens = realloc(toks->tokens, sizeof(toks->tokens[0]) * toks->len);
  toks->tokens[toks->len-1] = word;
}

struct pshy_tokens* pshy_tokens_from_buff(const struct pshy_buff *const buff) {
  char *data = strdup(pshy_buff_data(buff));
  char *data_for_tok = data;

  char *save1 = NULL;
  char *token = NULL;

  struct pshy_tokens* toks = pshy_tokens_create();

  while ((token = strtok_r(data_for_tok, " ", &save1)) != NULL) {
    add_item(toks, strdup(token));

    // strtok_r requires NULL for every subsequent call
    data_for_tok = NULL;
  }

  free(data);

  // null terminate
  add_item(toks, NULL);

  return toks;
}

struct pshy_tokens* pshy_tokens_create() {
  return calloc(1, sizeof(struct pshy_tokens));
}

void pshy_tokens_free(struct pshy_tokens *toks){
  for (size_t i = 0; i < toks->len; ++i) {
    free(toks->tokens[i]);
  }

  free(toks);
}

const char** pshy_tokens_get(const struct pshy_tokens* const toks) {
  return toks->tokens;
}

size_t pshy_tokens_len(const struct pshy_tokens* const toks) {
  return toks->len;
}
