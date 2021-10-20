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
#include "colshape.h"

namespace psy::gamejam {
  bool Colshape::CollidesWith(const Colshape& other) const {
    const Dim
      t_x = X(),       /* t = this */
      t_y = Y(),
      t_w = W(),
      t_h = H(),

      o_x = other.X(), /* o = other */
      o_y = other.Y(),
      o_w = other.W(),
      o_h = other.H();

    if (t_x > o_w || o_x > t_w) return false;
    if (t_y > o_h || o_y > t_h) return false;

    return true;
  }
}
