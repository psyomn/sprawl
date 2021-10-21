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
#include "level.h"

namespace psy::gamejam {
  /* Level */
  Level::Level(std::vector<Tile>&& tiles, size_t width, size_t height, std::string path_to_tileset) :
    tiles_(std::move(tiles)),
    width_(width),
    height_(height),
    path_to_tileset_(path_to_tileset)
  {}
}
