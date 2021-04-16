// TODO: redo with fork example
#include <cstdint>
#include <iostream>
#include <sstream>
#include <mutex>
#include <thread>
#include <vector>

// C - getopt
#include <unistd.h>

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

namespace Granulator {
}

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

  const size_t num_cores = std::thread::hardware_concurrency();

  // you can use (n(first, last) / 2) for this, but the point of
  // this exercise is mostly to familiarize with concurrency in c++
  MTVector<std::uint64_t> mt_vector;
  std::vector<std::uint64_t> vec(max_number);
  {
    std::uint64_t counter = 1;
    std::generate(vec.begin(),
                  vec.end(),
                  [&counter](){ return counter++; });
  }

  std::cout << "number of items in vector: " << vec.size() << std::endl;
  std::cout << "number of cores: " << num_cores << std::endl;

  auto fn =
    [&vec, &mt_vector]
    (std::vector<std::uint64_t>::iterator begin,
     std::vector<std::uint64_t>::iterator end) noexcept {
    std::uint64_t sum = 0;
    std::for_each(begin, end, [&sum](const std::uint64_t& val) { sum += val; });
    mt_vector.Add(sum);
  };

  const int offset = max_number / num_cores;

  std::vector<std::thread> threads;
  threads.reserve(num_cores);

  auto off_a = vec.begin(), off_b = vec.begin();
  std::advance(off_b, offset);

  for (size_t i = 0; i < num_cores - 1; ++i) {
    std::cout << "first: " << *off_a << " end:" << *off_b << std::endl;
    threads.push_back(std::thread(fn, off_a, off_b));
    std::advance(off_a, offset + 1);
    std::advance(off_b, offset);
  }

  std::cout << "first: " << *off_a << " end:" << *(vec.end()  - 1)<< std::endl;
  threads.push_back(std::thread(fn, off_a, vec.end()));

  for (auto it = threads.begin(); it != threads.end(); ++it)
    (*it).join();

  auto partial_sums = mt_vector.GetUnsafe();
  for (const auto& e : partial_sums) std::cout << "partial: " << e << std::endl;

  std::uint64_t final_sum = 0;
  std::for_each(partial_sums.cbegin(),
                partial_sums.cend(),
                [&final_sum](const std::uint64_t v) { final_sum += v; });

  std::cout << "final sum: " << final_sum << std::endl;

  return 0;
}
