#include <sstream>

#include "level_builder.h"

namespace psy::gamejam {
  LevelBuilder::ParseStatus LevelBuilder::ParseMetadata(std::istream& is) {
    auto parse_int_fn = [](std::istream& mis) -> std::optional<size_t> {
      char c = mis.get();

      if (c != ':') return std::nullopt;
      if (mis.peek() == '\n' || mis.peek() == EOF) return std::nullopt;

      std::stringstream buff;
      while (mis.peek() != ':' && (c = mis.get()) != '\n' && c != EOF)
        buff << c;

      size_t temp_i = 0;
      buff >> temp_i;

      return temp_i;
    };

    auto parse_str_fn = [](std::istream& mis) -> std::optional<std::string> {
      std::stringstream buff;

      char c = mis.get();

      if (c != ':') return std::nullopt;
      if (mis.peek() == '\n' || mis.peek() == EOF) return std::nullopt;

      while ((c = mis.get()) != '\n' && c != EOF)
        buff << c;

      return buff.str();
    };

again:
    char c = is.get();

    switch (c) {
    case 'x':
      {
        std::optional<size_t> maybe_x = parse_int_fn(is);
        if (!maybe_x) return ParseStatus::kError;
        width_ = maybe_x.value();
      }
      goto again;
    case 'y':
      {
        std::optional<size_t> maybe_y = parse_int_fn(is);
        if (!maybe_y) return ParseStatus::kError;
        height_ = maybe_y.value();
      }
      goto again;
    case 't':
      {
        std::optional<size_t> maybe_x = parse_int_fn(is);
        if (!maybe_x) return ParseStatus::kError;
        tileset_width_ = maybe_x.value();

        std::optional<size_t> maybe_y = parse_int_fn(is);
        if (!maybe_y) return ParseStatus::kError;
        tileset_height_ = maybe_y.value();

        std::optional<std::string> maybe_str = parse_str_fn(is);
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

  LevelBuilder::ParseStatus LevelBuilder::ParseTiles(std::istream& is) {
    return ParseStatus::kSuccess;
  }

  std::optional<Level> LevelBuilder::FromText(std::istream& is) {
    if (ParseMetadata(is) != ParseStatus::kSuccess)
      return std::nullopt;

    if (ParseTiles(is) != ParseStatus::kSuccess)
      return std::nullopt;

    return
      Level({}, width_, height_, path_to_tileset_);
  }
}
