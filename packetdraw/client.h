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
#ifndef _SPRAWL_PACKETDRAW_CLIENT_H
#define _SPRAWL_PACKETDRAW_CLIENT_H

#include <stdint.h>

#include "message.h"

struct client {
  char host[256];
  enum packetdraw_msgtype type;
  uint16_t port;
  uint32_t x;
  uint32_t y;
  uint32_t w;
  uint32_t h;
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

int packetdraw_client_send_message(struct client *client);

#endif
