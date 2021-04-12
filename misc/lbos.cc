/* This is me writing code to accompany my reading of the little book
 * of semaphores. The book is free and very good. Read it.
 */

#include <atomic>
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

namespace section_3_3 {
  class Semaphore {
  public:
    explicit Semaphore(std::int64_t s) : value(s) {}

    void signal(void) {
      value.fetch_add(1, std::memory_order_relaxed);
    }

    void wait(void) const {
      // block until we get signaled. yield for semi spinlock
      while (value.load() <= 0) std::this_thread::yield();
    }

  private:
    std::atomic<int64_t> value;
  };

  void rendez_vous(void);

  void rendez_vous(void) {
    Semaphore sa(0), sb(0);

    std::thread ta([&sa, &sb](){
      std::cout << "thread a, statement 1" << std::endl;
      sb.signal();
      sa.wait();
      std::cout << "thread a, statement 2" << std::endl;
    });

    std::thread tb([&sa, &sb](){
      std::cout << "thread b, statement 1" << std::endl;
      sb.wait();
      sa.signal();
      std::cout << "thread b, statement 2" << std::endl;
    });

    ta.join();
    tb.join();
  }
}

int main(int argc, char *argv[])
{
  int opt;

  while ((opt = getopt(argc, argv, "ab")) != -1) {
    switch (opt) {
    case 'a':
      section_1_5_2::counters_atomic();
      section_1_5_2::counters_mutex();
      break;
    case 'b':
      section_3_3::rendez_vous();
      break;
    default: /* '?' */
      fprintf(stderr,
              "Usage: %s <-ab> \n"
              "  -a run section 1.5.x examples \n"
              "  -b run section 3.3.x rendez vous examples\n"
              , argv[0]);

      exit(EXIT_FAILURE);
    }
  }

  return EXIT_SUCCESS;
}
