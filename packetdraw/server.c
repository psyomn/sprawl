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
#include "server.h"

/* sys */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>

#include <pthread.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

struct packetdraw_server* packetdraw_server_create(int port,
                                                   enum packetdraw_server_status *status) {
  const size_t initial_message_amount = 4096;

  struct packetdraw_server *server =
    calloc(1, sizeof(*server) + sizeof(struct message) * initial_message_amount);

  if (server == NULL) {
    *status = PACKETDRAW_SERVER_ENOMEM;
    goto error;
  }

  if (errno != 0) {
    *status = PACKETDRAW_SERVER_ENOMEM;
    goto cleanup;
  }

  if ((server->fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    *status = PACKETDRAW_SERVER_BADSOCK;
    goto cleanup;
  }

  int optval = 1;
  if ((setsockopt(server->fd, SOL_SOCKET, SO_REUSEPORT,
                  &optval, sizeof(optval))) == -1) {
    *status = PACKETDRAW_SERVER_BADSOCK;
    goto cleanup;
  }

  server->port = port;
  server->messages_len = 0;
  server->messages_cap = initial_message_amount;

  struct sockaddr_in addr = {0};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = server->port;

  if (bind(server->fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    *status = PACKETDRAW_SERVER_BADBIND;
    goto cleanup;
  }

  *status = PACKETDRAW_SERVER_SUCCESS;
  return server;

cleanup:
  free(server);
error:
  return NULL;
}

int packetdraw_server_free(struct packetdraw_server *server) {
  free(server);
  return 0;
}

static void *async_listen(void *maybe_server) {
  struct packetdraw_server *server = maybe_server;

  while (1) {
    uint8_t buff[512] = {0};
    struct sockaddr_storage peer = {0};
    socklen_t peer_len = sizeof(peer);

    const ssize_t len = recvfrom(
        server->fd, buff, sizeof(buff), 0,
        (struct sockaddr *)&peer, &peer_len);

    if (len == -1) {
      perror("recvfrom");
      continue;
    }

    packetdraw_server_insert_message_from_buffer(
        server,
        buff);
  }

  return NULL;
}

int packetdraw_server_async_listen(struct packetdraw_server *server) {
  pthread_t thread = {0};
  pthread_create(&thread, NULL, &async_listen, (void*)server);
  return 0;
}

void packetdraw_server_insert_message_from_buffer(struct packetdraw_server *server,
                                                  uint8_t *buffer) {
  struct message *mess = (struct message*)buffer;

  if (server->messages_len >= server->messages_cap) {
    fprintf(stderr, "reached message capacity\n");
    return;
  }

  switch (mess->discriminator) {
  case PACKETDRAW_MSGTYPE_PIXEL:
    {
      const size_t len = (server->messages_len)++;
      server->messages[len].discriminator = PACKETDRAW_MSGTYPE_PIXEL;
      server->messages[len].as.pixel.x = htonl(mess->as.pixel.x);
      server->messages[len].as.pixel.y = htonl(mess->as.pixel.y);
      server->messages[len].as.pixel.rgb[0] = mess->as.pixel.rgb[0];
      server->messages[len].as.pixel.rgb[1] = mess->as.pixel.rgb[1];
      server->messages[len].as.pixel.rgb[2] = mess->as.pixel.rgb[2];
    }
    break;
  case PACKETDRAW_MSGTYPE_LINE:
    {
      const size_t len = (server->messages_len)++;
      server->messages[len].discriminator = PACKETDRAW_MSGTYPE_LINE;
      server->messages[len].as.pixel.x = htonl(mess->as.pixel.x);
      server->messages[len].as.line.x = htonl(mess->as.line.x);
      server->messages[len].as.line.y = htonl(mess->as.line.y);
      server->messages[len].as.line.to_x = htonl(mess->as.line.to_x);
      server->messages[len].as.line.to_y = htonl(mess->as.line.to_y);
      server->messages[len].as.line.rgb[0] = mess->as.line.rgb[0];
      server->messages[len].as.line.rgb[1] = mess->as.line.rgb[1];
      server->messages[len].as.line.rgb[2] = mess->as.line.rgb[2];
    }
    break;
  case PACKETDRAW_MSGTYPE_RECTANGLE:
    {
      const size_t len = (server->messages_len)++;
      server->messages[len].discriminator = PACKETDRAW_MSGTYPE_RECTANGLE;
      server->messages[len].as.rectangle.x = htonl(mess->as.line.x);
      server->messages[len].as.rectangle.y = htonl(mess->as.line.y);
      server->messages[len].as.rectangle.to_x = htonl(mess->as.line.to_x);
      server->messages[len].as.rectangle.to_y = htonl(mess->as.line.to_y);
      server->messages[len].as.rectangle.rgb[0] = mess->as.rectangle.rgb[0];
      server->messages[len].as.rectangle.rgb[1] = mess->as.rectangle.rgb[1];
      server->messages[len].as.rectangle.rgb[2] = mess->as.rectangle.rgb[2];
    }
    break;
  default:
    fprintf(stderr, "draw message was not recognized...\n");
    for (size_t i = 0; i < sizeof(struct message); ++i) {
      printf("%02x ", *(buffer + i));
    }
  }
}

