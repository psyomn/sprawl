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
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <pthread.h>
#include <X11/Xlib.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

const static time_t MU_SLEEP_TIME = 5 * 1000000;

void engrave_date(char *_buffer, size_t _buff_size);
void engrave_wifi(char *_buffer, size_t _buff_size);
void engrave_batt(char *_buffer, size_t _buff_size);
uint16_t convert_battery_reading();

int main(int argc, char* argv[]) {
  (void) argc, (void) argv;

  void (*engravers[])(char*, size_t) = {
    &engrave_wifi,
    &engrave_batt,
    &engrave_date
  };

  const size_t engraver_size = sizeof(engravers) / sizeof(&engrave_date);

  static Display* display = NULL;
  static int screen = 0;
  const size_t buff_size = 255;
  char buffer[buff_size];

  while (1) {
    buffer[0] = 0;

    for (size_t x = 0; x < engraver_size; ++x)
      engravers[x](buffer, buff_size);

    display = XOpenDisplay(NULL);
    screen = DefaultScreen(display);

    const Window root = RootWindow(display, screen);
    if (!root)
      break;

    XStoreName(display, root, buffer);

#ifdef DEBUG
    printf("%s\n", buffer);
#endif
    XCloseDisplay(display);

    usleep(MU_SLEEP_TIME);
  }


  return 0;
}

void engrave_date(char* _buffer, size_t _buff_size) {
  const time_t now = time(NULL);
  const struct tm time = *localtime(&now);
  const char* date_fmt = "[%02d:%02d][%02d/%02d/%02d]";
  char store[32];

  sprintf(store, date_fmt,
          time.tm_hour, time.tm_min,
          time.tm_mday, time.tm_mon + 1, time.tm_year + 1900);

  strncat(_buffer, store, _buff_size);
}

void engrave_wifi(char* _buffer, size_t _buff_size) {
  const char* label = "Wifi:";
  const char* fmt = "[%s%s]";
  char wifi[16];

  sprintf(wifi, fmt, label, "todo");
  strncat(_buffer, wifi, _buff_size);
}

void engrave_batt(char* _buffer, size_t _buff_size) {
  const uint8_t max_batteries = 32;
  const char* fmt = "[BAT:%03d]";
  const size_t battery_size = 16;
  char battery[battery_size];

  FILE* bat_file = NULL;
  const char* battery_dev_path = "/sys/class/power_supply/BAT%d/capacity";
  const size_t path_buffer_size = 256;
  char path_buffer[path_buffer_size];
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

    char fcontents[4];
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
           fmt, cumulative_power / bat_count);

  strncat(_buffer, battery, _buff_size);
}
