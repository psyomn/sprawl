#ifndef _SPRAWL_PACKETDRAW_GRAPHICS_H
#define _SPRAWL_PACKETDRAW_GRAPHICS_H

#include <SDL2/SDL.h>

#include "message.h"
#include "server.h"

struct packetdraw_win {
  SDL_Window *window;
  SDL_Surface *surface;
  SDL_Renderer *renderer;
};

int packetdraw_graphics_init(struct packetdraw_win *win);
int packetdraw_graphics_free(struct packetdraw_win *win);
int packetdraw_graphics_render(struct packetdraw_win *win,
                               struct packetdraw_server *srv);

#endif
