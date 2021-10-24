#ifndef _SPRAWL_GAMEJAM_TILE_H
#define _SPRAWL_GAMEJAM_TILE_H

#include <cstddef>

namespace psy::gamejam {
  struct Tile {
    /**
     * this is the index of the tile in the graphics resource.  Count left to
     * right, top to bottom. eg:
     *
     * +---+---+---+
     * | 0 | 1 | 2 |
     * +---+---+---+
     * | 3 | 4 | 5 |
     * +---+---+---+
     * | 6 | 7 | n |
     * +---+---+---+
     * |...|...|...|
     * +---+---+---+
     */
    int index_;
  };
}

#endif
