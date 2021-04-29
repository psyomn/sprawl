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
#pragma once

#include <stdlib.h>

struct pshy_buff;

struct pshy_buff* pshy_buff_create(void);
void pshy_buff_free(struct pshy_buff*);
void pshy_buff_add(struct pshy_buff*, char c);
void pshy_buff_clear(struct pshy_buff*);
const char* pshy_buff_data(const struct pshy_buff *const);
size_t pshy_buff_len(const struct pshy_buff *const);
