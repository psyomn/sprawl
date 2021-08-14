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
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

#include "psycal/dump.h"
#include "psycal/utils.h"

#include <chrono>
#include <fstream>
#include <functional>
#include <thread>

void EngraveDate(char *, const size_t );
void EngraveBattery(char *, const size_t );
void EngraveCalendar(char *, const size_t);
void EngraveReminder(char *, const size_t);

// originally was a simple C program, so I'll have to migrate this into C++ and
// not let it into this weird mixture it currently is like.
int main(int argc, char* argv[]) {
  (void) argc, (void) argv;

  std::function<void(char*, const size_t)> engravers[] = {
    EngraveCalendar,
    EngraveReminder,
    EngraveBattery,
    EngraveDate,
  };

  char buffer[256] = {0};

  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    memset(buffer, 0, sizeof(buffer));

    for (auto& engraver : engravers)
      engraver(buffer, sizeof(buffer));

    Display* display = XOpenDisplay(NULL);
    if (!display) continue;

    int screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);
    if (!root) goto cleanup;

    Xutf8SetWMProperties(display, root, buffer, NULL, NULL, 0, NULL, NULL, NULL);

cleanup:
    XCloseDisplay(display);
  }


  return 0;
}

void EngraveDate(char* buffer, const size_t size) {
  const time_t now = time(NULL);
  const struct tm time = *localtime(&now);
  const char* date_fmt = "[%02d:%02d][%02d/%02d/%02d]";
  char store[64] = {0};

  sprintf(store, date_fmt,
          time.tm_hour, time.tm_min,
          time.tm_mday, time.tm_mon + 1, time.tm_year + 1900);

  strncat(buffer, store, size);
}

void EngraveBattery(char* _buffer, const size_t _buff_size) {
  const uint8_t max_batteries = 32;
  const char* fmt = "[BAT:%03d]";
  const size_t battery_size = 16;
  char battery[battery_size] = {0};

  FILE* bat_file = NULL;
  const char* battery_dev_path = "/sys/class/power_supply/BAT%d/capacity";
  const size_t path_buffer_size = 256;
  char path_buffer[path_buffer_size] = {0};
  uint16_t cumulative_power = 0;

  uint8_t bat_count = 0;
  for (; bat_count < max_batteries; ++bat_count) {
    path_buffer[0] = 0;

    snprintf(path_buffer,
             path_buffer_size,
             battery_dev_path,
             bat_count);

    bat_file = fopen(path_buffer, "r");

    if (!bat_file)
      break;

    uint8_t fcontents[4] = {0};
    fread(fcontents, 1, 3, bat_file);
    fclose(bat_file);

    uint16_t tmp_power = 0;
    for (size_t i = 0; i < sizeof(fcontents) - 1; ++i) {
      const uint8_t curr_char = fcontents[i];
      if (curr_char == 0x0A)
        break;

      tmp_power = (curr_char - 48) + (tmp_power * 10);
    }

    cumulative_power += tmp_power;
  }

  snprintf(battery, battery_size,
           fmt,
           cumulative_power / bat_count);

  strncat(_buffer, battery, _buff_size);
}

void EngraveCalendar(char *buffer, const size_t buff_size) {
  using namespace psy;
  std::ifstream is(psycal::Utils::EventsFilePath(), std::ifstream::binary);
  is.imbue(std::locale::classic());
  auto events = psycal::ReadEventsFrom(is);

  strncat(buffer, "[", buff_size);
  for (const auto& event : events) {
    char hourglass[32] = {0};
    std::strftime(hourglass, sizeof(hourglass), "%Hh%M", &event.GetRawTime());

    strncat(buffer, hourglass, buff_size);
    strncat(buffer, ":", buff_size);
    strncat(buffer, event.GetMessage().c_str(), buff_size);
    strncat(buffer, "/", buff_size);
  }
  strncat(buffer, "]", buff_size);
}

void EngraveReminder(char* buffer, const size_t size) {
  static const unsigned char reminder[] = {
    '[',  0xe6,
    0xad, 0xa3,
    0xe7, 0xbe,
    0xa9, ']',
    0x0,
  };

  strncat(buffer, reinterpret_cast<const char*>(reminder), size);
}
