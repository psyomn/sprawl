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
#ifndef _SPRAWL_GAMEJAM_COLSHAPE_H
#define _SPRAWL_GAMEJAM_COLSHAPE_H

#include <cstdint>
#include <ostream>

namespace psy::gamejam {
  /**
   * a colshape will be a rectangle in this case.  for my uses, I don't find any
   * good reason to support
   */
  class Colshape {
    /** easilly pluggable dimension unit */
    typedef std::int64_t Dim;
  public:
    /**
     * x, y = coordinates of the rectangle
     * w, h = width, height of the rectangle
     */
    Colshape(Dim x, Dim y, Dim w, Dim h) :
      x_(x), y_(y), w_(w), h_(h) {}

    bool CollidesWith(const Colshape& other) const;

    /**
     * the x coordinate of the rectangle
     */
    inline Dim X() const { return x_; }

    /**
     * the y coordinate of the rectangle
     */
    inline Dim Y() const { return y_; }

    /**
     * the coordinate after adding the _w_idth of the rectangle.
     * for example a retangle at pos (0, 0), with width, height (5, 5) will have
     * a width, and height of 5, 5.
     *
     * for anohter example, a rectangle of pos (2, 3) with width, height (2, 2)
     * will have a width, and height of (4, 5)
     */
    inline Dim W() const { return x_ + w_; }

    /**
     * the coordinate after adding the _h_eight of the rectangle
     */
    inline Dim H() const { return y_ + h_; }

    friend std::ostream& operator<<(std::ostream& os, const Colshape& obj) {
      os
        << "x:" << obj.X() << " "
        << "y:" << obj.Y() << " "
        << "w:" << obj.W() << " "
        << "h:" << obj.H();
      return os;
    }
  private:
    Dim x_;
    Dim y_;
    Dim w_;
    Dim h_;
  };
}

#endif
