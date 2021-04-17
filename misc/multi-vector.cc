// TODO: redo with fork example
//
// Basically, wanted to make a very simple example, remind myself of
// what tends to be annoying when doing multicore programming, and
// what patterns can benefit me a little more.
//
#include <cstdint>
#include <iostream>
#include <sstream>
#include <mutex>
#include <thread>
#include <utility> // pair, make_pair
#include <vector>

// C - getopt
#include <unistd.h>

#include "common/granulator.h"

template <typename T>
class MTVector {
public:
  MTVector() : items_({}), mut_({}) {}
  ~MTVector() {}
  MTVector(MTVector& other) = delete;
  MTVector(MTVector&& other) = delete;
  MTVector& operator=(MTVector& other) = delete;
  MTVector& operator=(MTVector&& other) = delete;

  void Add(T t) {
    std::lock_guard lock(mut_);
    items_.push_back(t);
  }

  const std::vector<T>& GetUnsafe() const {
    return items_;
  }

private:
  std::vector<T> items_;
  std::mutex mut_;
};

int main(int argc, char *argv[]) {
  int opt = 0;
  std::uint64_t max_number = 0;

  while ((opt = getopt(argc, argv, "n:")) != -1) {
    switch (opt) {
    case 'n':
      {
        std::stringstream ss;
        ss << optarg;
        ss >> max_number;
      }
      break;
    default:
      std::cerr << "usage: " << std::endl;
      std::cerr << "  " << argv[0] << ": <-n> <upto-max>" << std::endl;
      break;
    }
  }

  std::cout << "generating data..." << std::endl;

  // you can use (n(first, last) / 2) for this, but the point of
  // this exercise is mostly to familiarize with concurrency in c++
  std::vector<std::uint64_t> numbers(max_number);
  {
    std::uint64_t counter = 1;
    std::generate(numbers.begin(), numbers.end(),
                  [&counter](){ return counter++; });
  }

  auto suggestion = psy::common::Granulator(max_number);
  std::cout << "number of items in vector: " << numbers.size() << std::endl;
  std::cout << "number of cores: " << suggestion.GetNumThreads() << std::endl;
  std::cout << "grain size     : " << suggestion.GetGrain() << std::endl;

  MTVector<std::uint64_t> mt_vector;
  std::vector<std::thread> threads;

  auto work_fn = [](MTVector<std::uint64_t>& mtv,
                    const std::vector<std::uint64_t>& nms,
                    const std::pair<size_t,size_t>& rng) noexcept {
    std::uint64_t sum = 0;
    for (size_t i = rng.first; i <= rng.second; ++i) sum += nms[i];
    mtv.Add(sum);
  };

  for (size_t i = 0; i < suggestion.GetNumThreads(); ++i) {
    const auto range = suggestion.Step();

    threads.push_back(std::thread(work_fn,
                                  std::ref(mt_vector),
                                  std::ref(numbers),
                                  std::ref(range)));
  }

  for (auto& e : threads) e.join();

  std::uint64_t final_sum = 0;
  {
    auto parts = mt_vector.GetUnsafe();
    for (const auto& e : mt_vector.GetUnsafe())
      final_sum += e;
  }

  std::cout << "total sum: " << final_sum << std::endl;

  return 0;
}
