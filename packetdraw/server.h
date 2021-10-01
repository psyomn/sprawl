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
#ifndef _CACOPHAGY_PACKETDRAW_SERVER_H
#define _CACOPHAGY_PACKETDRAW_SERVER_H

#include <stddef.h>
#include <stdint.h>

#include "message.h"

enum packetdraw_server_status {
  PACKETDRAW_SERVER_SUCCESS = 0,
  PACKETDRAW_SERVER_ENOMEM  = 1,
  PACKETDRAW_SERVER_BADSOCK = 2,
  PACKETDRAW_SERVER_BADBIND = 3,
};

struct packetdraw_server {
  int port;
  int fd;
  uint8_t buffer[512];

  size_t messages_len;
  size_t messages_cap;
  struct message messages[];
};

struct packetdraw_server* packetdraw_server_create(int port,
                                                   enum packetdraw_server_status *status);

int packetdraw_server_free(struct packetdraw_server *server);

int packetdraw_server_async_listen(struct packetdraw_server *server);

void packetdraw_server_insert_message_from_buffer(
    struct packetdraw_server *server,
    uint8_t *buffer);

#endif
