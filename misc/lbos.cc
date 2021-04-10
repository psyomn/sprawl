#include <chrono>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

#include <unistd.h>

namespace section_1_5_2 {
  void counters_atomic(void);
  void counters_mutex(void);
  void add(std::string label, int& cnt, std::mutex& mm);

  void counters_atomic(void) {
    std::atomic<uint64_t> counter;
    auto fn = [&counter]() noexcept {
      for (size_t i = 0; i < 100; ++i)
        counter.fetch_add(1, std::memory_order_relaxed);
    };

    std::thread t1(fn), t2(fn);

    t1.join();
    t2.join();

    std::cout
      << "counters (atomic) result (should be 200): "
      << counter.load() << std::endl;
  }

  void add(std::string label, int& cnt, std::mutex& mm) {
    (void) label;

    for (size_t i = 0; i < 1000; ++i) {
      const std::lock_guard<std::mutex> lock(mm);
      // std::cout << label << " ";
      cnt = cnt + 1;
      std::this_thread::yield();
    }
  }

  void counters_mutex(void) {
    int counter = 0;

    std::mutex m;

    std::thread
      t1(add, "a", std::ref(counter), std::ref(m)),
      t2(add, "b", std::ref(counter), std::ref(m));

    t1.join();
    t2.join();

    std::cout
      << "counters (mutex) result (should be 2000): "
      << counter << std::endl;
  }
}

int main(int argc, char *argv[])
{
  int opt;

  while ((opt = getopt(argc, argv, "a")) != -1) {
    switch (opt) {
    case 'a':
      section_1_5_2::counters_atomic();
      section_1_5_2::counters_mutex();
      break;
    default: /* '?' */
      fprintf(stderr,
              "Usage: %s <-a> \n"
              "  -a run first example \n",
              argv[0]);

      exit(EXIT_FAILURE);
    }
  }

  return EXIT_SUCCESS;
}
