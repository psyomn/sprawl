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
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "event.h"
#include "message.h"
#include "utils.h"

#include <queue>

TEST(psycal, sooner_comparator) {
  using psy::psycal::Event;

  auto tm1 = psy::psycal::Utils::ProcessTimestamp("13h00");
  std::priority_queue<Event, std::vector<Event>, Event::Comparator>
    events(Event::SoonerCmp);

  for (auto el : {"17h00", "13h01", "14h02"}) {
    auto tm = psy::psycal::Utils::ProcessTimestamp(el).value();
    events.push(Event(std::move(tm), el));
  }

  EXPECT_EQ(events.top().GetMessage(), "13h01");
  events.pop();

  EXPECT_EQ(events.top().GetMessage(), "14h02");
  events.pop();

  EXPECT_EQ(events.top().GetMessage(), "17h00");
  events.pop();
}
