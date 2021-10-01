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

/* graphics */
#include <SDL2/SDL.h>

/* lib */
#include "graphics.h"
#include "message.h"
#include "server.h"
#include "client.h"

struct session {
  char host[512];
  int port;

  int server_mode;
  int client_mode;

  struct client cl;
};

void print_usage(const char *name) {
  printf(
      "usage:\n"
      "%s\n"
      "  -s        servermode\n"
      "  -h <host> host to listen/connect to\n"
      "  -p <port> port to listen/connect to\n"
      "\n"
      "  -c                   clientmode (use with below options)\n"
      "  -t <pixel|line|rect> shape type\n"
      "  -x <pos>             x coordinate (int)\n"
      "  -y <pos>             y coordinate (int)\n"
      "  -w <pos>             width (int)\n"
      "  -k <pos>             height (int)\n"
      "  -r <0-255>           red color value\n"
      "  -g <0-255>           green color value\n"
      "  -b <0-255>           blue color value\n"
      ,
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

void parse_uint32_or_die(uint32_t *value, const char *arg) {
  char *endptr = NULL;

  errno = 0;
  int tmp = strtoul(arg, &endptr, 0);

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
  enum packetdraw_server_status status;
  struct packetdraw_server *srv = packetdraw_server_create(sess->port, &status);
  if (status != PACKETDRAW_SERVER_SUCCESS) {
    fprintf(stderr, "problem starting up server: %d\n", status);
    abort();
  }

  struct packetdraw_win win = {0};

  if (packetdraw_graphics_init(&win) == -1) {
    fprintf(stderr, "problem initializing graphics\n");
    goto cleanup;
  }

  packetdraw_server_async_listen(srv);

  packetdraw_graphics_render(&win, srv);

  if (packetdraw_graphics_free(&win) == -1) {
    fprintf(stderr, "problem deallocing SDL resources\n");
    goto cleanup;
  }

cleanup:
  packetdraw_server_free(srv);
}

int main(int argc, char *argv[]) {
  struct session sess = {0};

  int opt = 0;
  while ((opt = getopt(argc, argv, "h:p:sct:x:y:w:k:r:g:b:")) != -1) {
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
    case 'c':
      sess.client_mode = 1;
      break;
    case 't':
      if (strcmp(optarg, "pixel") == 0)
        sess.cl.type = PACKETDRAW_MSGTYPE_PIXEL;
      else if (strcmp(optarg, "line") == 0)
        sess.cl.type = PACKETDRAW_MSGTYPE_LINE;
      else if (strcmp(optarg, "rect") == 0)
        sess.cl.type = PACKETDRAW_MSGTYPE_RECTANGLE;
      else {
        printf("WWWUT\n");
        sess.cl.type = PACKETDRAW_MSGTYPE_UNKNOWN;
      }
      break;
    case 'x':
      parse_uint32_or_die(&sess.cl.x, optarg);
      break;
    case 'y':
      parse_uint32_or_die(&sess.cl.y, optarg);
      break;
    case 'w':
      parse_uint32_or_die(&sess.cl.w, optarg);
      break;
    case 'k':
      parse_uint32_or_die(&sess.cl.h, optarg);
      break;
    case 'r':
      {
        int r = 0;
        parse_int_or_die(&r, optarg);
        sess.cl.r = 0xff & r;
      }
      break;
    case 'g':
      {
        int g = 0;
        parse_int_or_die(&g, optarg);
        sess.cl.g = 0xff & g;
      }
      break;
    case 'b':
      {
        int b = 0;
        parse_int_or_die(&b, optarg);
        sess.cl.b = 0xff & b;
      }
      break;
    case '?':
    default:
      print_usage(argv[0]);
    }
  }

  if (sess.server_mode) {
    render_loop(&sess);
  } else if (sess.client_mode) {
    sess.cl.port = sess.port;
    strncpy(sess.cl.host, sess.host, sizeof(sess.cl.host) - 1);
    packetdraw_client_send_message(&sess.cl);
  }

  return 0;
}
