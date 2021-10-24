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

#include <iostream>
#include <exception>
#include <cstddef>

#include <SDL2/SDL.h>

class Graphics {
public:
  Graphics(
      std::string title,
      std::size_t w,
      std::size_t h) :
    title_(title), window_width_(w), window_height_(h),
    window_(nullptr), surface_(nullptr), renderer_(nullptr)
  {
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
      throw std::runtime_error("could not init video");

    window_ = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        window_width_,
        window_height_,
        SDL_WINDOW_SHOWN
    );
    if (window_ == nullptr)
      throw std::runtime_error("could not initialize window");

    renderer_ = SDL_CreateRenderer(
        window_,
        -1,
        SDL_RENDERER_ACCELERATED
    );
    if (renderer_ == nullptr)
      throw std::runtime_error("could not initialize renderer");
  }

  ~Graphics() {
    SDL_DestroyWindow(window_);
    SDL_Quit();
  }

  void RenderLoop() {
    bool alive = true;
    SDL_Event e;

    while (alive) {
      while (SDL_PollEvent(&e) != 0)
       if (e.type == SDL_QUIT) alive = 0;

      // SDL_BlitSurface(winow_->surface, NULL, window_->surface, NULL);
      SDL_UpdateWindowSurface(window_);
      SDL_RenderPresent(renderer_);
    }
  }
private:
  std::string title_;
  std::size_t window_width_;
  std::size_t window_height_;

  SDL_Window* window_;
  SDL_Surface* surface_;
  SDL_Renderer* renderer_;
};

int main(int argc, char *argv[]) {
  (void) argc;
  (void) argv;

  constexpr std::size_t window_height = 240;
  constexpr std::size_t window_width = 256;

  Graphics graphics(
      "sdl/sprawl examples",
      window_width,
      window_height);

  graphics.RenderLoop();

  return 0;
}
