/*
   socktk.c is a simple collection of socket tools to emulate nasty behavior.

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
#include <getopt.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* system */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

struct session {
  char host[255];
  int port;

  int exec_peer_reset;
};

int session_print_info(const struct session *sess) {
  return
    printf(
        "---- session info --------\n"
        "  host           : %s\n"
        "  port           : %d\n"
        "  exec_peer_reset: %d\n"
        "--------------------------\n",
        sess->host,
        sess->port,
        sess->exec_peer_reset
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

int print_usage(const char *name) {
  return
    printf(
      "usage: \n"
      "  %s\n"
      "required configs:\n"
      "  --host <hostname string>\n"
      "  --port <port number>\n"
      "scenarios:\n"
      "  --reset-peer (emulate tcp reset by peer)\n",
      name
  );
}

/**
 * do_exec_reset_peer - this will emulate a tcp premature reset on the client
 * side.
 */
int do_exec_reset_peer(const struct session *sess) {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1) {
    perror("socket/tcp");
    abort();
  }

  struct linger ling = {
    .l_linger = 0,
    .l_onoff = 1,
  };

  if (setsockopt(fd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling)) == -1) {
    perror("setsockopt");
    printf("%d\n", errno);
    abort();
  }

  struct sockaddr_in addr = {0};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(sess->host);
  addr.sin_port = htons(sess->port);

  if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("connect");
    abort();
  }

  write(fd, "payload", 7);

  close(fd);
  return 0;
}

int main(int argc, char *argv[]) {
  static struct session sess = {0};

  static struct option long_options[] = {
    /* basic */
    {"host", required_argument, 0, 0},
    {"port", required_argument, 0, 0},

    /* evil stuff */
    {"reset-peer", no_argument, 0, 0},
    {0, 0, 0, 0}
  };

  while (1) {
    int option_index = 0;

    int c = getopt_long(argc, argv, "", long_options, &option_index);
    if (c == -1)
      break;

    if (c == '?') {
      /* the only case where we accept shortops (63 == '?') */
      print_usage(argv[0]);
      exit(EXIT_SUCCESS);
    }

    if (c != 0) {
      /* should not encounter shortops -- get angry and exit */
      fprintf(stderr, "ret %d: short opts not supported\n", c);
      exit(EXIT_FAILURE);
    }

    const char *curr_name = long_options[option_index].name;
    const size_t curr_size = sizeof(long_options[option_index].name);

    if (strncmp(curr_name, "host", curr_size) == 0) {
      (void) strncpy(sess.host, optarg, sizeof(sess.host) - 1);
      continue;
    }

    if (strncmp(curr_name, "port", curr_size) == 0) {
      parse_int_or_die(&sess.port, optarg);
      continue;
    }

    if (strncmp(curr_name, "reset-peer", curr_size) == 0) {
      sess.exec_peer_reset = 1;
      continue;
    }
  }

  (void) session_print_info(&sess);

  if (sess.host[0] == 0 || sess.port == 0) {
    printf("you need to set values for '--host', and '--port'\n");
    exit(EXIT_FAILURE);
  }

  if (sess.exec_peer_reset) do_exec_reset_peer(&sess);

  return EXIT_SUCCESS;
}
