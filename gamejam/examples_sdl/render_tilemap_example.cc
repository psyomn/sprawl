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
#include <vector>

#include <cstddef>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "gamejam/tile.h"

#define SPRAWL_FL __FILE__ ":" "__LINE__"

/**
 * Load a texture as a tilemap.  Depends on SDL.
 */
class Tilemap {
public:
  Tilemap(std::string texture_path, SDL_Renderer *renderer) :
    renderer_(renderer) ,
    texture_path_(texture_path),
    surface_(IMG_Load(texture_path_.c_str())),
    texture_(SDL_CreateTextureFromSurface(renderer_, surface_)) {
    if (surface_ == nullptr) throw std::runtime_error(SPRAWL_FL " could not load the surface, at path: " + texture_path_);
    if (texture_ == nullptr) throw std::runtime_error(SPRAWL_FL " could not create the texture");
  }

  ~Tilemap() {
    SDL_DestroyTexture(texture_);
    SDL_FreeSurface(surface_);
  }
private:
  SDL_Renderer *renderer_;
  std::string texture_path_;
  SDL_Surface *surface_;
  SDL_Texture *texture_;
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
      std::string title,
      std::size_t w,
      std::size_t h) :
    title_(title), window_width_(w), window_height_(h),
    window_(nullptr), surface_(nullptr), renderer_(nullptr),
    img_flags_(IMG_INIT_PNG) {
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

  SDL_Renderer* GetRendererPtr() {
    return renderer_;
  }

  void AddTilemap(Tilemap&& tilemap) { tilemaps_.push_back(std::move(tilemap)); }

  void RenderLoop() {
    bool alive = true;
    SDL_Event e;

    psy::gamejam::Tile tile_00, tile_01;
    tile_00.index_ = 0;
    tile_01.index_ = 1;

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

  /** default to png for now */
  int img_flags_;

  std::vector<Tilemap> tilemaps_;
};

int main(int argc, char *argv[]) {
  (void) argc;
  (void) argv;

  const std::string asset_paths[] = {
    "gamejam/examples_sdl/assets/Autumn_Ground_32x32.png",
    "gamejam/examples_sdl/assets/Summer_Ground_32x32.png",
  };

  constexpr std::size_t window_height = 240;
  constexpr std::size_t window_width = 256;

  Graphics graphics(
      "sdl/sprawl examples",
      window_width,
      window_height
  );

  Tilemap
    tilemap_autumn(asset_paths[0], graphics.GetRendererPtr()),
    tilemap_summer(asset_paths[1], graphics.GetRendererPtr());

  graphics.AddTilemap(std::move(tilemap_autumn));
  graphics.AddTilemap(std::move(tilemap_summer));

  graphics.RenderLoop();

  return 0;
}
