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
#ifndef _CACOPHAGY_PACKETDRAW_MESSAGE_H
#define _CACOPHAGY_PACKETDRAW_MESSAGE_H

#include <stdint.h>
#include <stddef.h>

enum packetdraw_msgtype {
  PACKETDRAW_MSGTYPE_UNKNOWN   = 0,
  PACKETDRAW_MSGTYPE_PIXEL     = 1,
  PACKETDRAW_MSGTYPE_LINE      = 2,
  PACKETDRAW_MSGTYPE_RECTANGLE = 3,
};

struct pixel_message {
  uint32_t x;
  uint32_t y;
  uint8_t rgb[3];
} __attribute__((packed));

struct line_message {
  uint32_t x;
  uint32_t y;
  uint32_t to_x;
  uint32_t to_y;
  uint8_t rgb[3];
} __attribute__((packed));

struct rectangle_message {
  uint32_t x;
  uint32_t y;
  uint32_t to_x;
  uint32_t to_y;
  uint8_t rgb[3];
} __attribute__((packed));

struct message {
  uint8_t discriminator;
  union {
    struct pixel_message pixel;
    struct rectangle_message rectangle;
    struct line_message line;
  } as;
} __attribute__((packed));

#endif
