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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct pshy_tokens {
  size_t len;
  char **tokens;
};

void add_item(struct pshy_tokens *toks, char *word) {
  ++toks->len;

  void *tmp = realloc(toks->tokens, sizeof(toks->tokens) * toks->len);
  if (tmp == NULL) {
    perror("realloc");
    abort();
  }

  toks->tokens = tmp;
  toks->tokens[toks->len-1] = word;
}

enum token_builtin pshy_tokens_builtin(const struct pshy_tokens* const toks) {
  if (toks->len == 0) return TOKEN_NOT_BUILTIN;
  if (strncmp(toks->tokens[0], "exit", 4) == 0) return TOKEN_BUILTIN_EXIT;
  if (strncmp(toks->tokens[0], "cd", 2) == 0) return TOKEN_BUILTIN_CD;
  return TOKEN_NOT_BUILTIN;
}

struct pshy_tokens* pshy_tokens_from_buff(const struct pshy_buff *const buff) {
  char *data = strdup(pshy_buff_data(buff));
  char *data_for_tok = data;

  struct pshy_tokens* toks = pshy_tokens_create();

  char *save = NULL;
  char *token = NULL;
  while ((token = strtok_r(data_for_tok, " ", &save)) != NULL) {
    add_item(toks, strdup(token));
    data_for_tok = NULL;
  }

  // null terminate string list
  add_item(toks, NULL);

  free(data);
  return toks;
}

struct pshy_tokens* pshy_tokens_create() {
  return calloc(1, sizeof(struct pshy_tokens));
}

void pshy_tokens_free(struct pshy_tokens *toks){
  for (size_t i = 0; i < toks->len; ++i)
    free(toks->tokens[i]);

  free(toks->tokens);
  free(toks);
}

char* const* pshy_tokens_get(const struct pshy_tokens* const toks) {
  return (char* const *) toks->tokens;
}

size_t pshy_tokens_len(const struct pshy_tokens* const toks) {
  return toks->len;
}
