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
#include <errno.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* sys */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>

/* graphics */
#include <SDL2/SDL.h>

/* lib */
#include "graphics.h"

struct session {
  char host[512];
  int port;

  int server_mode;
};

void print_usage(const char *name) {
  printf(
      "usage:\n"
      "%s\n"
      "  -s        servermode\n"
      "  -h <host> host to listen/connect to\n"
      "  -p <port> port to listen/connect to\n",
      name
  );
}

void parse_int_or_die(int *value, const char *arg) {
  char *endptr = NULL;

  errno = 0;
  int tmp = strtol(arg, &endptr, 0);

  if (errno != 0) {
    perror("strtol");
    exit(EXIT_FAILURE);
  }

  if (endptr == arg) {
    fprintf(stderr, "expected integer, got: %s\n", arg);
    exit(EXIT_FAILURE);
  }

  *value = tmp;
}

void render_loop(struct session *sess) {
  printf("creating window...\n");

  struct server {
    int port;
    int fd;
    uint8_t buffer[512];
  } srv = {
    .port = sess->port,
    .fd = socket(AF_INET, SOCK_DGRAM, 0),
    .buffer = {0},
  };

  struct packetdraw_win win = {0};
  if (packetdraw_graphics_init(&win) == -1) {
    fprintf(stderr, "problem initializing graphics\n");
    goto cleanup;
  }

  packetdraw_graphics_render(&win);

  if (packetdraw_graphics_free(&win) == -1) {
    fprintf(stderr, "problem deallocing SDL resources\n");
    goto cleanup;
  }

cleanup:
  close(srv.fd);
}

int main(int argc, char *argv[]) {
  struct session sess = {0};

  int opt = 0;
  while ((opt = getopt(argc, argv, "h:p:s")) != -1) {
    switch (opt) {
    case 'h':
      strncpy(sess.host, optarg, sizeof(sess.host) - 1);
      break;
    case 'p':
      parse_int_or_die(&sess.port, optarg);
      break;
    case 's':
      sess.server_mode = 1;
      break;
    case '?':
    default:
      print_usage(argv[0]);
    }
  }

  render_loop(&sess);

  return 0;
}
