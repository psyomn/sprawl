#include "graphics.h"

#include "message.h"

#include <stdio.h>

int packetdraw_graphics_init(struct packetdraw_win *win) {
  /* window stuff */
  if (SDL_Init(SDL_INIT_VIDEO) == -1) {
    fprintf(stderr, "problem with SDL: %s\n", SDL_GetError());
    return -1;
  }

  const size_t width = 800;
  const size_t height = 600;
  win->window = SDL_CreateWindow(
      "packetdraw",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      width,
      height,
      SDL_WINDOW_SHOWN
  );

  if (win->window == NULL) goto error;
  win->surface = SDL_GetWindowSurface(win->window);

  /* renderer */
  win->renderer = SDL_CreateRenderer(
      win->window,
      -1,
      SDL_RENDERER_ACCELERATED
  );

  if (win->renderer == NULL) goto cleanup;

  return 0;

cleanup:
  SDL_DestroyWindow(win->window);
error:
  return -1;
}

int packetdraw_graphics_free(struct packetdraw_win *win) {
  SDL_DestroyWindow(win->window);
  SDL_Quit();
  return 0;
}

static void draw_rect(struct packetdraw_win *win, struct rectangle_message *rect_msg) {
  SDL_Rect rect = {
    .x = rect_msg->x,
    .y = rect_msg->y,
    .h = rect_msg->to_x,
    .w = rect_msg->to_y,
  };

  SDL_SetRenderDrawColor(
      win->renderer,
      rect_msg->rgb[0],
      rect_msg->rgb[1],
      rect_msg->rgb[2],
      0xff
  );

  SDL_RenderDrawRect(win->renderer, &rect);
}

static void draw_pixel(struct packetdraw_win *win, struct pixel_message *pixel) {
  SDL_SetRenderDrawColor(
      win->renderer,
      pixel->rgb[0],
      pixel->rgb[1],
      pixel->rgb[2],
      0xff
  );

  SDL_RenderDrawPoint(win->renderer, pixel->x, pixel->y);
}

static void draw_line(struct packetdraw_win *win, struct line_message *line) {
  SDL_SetRenderDrawColor(
      win->renderer,
      line->rgb[0],
      line->rgb[1],
      line->rgb[2],
      0xff
  );

  SDL_RenderDrawLine(
      win->renderer,
      line->x,
      line->y,
      line->to_x,
      line->to_y
  );
}

static void draw_message(struct packetdraw_win *win, struct message *message) {
  /* TODO: casting unions like below might be dangerous.
   * don't do this in prod. :o) */
  switch (message->discriminator) {
  case PACKETDRAW_MSGTYPE_UNKNOWN:
    return;
  case PACKETDRAW_MSGTYPE_PIXEL:
    draw_pixel(win, &message->as.pixel);
    return;
  case PACKETDRAW_MSGTYPE_LINE:
    draw_line(win, &message->as.line);
    return;
  case PACKETDRAW_MSGTYPE_RECTANGLE:
    draw_rect(win, &message->as.rectangle);
    return;
  }
}

/**
 * classic render loop
 */
int packetdraw_graphics_render(struct packetdraw_win *win,
                               struct packetdraw_server *srv) {
  int alive = 1;
  SDL_Event e;

  while (alive) {
    while (SDL_PollEvent(&e) != 0)
      if (e.type == SDL_QUIT) alive = 0;

    for (size_t i = 0; i < srv->messages_len; ++i)
       draw_message(win, &srv->messages[i]);

    // SDL_BlitSurface(win->surface, NULL, win->surface, NULL);
    SDL_UpdateWindowSurface(win->window);
    SDL_RenderPresent(win->renderer);
  }

  return 0;
}

