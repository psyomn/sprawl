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
#include "dynbuf.h"
#include "shell.h"
#include "utils.h"
#include "tokenizer.h"

#include <stdio.h>
#include <unistd.h>

#include <sys/wait.h>

void pshy_shell() {
  bool is_done = false;
  while (!is_done) {
    struct pshy_buff *buff = pshy_buff_create();

    printf("ψ ");

    char rc = 0;
    while ((rc = getchar())) {
      if (rc == 0) break;
      if (rc == EOF) break;
      if (pshy_is_delimiter(rc)) break;

      pshy_buff_add(buff, rc);
    }

    if (pshy_buff_len(buff) == 0) goto cleanup_buff;

    struct pshy_tokens *toks = pshy_tokens_from_buff(buff);
    const char **tokens = pshy_tokens_get(toks);

    switch (pshy_tokens_builtin(toks)) {
    case TOKEN_BUILTIN_EXIT:
      is_done = true;
      goto cleanup_tokens;
    case TOKEN_BUILTIN_CD:
      if (pshy_tokens_len(toks) < 2) {
        printf("usage: \n"
               "  cd <path>\n");
        goto cleanup_tokens;
      }

      if (chdir(tokens[1])) perror("chdir");

      goto cleanup_tokens;

    case TOKEN_NOT_BUILTIN:
    default:
      break;
    }

    // fork stuff here

    pid_t pid = fork();

    if (pid == 0) {
      // child
      if (execvp(tokens[0], tokens) != 0) perror("execvp");
    }

    if (pid > 0) {
      // parent
      int wstatus = 0;
      do {
        waitpid(pid, &wstatus, WUNTRACED | WCONTINUED);
      } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
    }

    if (pid < 0) {
      // error
      perror("fork");
    }

  cleanup_tokens:
    pshy_tokens_free(toks);
  cleanup_buff:
    pshy_buff_free(buff);
  }
}
