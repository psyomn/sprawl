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
#ifndef _CACOPHAGY_GAMEJAM_LEVEL
#define _CACOPHAGY_GAMEJAM_LEVEL

#include <optional>
#include <vector>
#include <istream>

namespace psy::gamejam {
  class Tile {
  public:
    size_t x;
    size_t y;
  };

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
  private:
    std::vector<Tile> tiles_;
    size_t width_;
    size_t height_;
    std::string path_to_tileset_;
  };

  /**
   * LevelBuilder - the idea is to provide means to create levels really quick
   * with some dirty textfiles.  Ideal maybe for gamejams, but perhaps not for
   * something very full fledged.
   */
  class LevelBuilder {
  public:
    enum class ParseStatus { kUnknown, kSuccess, kError };

    std::optional<Level> FromText(std::istream& is);
    ParseStatus ParseMetadata(std::istream& is);
    ParseStatus ParseTiles(std::istream& is);

    size_t width_;
    size_t height_;
    std::string path_to_tileset_;
    size_t tileset_width_;
    size_t tileset_height_;
  };
}

#endif
