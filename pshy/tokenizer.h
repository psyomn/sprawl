#pragma once
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
#include <stddef.h>

#include "dynbuf.h"

struct pshy_tokens;

enum token_builtin {
  TOKEN_NOT_BUILTIN,
  TOKEN_BUILTIN_EXIT,
  TOKEN_BUILTIN_CD,
};

struct pshy_tokens* pshy_tokens_from_buff(const struct pshy_buff* const);
struct pshy_tokens* pshy_tokens_create();
enum token_builtin pshy_tokens_builtin(const struct pshy_tokens* const);
void pshy_tokens_free(struct pshy_tokens*);
char * const* pshy_tokens_get(const struct pshy_tokens* const);
size_t pshy_tokens_len(const struct pshy_tokens* const);
