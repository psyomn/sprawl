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

/*
** Hypothetical setup for a game in SDL.  Depending on how reusable some of
** these aspects are, I'm planning to abstract them in the gamejam library in
** `psyomn/sprawl`.
*/

#include <iostream>
#include <exception>
#include <memory>
#include <vector>

#include <cstddef>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "gamejam/tile.h"

/**
 * Load a texture as a tilemap.  Depends on SDL.
 */
class Tilemap {
public:
  int img_init_ret_;
  std::string texture_path_;
  SDL_Renderer *renderer_;
  SDL_Surface *surface_;
  SDL_Texture *texture_;

  Tilemap(const std::string& texture_path, SDL_Renderer *renderer) :
    img_init_ret_(IMG_Init(IMG_INIT_PNG)),
    texture_path_(texture_path),
    renderer_(renderer),
    surface_(IMG_Load(texture_path_.c_str())),
    texture_(SDL_CreateTextureFromSurface(renderer_, surface_))
  {
    if (renderer_ == nullptr) throw std::runtime_error(" you must supply a renderer");
    if (surface_ == nullptr) throw std::runtime_error(" could not load the surface, at path: " + texture_path_);
    if (texture_ == nullptr) throw std::runtime_error(" could not create the texture");

    SDL_FreeSurface(surface_);
    surface_ = nullptr;
  }

  ~Tilemap() {
    std::cout << "FFUUUCK" << std::endl;
    if (texture_) SDL_DestroyTexture(texture_);
  }
};

class Entity {
public:
  Entity(const Tilemap& tilemap) : tilemap_(tilemap) {}
private:
  const Tilemap& tilemap_;
};

/**
 * Basic graphics rendering routines.  Depends on SDL.
 */
class Graphics {
public:
  Graphics(
    std::string title, std::size_t w, std::size_t h) :
    title_(title), window_width_(w), window_height_(h),
    window_(nullptr), renderer_(nullptr),
    tilemaps_(std::vector<std::unique_ptr<Tilemap>>{})
  {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
      throw std::runtime_error("could not init sdl_init_everything");

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

  SDL_Renderer* GetRendererPtr() {
    return renderer_;
  }

  void AddTilemap(const std::string& path) {
    auto tm = std::make_unique<Tilemap>(path, renderer_);
    tilemaps_.push_back(std::move(tm));
  }

  void RenderLoop() {
    bool alive = true;
    SDL_Event e;

    psy::gamejam::Tile tile_00, tile_01;
    tile_00.index_ = 0;
    tile_01.index_ = 1;

    SDL_Rect position = {
      .x = 32*0, .y = 0,
      .w = 32, .h = 32,
    };

    SDL_Rect position2 = {
      .x = 32*1, .y = 0,
      .w = 32, .h = 32,
    };

    SDL_Rect position3 = {
      .x = 32*2, .y = 0,
      .w = 32, .h = 32,
    };



    SDL_Rect dest = {
      .x = 0, .y = 0,
      .w = 32, .h = 32,
    };

    SDL_Rect dest2 = {
      .x = 32, .y = 0,
      .w = 32, .h = 32,
    };

    SDL_Rect dest3 = {
      .x = 64, .y = 0,
      .w = 32, .h = 32,
    };

    while (alive) {
      while (SDL_PollEvent(&e) != 0)
        if (e.type == SDL_QUIT) alive = 0;

      SDL_RenderCopy(renderer_, tilemaps_[0]->texture_, &dest, &position);
      SDL_RenderCopy(renderer_, tilemaps_[1]->texture_, &dest, &position2);

      SDL_RenderPresent(renderer_);
      SDL_Delay(500);
    }
  }
private:
  std::string title_;
  std::size_t window_width_;
  std::size_t window_height_;

  SDL_Window* window_;
  SDL_Renderer* renderer_;

  std::vector<std::unique_ptr<Tilemap>> tilemaps_;
};

int main(int argc, char *argv[]) {
  (void) argc;
  (void) argv;
  constexpr std::size_t window_height = 240;
  constexpr std::size_t window_width = 256;

  Graphics graphics(
      "sdl/sprawl examples",
      window_width,
      window_height
  );

  graphics.AddTilemap(
    "gamejam/examples_sdl/assets/Autumn_Ground_32x32.png"
  );

  graphics.AddTilemap(
    "gamejam/examples_sdl/assets/Summer_Ground_32x32.png"
  );

  graphics.RenderLoop();

  return 0;
}
