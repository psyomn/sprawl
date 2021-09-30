#ifndef _CACOPHAGY_PACKETDRAW_MESSAGE_H
#define _CACOPHAGY_PACKETDRAW_MESSAGE_H

#include <stdint.h>
#include <stddef.h>

enum packetdraw_msgtype {
  PACKETDRAW_MSGTYPE_PIXEL,
  PACKETDRAW_MSGTYPE_LINE,
  PACKETDRAW_MSGTYPE_RECTANGLE,
};

struct pixel_message {
  uint32_t x;
  uint32_t y;
  uint8_t rgb[3];
};

struct line_message {
  uint32_t x;
  uint32_t y;
  uint32_t to_x;
  uint32_t to_y;
  uint8_t rgb[3];
};

struct rectangle_message {
  uint32_t x;
  uint32_t y;
  uint32_t to_x;
  uint32_t to_y;
  uint8_t rgb[3];
};

struct message {
  uint8_t discriminator;
  union {
    struct pixel_message pixel;
    struct rectangle_message rectangle;
    struct line_message line;
  };
} __attribute__((packed));

#endif
