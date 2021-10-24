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
#ifndef _SPRAWL_GAMEJAM_LEVEL_H
#define _SPRAWL_GAMEJAM_LEVEL_H

#include <vector>
#include <istream>

#include "tile.h"

namespace psy::gamejam {
  class Level {
  public:
    Level(
        std::vector<Tile>&& tiles,
        size_t width,
        size_t height,
        std::string path_to_tileset_
    );
    inline size_t Width() const { return width_; }
    inline size_t Height() const { return height_; }
    std::string TilesetPath() const { return path_to_tileset_; }
    inline const std::vector<Tile>& GetTiles() const { return tiles_; }
    /* you're trusted not to supply bogus here */
    inline const Tile& At(size_t x, size_t y) const { return tiles_[width_ * y + x]; }
  private:
    std::vector<Tile> tiles_;
    size_t width_;
    size_t height_;
    std::string path_to_tileset_;
  };
}

#endif
