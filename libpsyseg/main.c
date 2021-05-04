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
#include <unistd.h>

#include "psyseg.h"

// declarations

struct session {
  char *filename;
  char *sig_filename;
  char *tags;
};

void session_free(struct session*);
void usage();

// program

void session_free(struct session* sess) {
  if (sess->filename) free(sess->filename);
  if (sess->sig_filename) free(sess->sig_filename);
  if (sess->tags) free(sess->tags);
}

void usage() {
  printf("psyseg -s <sigfile> -t <tags,...> -f <file>\n");
}

int main(int argc, char *argv[]) {
  struct session sess = {0};
  int opt = 0;

  while((opt = getopt(argc, argv, "t:s:f:")) != -1) {
    switch (opt) {
    case 't':
      sess.tags = strdup(optarg);
      break;
    case 's':
      sess.sig_filename = strdup(optarg);
      break;
    case 'f':
      sess.filename = strdup(optarg);
      break;
    default:
      usage();
      exit(EXIT_FAILURE);
    }
  }

  if (optind == 1) {
    usage();
    exit(EXIT_FAILURE);
  }

  if (!(sess.filename && sess.sig_filename && sess.tags)) {
    printf("you need to specify all three configurations\n");
    usage();
    exit(EXIT_FAILURE);
  }

  psyseg_mark(sess.filename, sess.sig_filename, sess.tags);

  session_free(&sess);
  return EXIT_SUCCESS;
}
