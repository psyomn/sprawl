#include "graphics.h"

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

/**
 * classic render loop
 */
int packetdraw_graphics_render(struct packetdraw_win *win) {
  int alive = 1;
  SDL_Event e;

  while (alive) {
    while (SDL_PollEvent(&e) != 0)
      if (e.type == SDL_QUIT) alive = 0;

    // SDL_BlitSurface(win->surface, NULL, win->surface, NULL);
    SDL_UpdateWindowSurface(win->window);
    SDL_Rect rect = {
      .x = 10, .y = 10, .h = 100, .w = 100,
    };
    SDL_SetRenderDrawColor(win->renderer, 0x00, 0x00, 0xff, 0xff);
    SDL_RenderDrawRect(win->renderer, &rect);
    SDL_RenderPresent(win->renderer);
  }

  return 0;
}

