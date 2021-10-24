#include <sstream>

#include <iostream> // TODO remove me

#include "level_builder.h"
#include "utils.h"

namespace psy::gamejam {
  LevelBuilder::ParseStatus LevelBuilder::ParseMetadata(std::istream& is) {
again:
    char c = is.get();

    switch (c) {
    case 'x':
      {
        std::optional<size_t> maybe_x = ParseInt(is);
        if (!maybe_x) return ParseStatus::kError;
        width_ = maybe_x.value();
      }
      goto again;
    case 'y':
      {
        std::optional<size_t> maybe_y = ParseInt(is);
        if (!maybe_y) return ParseStatus::kError;
        height_ = maybe_y.value();
      }
      goto again;
    case 't':
      {
        std::optional<size_t> maybe_x = ParseInt(is);
        if (!maybe_x) return ParseStatus::kError;
        tileset_width_ = maybe_x.value();

        std::optional<size_t> maybe_y = ParseInt(is);
        if (!maybe_y) return ParseStatus::kError;
        tileset_height_ = maybe_y.value();

        std::optional<std::string> maybe_str = ParseStr(is);
        if (!maybe_str) return ParseStatus::kError;
        path_to_tileset_ = maybe_str.value();
      }
      goto again;
    case '\n':
      return ParseStatus::kSuccess;
      break;
    default:
      return ParseStatus::kError;
    }

    return ParseStatus::kSuccess;
  }

  /**
   * this will take a stream, find where the double newline happens, and start
   * parsing tiles.
   */
  LevelBuilder::ParseStatus LevelBuilder::ParseTiles(std::istream& is) {
    char c = 0;
    char prev = 0;
    bool found_pos = false;

    while ((c = is.get()) && c != EOF) {
      if (prev == '\n' && c == '\n') {
        found_pos = true;
        break;
      }

      prev = c;
    }

    /* did not find delimiter; assume malformed tile data */
    if (!found_pos) return ParseStatus::kError;

    /* found delimiter for tile data; start parsing tiles
     * TODO: configurable mapping might be useful at some point
     */

    const size_t reserved_size = tileset_height_ * tileset_width_;
    tiles_.reserve(reserved_size ? reserved_size : 256);

    while ((c = is.get()) && c != EOF) {
      Tile tile;

      switch (c) {
      case ' ': tile.index_ = 0; break;
      case '#': tile.index_ = 1; break;
      case '.': tile.index_ = 2; break;
      case 0xa: continue; /* i had to keep it aligned ok */

      default:
        tiles_.clear();
        return ParseStatus::kError;
      }

      tiles_.push_back(std::move(tile));
    }

    return ParseStatus::kSuccess;
  }

  std::optional<Level> LevelBuilder::FromText(std::istream& is) {
    if (ParseMetadata(is) != ParseStatus::kSuccess)
      return std::nullopt;

    if (ParseTiles(is) != ParseStatus::kSuccess) {
      std::cout << "parse tiles fail" << std::endl;
      return std::nullopt;
    }

    return
      Level(std::move(tiles_), width_, height_, path_to_tileset_);
  }
}
